/* pmsg_create.cpp

   Create a POSIX message queue.

   Usage as shown in usageError().

   Linux supports POSIX message queues since kernel 2.6.6.
*/
#include <iostream>
#include <string>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/permissions.hpp>

#include "cxxopts.hpp"

using namespace boost::interprocess;

static void
printUsage(const char *progName)
{
    std::cerr << "Usage: " << progName << "[-cx] [-m maxmsg] [-s msgsize] mq-name "
             << "[octal-perms]\n"
             <<  "    -c          Create queue (O_CREAT)\n"
             <<  "    -m maxmsg   Set maximum # of messages\n"
             <<  "    -s msgsize  Set maximum message size\n"
             <<  "    -x          Create exclusively (O_EXCL)\n";
}

int
main(int argc, char *argv[])
{
    cxxopts::Options options("pmsg_create", "create message queue");

    options.add_options()
        ("h,help", "Print help")
        ("c", "Create queue (O_CREAT)", cxxopts::value<bool>()->default_value("false"))
        ("x", "Create exclusively (O_EXCL)", cxxopts::value<bool>()->default_value("false"))
        ("m,maxmsg", "Set maximum # of messages", cxxopts::value<size_t>()->default_value("10"))
        ("s,msgsize", "Set maximum message size", cxxopts::value<size_t>()->default_value("2048"))
        ("name", "mq-name", cxxopts::value<std::string>())
        ("perms", "octal-perms", cxxopts::value<std::string>());

    options.parse_positional({"name", "perms"});

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
        size_t maxmsg = result["maxmsg"].as<size_t>();
        size_t msgsize = result["msgsize"].as<size_t>();

        permissions perms;
        if (result.count("perms")) {
            perms.set_permissions(
                    std::stoul(result["perms"].as<std::string>(), nullptr, 8));
        } else {
            perms.set_default();
        }

        std::unique_ptr<message_queue> mq;
        if (result["c"].as<bool>() && result["x"].as<bool>()) {
            mq = std::make_unique<message_queue>(create_only, name.c_str(), maxmsg, msgsize, perms);
        } else if (result["c"].as<bool>()) {
            mq = std::make_unique<message_queue>(open_or_create, name.c_str(), maxmsg, msgsize, perms);
        } else {
            mq = std::make_unique<message_queue>(open_only, name.c_str());
        }
    } catch (const cxxopts::exceptions::exception& e) {
        // 捕获解析错误（如缺少必需参数、类型不匹配）
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << options.help() << std::endl;
        return 1;
    }

    return 0;
}
