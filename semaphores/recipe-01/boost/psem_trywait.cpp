/* psem_trywait.cpp

   Try to decrease the value of a POSIX named semaphore using the
   nonblocking sem_trywait() function.

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
        return 1;
    }

    named_semaphore sem{open_only, argv[1]};
    if (sem.try_wait()) {
        std::cout << "sem_trywait() succeeded\n";
    } else {
        std::cout << "sem_trywait() failed\n";
    }

    return 0;
}
