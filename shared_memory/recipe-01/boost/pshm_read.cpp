/* pshm_read.cpp

   Usage: pshm_read shm-name

   Copy the contents of the POSIX shared memory object named in
   'name' to stdout.

   See also pshm_write.c.
*/
#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;

int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        std::cerr << "Usage: " << argv[0] << "shm-name\n";
        return 1;
    }

    shared_memory_object shdmem{open_only, argv[1], read_only};
    offset_t size;
    shdmem.get_size(size);

    mapped_region region{shdmem, read_only};
    const uint8_t* addr = static_cast<const uint8_t*>(region.get_address());

    std::string data((char*)addr, (size_t)size);
    std::cout << data << std::endl;

    return 0;
}
