#include <iostream>
#include <chrono>
#include <thread>
#include <asio.hpp>

#include "print_message.hpp"

void hello(const std::error_code& ec) {
    if (!ec) {
        print_message("任务执行了");
    } else if (ec == asio::error::operation_aborted) {
        print_message("任务取消了");
    }
}

int main() {
    asio::io_context io;
    
    asio::system_timer timer(io, std::chrono::seconds(5));
    timer.async_wait(hello);

    print_message("定时器已启动，等待5秒...");

    std::thread io_thread([&io]() { io.run(); });
    io_thread.join();
    
    return 0;
}

