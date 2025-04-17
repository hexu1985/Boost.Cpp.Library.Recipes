/* pshm_create.cpp

   Create a POSIX shared memory object with specified size and permissions.

   Usage as shown in usageError().
*/
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/permissions.hpp>

using namespace boost::interprocess;

static void
usageError(const char *progName)
{
    fprintf(stderr, "Usage: %s [-cx] shm-name size [octal-perms]\n", progName);
    fprintf(stderr, "    -c   Create shared memory (O_CREAT)\n");
    fprintf(stderr, "    -x   Create exclusively (O_EXCL)\n");
    exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
    const int open_bit = 1;
    const int create_bit = 10;
    const int exist_bit = 100;

    int flags = open_bit;
    int opt;
    while ((opt = getopt(argc, argv, "cx")) != -1) {
        switch (opt) {
        case 'c':   flags += create_bit;        break;
        case 'x':   flags += exist_bit;         break;
        default:    usageError(argv[0]);
        }
    }

    if (optind + 1 >= argc)
        usageError(argv[0]);

    /* Create shared memory object and set its size */

    size_t size = std::stol(argv[optind + 1]);
    permissions perms;
    if (argc <= optind + 2) {
        perms.set_default();
    } else {
        perms.set_permissions(std::stoul(argv[optind + 2], nullptr, 8));
    }

    shared_memory_object shdmem;
    if (flags == open_bit) {
        shdmem = shared_memory_object{open_only, argv[optind], read_write};
    } else if (flags == open_bit + create_bit) {
        shdmem = shared_memory_object{open_or_create, argv[optind], read_write, perms};
    } else if (flags == open_bit + create_bit + exist_bit) {
        shdmem = shared_memory_object{create_only, argv[optind], read_write, perms};
    } else {
        std::cerr << "invalid flags: " << flags << '\n';
        exit(1);
    }

    shdmem.truncate(size);

    /* Map shared memory object */

    mapped_region region{shdmem, read_only};
    const uint8_t* addr = static_cast<const uint8_t*>(region.get_address());
    (void) addr;

    return 0;
}
