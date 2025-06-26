#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>

#include <asio.hpp>

int main(int argc, char* argv[]) {
	if(argc!=2) {
		printf("Usage : %s <IP>\n", argv[0]);
		exit(1);
	}

    std::string ip_address = argv[1];

    try {
        asio::io_context io;
        asio::ip::tcp::resolver resolver(io);
        
        // 创建endpoint用于反向查询
        asio::ip::address addr = asio::ip::make_address(ip_address);
        asio::ip::tcp::endpoint endpoint(addr, 0); // 端口号设为0
        
        // 进行反向解析
        auto results = resolver.resolve(endpoint);

        if (!results.empty()) {
            std::cout << "Reverse DNS for " << ip_address << ":\n";
            std::cout << "  Official name: " << results.begin()->host_name() << "\n";
            
            // 打印所有结果（可能包含多个名称）
            for (const auto& entry : results) {
                if (entry.host_name() != results.begin()->host_name()) {
                    std::cout << "  Alias: " << entry.host_name() << "\n";
                }
            }
        } else {
            std::cout << "No reverse DNS record found for " << ip_address << "\n";
        }
    } catch (const std::system_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
