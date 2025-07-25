/* pmsg_send.cpp

   Usage as shown in usageError().

   Send a message (specified as a command line argument) to a
   POSIX message queue.

   See also pmsg_receive.cpp.

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
    std::cerr << "Usage: " << progName << "[-n] mq-name msg [prio]\n"
              << "    -n           Use O_NONBLOCK flag\n";
}

int
main(int argc, char *argv[])
{
    cxxopts::Options options("pmsg_send", "send to message queue");

    options.add_options()
        ("h,help", "Print help")
        ("n", "Use O_NONBLOCK flag", cxxopts::value<bool>()->default_value("false"))
        ("name", "mq-name", cxxopts::value<std::string>())
        ("msg", "message", cxxopts::value<std::string>())
        ("prio", "priority", cxxopts::value<unsigned int>()->default_value("0"));

    options.parse_positional({"name", "msg", "prio"});

    try {
        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            printUsage(argv[0]);
            return 0;
        }

        if (!result.count("name") || !result.count("msg")) {
            printUsage(argv[0]);
            return 1;
        }

        std::string name = result["name"].as<std::string>();
        std::string msg = result["msg"].as<std::string>();
        unsigned int prio = result["prio"].as<unsigned int>();
        bool non_block = result["n"].as<bool>();

        message_queue mq{open_only, name.c_str()};

        if (non_block) {
            mq.try_send(msg.data(), msg.length(), prio);
        } else {
            mq.send(msg.data(), msg.length(), prio);
        }
    } catch (const cxxopts::exceptions::exception& e) {
        // 捕获解析错误（如缺少必需参数、类型不匹配）
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << options.help() << std::endl;
        return 1;
    }

    return 0;
}
