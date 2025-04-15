/* pshm_unlink.cpp

   Usage: pshm_unlink shm-name

   Remove the POSIX shared memory object identified by 'name'
*/
#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp>

using namespace boost::interprocess;

int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        std::cerr << "Usage: " << argv[0] << "shm-name\n";
        exit(1);
    }

    shared_memory_object::remove(argv[1]);

    return 0;
}
