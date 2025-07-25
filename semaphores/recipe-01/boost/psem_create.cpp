/* psem_create.cpp

   Create a POSIX named semaphore.

   Usage as shown in usageError().

   On Linux, named semaphores are supported with kernel 2.6 or later, and
   a glibc that provides the NPTL threading implementation.
*/
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <boost/interprocess/permissions.hpp>
#include <iostream>
#include <string>
#include <memory>

#include "cxxopts.hpp"

using namespace boost::interprocess;

static void
printUsage(const char *progName)
{
    std::cerr << "Usage: " << progName << "[-cx] name [octal-perms [value]]\n"
              <<  "    -c   Create semaphore (O_CREAT)\n"
              <<  "    -x   Create exclusively (O_EXCL)\n";
}

int
main(int argc, char *argv[])
{
    cxxopts::Options options("pshm_create", "create shared memory");

    options.add_options()
        ("h,help", "Print help")
        ("c", "Create semaphore (O_CREAT)", cxxopts::value<bool>()->default_value("false"))
        ("x", "Create exclusively (O_EXCL)", cxxopts::value<bool>()->default_value("false"))
        ("name", "shm-name", cxxopts::value<std::string>())
        ("perms", "octal-perms", cxxopts::value<std::string>())
        ("value", "value", cxxopts::value<unsigned int>());

    options.parse_positional({"name", "perms", "value"});

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
        unsigned int value = 0;
        if (result.count("value")) {
            value = result["value"].as<unsigned int>();
        }

        permissions perms;
        if (result.count("perms")) {
            perms.set_permissions(
                    std::stoul(result["perms"].as<std::string>(), nullptr, 8));
        } else {
            perms.set_default();
        }

        std::unique_ptr<named_semaphore> sem;
        if (result["c"].as<bool>() && result["x"].as<bool>()) {
            sem = std::make_unique<named_semaphore>(create_only, name.c_str(), value, perms);
        } else if (result["c"].as<bool>()) {
            sem = std::make_unique<named_semaphore>(open_or_create, name.c_str(), value, perms);
        } else {
            sem = std::make_unique<named_semaphore>(open_only, name.c_str());
        }
    } catch (const cxxopts::exceptions::exception& e) {
        // 捕获解析错误（如缺少必需参数、类型不匹配）
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << options.help() << std::endl;
        return 1;
    }

    return 0;
}
