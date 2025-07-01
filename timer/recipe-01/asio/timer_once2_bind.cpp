#include <iostream>
#include <chrono>
#include <functional>
#include <asio.hpp>

#include "print_message.hpp"

using namespace std::placeholders;

void greet(const std::string& name, const std::error_code& ec) {
    if (!ec) {
        print_message("Hello, "+name+"!");
    } else if (ec == asio::error::operation_aborted) {
        print_message("任务取消了");
    }
}

int main() {
    asio::io_context io;
    
    std::string name = "Alice";
    asio::system_timer timer(io, std::chrono::seconds(5));
    timer.async_wait(std::bind(greet, name, _1));

    print_message("定时器已启动，等待5秒...");
    io.run();
    
    return 0;
}

