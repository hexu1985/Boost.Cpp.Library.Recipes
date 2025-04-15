/* pshm_unlink.cpp

   Usage: pshm_unlink shm-name

   Remove the POSIX shared memory object identified by 'name'
*/
#include <iostream>
#include <fmt/ranges.h>

using fmt::format;

int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        std::cerr << format("{} shm-name\n", argv[0]);
        exit(1);
    }

#if 0
    if (shm_unlink(argv[1]) == -1)
        errExit("shm_unlink");
    exit(EXIT_SUCCESS);
#endif
}
