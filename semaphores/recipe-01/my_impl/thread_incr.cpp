/* thread_incr.cpp

   This program employs two POSIX threads that increment the same global
   variable, without using any synchronization method. As a consequence,
   updates are sometimes lost.

   See also thread_incr_mutex.c.
*/
#include <thread>
#include <iostream>

static volatile int glob = 0;   /* "volatile" prevents compiler optimizations
                                   of arithmetic operations on 'glob' */

static void                   /* Loop 'arg' times incrementing 'glob' */
threadFunc(int loops)
{
    int loc, j;

    for (j = 0; j < loops; j++) {
        loc = glob;
        loc++;
        glob = loc;
    }
}

int
main(int argc, char *argv[])
{
    std::thread t1, t2;
    int loops;

    loops = (argc > 1) ? std::stoi(argv[1]) : 10000000;

    t1 = std::thread(threadFunc, loops);
    t2 = std::thread(threadFunc, loops);

    t1.join();
    t2.join();

    std::cout << "glob = " << glob << "\n";

    return 0;
}
