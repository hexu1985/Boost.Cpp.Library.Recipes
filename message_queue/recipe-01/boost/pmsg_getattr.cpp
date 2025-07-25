/* pmsg_getattr.cpp

   Display attributes of a POSIX message queue.

   Linux supports POSIX message queues since kernel 2.6.6.
*/

#include <iostream>
#include <boost/interprocess/ipc/message_queue.hpp>

using namespace boost::interprocess;

int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        std::cerr << argv[0] << " mq-name\n";
        return 1;
    }

    message_queue mq{open_only, argv[1]};
    std::cout << "Maximum # of messages on queue:   " << mq.get_max_msg() << std::endl;
    std::cout << "Maximum message size:             " << mq.get_max_msg_size() << std::endl;
    std::cout << "# of messages currently on queue: " << mq.get_num_msg() << std::endl; 

    return 0;
}
