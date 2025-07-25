/* pshm_create.cpp

   Create a POSIX shared memory object with specified size and permissions.

   Usage as shown in usageError().
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/permissions.hpp>

#include "cxxopts.hpp"

using namespace boost::interprocess;

static void
printUsage(const char *progName)
{
    std::cerr << "Usage: " << progName << "[-cx] shm-name size [octal-perms]\n"
              << "    -c   Create shared memory (O_CREAT)\n"
              << "    -x   Create exclusively (O_EXCL)\n";
}

int
main(int argc, char *argv[])
{
    cxxopts::Options options("pshm_create", "create shared memory");

    options.add_options()
        ("h,help", "Print help")
        ("c", "Create shared memory (O_CREAT)", cxxopts::value<bool>()->default_value("false"))
        ("x", "Create exclusively (O_EXCL)", cxxopts::value<bool>()->default_value("false"))
        ("name", "shm-name", cxxopts::value<std::string>())
        ("size", "size", cxxopts::value<size_t>())
        ("perms", "octal-perms", cxxopts::value<std::string>());

    options.parse_positional({"name", "size", "perms"});

    try {
        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            printUsage(argv[0]);
            return 0;
        }

        if (!result.count("name") || !result.count("size")) {
            printUsage(argv[0]);
            return 1;
        }

        std::string name = result["name"].as<std::string>();
        size_t size = result["size"].as<size_t>();

        permissions perms;
        if (result.count("perms")) {
            perms.set_permissions(
                    std::stoul(result["perms"].as<std::string>(), nullptr, 8));
        } else {
            perms.set_default();
        }

        /* Create shared memory object and set its size */
        shared_memory_object shdmem;
        if (result["c"].as<bool>() && result["x"].as<bool>()) {
            shdmem = shared_memory_object{create_only, name.c_str(), read_write, perms};
        } else if (result["c"].as<bool>()) {
            shdmem = shared_memory_object{open_or_create, name.c_str(), read_write, perms};
        } else {
            shdmem = shared_memory_object{open_only, name.c_str(), read_write};
        }

        shdmem.truncate(size);

        /* Map shared memory object */

        mapped_region region{shdmem, read_only};
        const uint8_t* addr = static_cast<const uint8_t*>(region.get_address());
        (void) addr;

    } catch (const cxxopts::exceptions::exception& e) {
        // 捕获解析错误（如缺少必需参数、类型不匹配）
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << options.help() << std::endl;
        return 1;
    }

    return 0;
}
