/* thread_incr_psem.cpp

   Use a POSIX unnamed semaphore to synchronize access by two threads to
   a global variable.

   See also thread_incr.c and thread_incr_mutex.c.
*/
#include <thread>
#include <iostream>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

using namespace boost::interprocess;

static int glob = 0;

/* Initialize a semaphore with the value 1 */
static interprocess_semaphore sem{1};

static void                   /* Loop 'arg' times incrementing 'glob' */
threadFunc(int loops)
{
    int loc, j;

    for (j = 0; j < loops; j++) {
        sem.wait();

        loc = glob;
        loc++;
        glob = loc;

        sem.post();
    }
}

int
main(int argc, char *argv[])
{
    std::thread t1, t2;
    int loops, s;

    loops = (argc > 1) ? std::stoi(argv[1]) : 10000000;

    /* Create two threads that increment 'glob' */

    t1 = std::thread(threadFunc, loops);
    t2 = std::thread(threadFunc, loops);

    /* Wait for threads to terminate */

    t1.join();
    t2.join();

    std::cout << "glob = " << glob << "\n";

    return 0;
}
