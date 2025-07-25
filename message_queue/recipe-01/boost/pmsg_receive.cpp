/* pmsg_receive.cpp

   Usage as shown in usageError().

   Receive a message from a POSIX message queue, and write it on
   standard output.

   See also pmsg_send.cpp.

   Linux supports POSIX message queues since kernel 2.6.6.
*/
#include <iostream>
#include <string>
#include <boost/interprocess/ipc/message_queue.hpp>

#include "cxxopts.hpp"

using namespace boost::interprocess;

static void
printUsage(const char *progName)
{
    std::cerr << "Usage: " << progName << "[-n] mq-name\n"
              << "    -n           Use O_NONBLOCK flag\n";
}

int
main(int argc, char *argv[])
{
    cxxopts::Options options("pmsg_receive", "receive from message queue");

    options.add_options()
        ("h,help", "Print help")
        ("n", "Use O_NONBLOCK flag", cxxopts::value<bool>()->default_value("false"))
        ("name", "mq-name", cxxopts::value<std::string>());

    options.parse_positional({"name"});

    try {
        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            printUsage(argv[0]);
            return 0;
        }

        if (!result.count("name")) {
            printUsage(argv[0]);
            return 1;
        }

        std::string name = result["name"].as<std::string>();
        bool non_block = result["n"].as<bool>();

        message_queue mq{open_only, name.c_str()};
        size_t msgsize = mq.get_max_msg_size();
        std::unique_ptr<char[]> buffer{new char[msgsize]};

        size_t num_read = 0;
        unsigned int prio = 0;
        if (non_block) {
            mq.try_receive(buffer.get(), msgsize, num_read, prio);
        } else {
            mq.receive(buffer.get(), msgsize, num_read, prio);
        }

        std::cout << "Read " << num_read << " bytes; priority = " << prio << "\n";
        std::cout << std::string(buffer.get(), num_read) << std::endl;
    } catch (const cxxopts::exceptions::exception& e) {
        // 捕获解析错误（如缺少必需参数、类型不匹配）
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << options.help() << std::endl;
        return 1;
    }

    return 0;
}
