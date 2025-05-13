#include <iostream>
#include <chrono>
#include <asio.hpp>

#include "print_message.hpp"

using asio::io_context;
using asio::steady_timer;

void hello(const std::error_code& /*e*/) {
    print_message("Hello, World!");
}

int main() {
    io_context io;
    
    steady_timer timer(io, std::chrono::seconds(5));
    timer.async_wait(hello);

    print_message("定时器已启动，等待5秒...");
    io.run();
    
    return 0;
}

