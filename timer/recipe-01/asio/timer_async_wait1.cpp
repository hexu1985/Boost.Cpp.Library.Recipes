#include <iostream>
#include <chrono>
#include <asio.hpp>

#include "print_message.hpp"

void hello(const std::error_code& ec) {
    if (!ec) {
        print_message("任务执行了");
    } else if (ec == asio::error::operation_aborted) {
        print_message("任务取消了");
    }
}

void hello2(const std::error_code& ec) {
    if (!ec) {
        print_message("2任务执行了");
    } else if (ec == asio::error::operation_aborted) {
        print_message("2任务取消了");
    }
}

int main() {
    asio::io_context io;
    
    asio::system_timer timer(io, std::chrono::seconds(5));
    timer.async_wait(hello);

    timer.expires_after(std::chrono::seconds(10));
    timer.async_wait(hello2);

    print_message("定时器已启动，等待5秒...");
    io.run();
    
    return 0;
}

