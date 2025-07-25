/* pshm_write.cpp

   Usage: pshm_write shm-name string

   Copy 'string' into the POSIX shared memory object named in 'shm-name'.

   See also pshm_read.c.
*/
#include <stdio.h>
#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;

int
main(int argc, char *argv[])
{
    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        std::cerr << "Usage: " << argv[0] << "shm-name string\n";
        return 1;
    }

    shared_memory_object shdmem{open_only, argv[1], read_write}; /* Open existing object */

    size_t len = strlen(argv[2]);
    shdmem.truncate(len);   /* Resize object to hold string */
    printf("Resized to %ld bytes\n", (long) len);

    mapped_region region{shdmem, read_write};
    uint8_t* addr = static_cast<uint8_t*>(region.get_address());

    printf("copying %ld bytes\n", (long) len);
    memcpy(addr, argv[2], len);             /* Copy string to shared memory */

    return 0;
}
