/* psem_unlink.cpp

   Unlink a POSIX named semaphore.

   On Linux, named semaphores are supported with kernel 2.6 or later, and
   a glibc that provides the NPTL threading implementation.
*/
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <iostream>

using namespace boost::interprocess;

int
main(int argc, char *argv[])
{
    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        std::cerr << argv[0] << " sem-name\n";
        exit(1);
    }

    if (named_semaphore::remove(argv[1])) {
        std::cout << "sem_unlink() succeeded\n";
    } else {
        std::cout << "sem_unlink() failed\n";
    }

    return 0;
}
