/* psem_post.cpp

   Increase the value of a POSIX named semaphore.

   See also psem_wait.c.

   On Linux, named semaphores are supported with kernel 2.6 or later, and
   a glibc that provides the NPTL threading implementation.
*/
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <iostream>

using namespace boost::interprocess;

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << argv[0] << " sem-name\n";
        exit(1);
    }

    named_semaphore sem{open_only, argv[1]};
    sem.post();

    return 0;
}
