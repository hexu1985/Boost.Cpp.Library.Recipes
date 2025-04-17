/* psem_create.cpp

   Create a POSIX named semaphore.

   Usage as shown in usageError().

   On Linux, named semaphores are supported with kernel 2.6 or later, and
   a glibc that provides the NPTL threading implementation.
*/
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/interprocess/permissions.hpp>
#include <unistd.h>
#include <iostream>
#include <string>
#include <memory>
#include <stdio.h>

using namespace boost::interprocess;

static void
usageError(const char *progName)
{
    fprintf(stderr, "Usage: %s [-cx] name [octal-perms [value]]\n", progName);
    fprintf(stderr, "    -c   Create semaphore (O_CREAT)\n");
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

    if (optind >= argc)
        usageError(argv[0]);

    /* Default permissions are rw-------; default semaphore initialization
       value is 0 */

    permissions perms;
    if (argc <= optind + 1) {
        perms.set_default();
    } else {
        perms.set_permissions(std::stoul(argv[optind + 1], nullptr, 8));
    }

    unsigned int value; 
    if (argc <= optind + 2) {
        value = 0;
    } else {
        value = std::stoul(argv[optind + 2]);
    }

    std::unique_ptr<named_semaphore> sem;
    if (flags == open_bit) {
        sem = std::make_unique<named_semaphore>(open_only, argv[optind]);
    } else if (flags == open_bit + create_bit) {
        sem = std::make_unique<named_semaphore>(open_or_create, argv[optind], value, perms);
    } else if (flags == open_bit + create_bit + exist_bit) {
        sem = std::make_unique<named_semaphore>(create_only, argv[optind], value, perms);
    } else {
        std::cerr << "invalid flags: " << flags << '\n';
        exit(1);
    }

    return 0;
}
