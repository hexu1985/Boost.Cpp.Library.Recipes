/* pmsg_unlink.cpp

   Usage: pmsg_unlink mq-name

   Unlink a POSIX message queue.

   Linux supports POSIX message queues since kernel 2.6.6.
*/
#include <iostream>
#include <boost/interprocess/ipc/message_queue.hpp>

using namespace boost::interprocess;

int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        std::cerr << "Usage: " << argv[0] << "mq-name\n";
        return 1;
    }

    message_queue::remove(argv[1]);

    return 0;
}
