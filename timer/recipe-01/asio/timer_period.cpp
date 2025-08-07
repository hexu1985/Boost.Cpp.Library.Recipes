#include <iostream>
#include <chrono>
#include <functional>
#include <asio.hpp>

#include "print_message.hpp"

using namespace std::placeholders;

void periodic_task(asio::system_timer& timer, std::chrono::seconds intervel, const std::error_code& ec);

void start_timer(asio::system_timer& timer, std::chrono::seconds intervel) {
    // 再次设置定时器以实现周期性执行
    timer.expires_after(intervel);
    // also can use
    // timer.expires_at(timer.expiry() + std::chrono::seconds(2));
    timer.async_wait([&timer, intervel](const std::error_code& ec) { periodic_task(timer, intervel, ec); });
}

void periodic_task(asio::system_timer& timer, std::chrono::seconds intervel, const std::error_code& ec) {
    if (ec == asio::error::operation_aborted) {
        print_message("任务取消了");
        return;
    }

    print_message("周期性任务执行");
    start_timer(timer, intervel);
}

int main() {
    asio::io_context io;
    
    // 第一次启动定时器
    asio::system_timer timer(io);
    start_timer(timer, std::chrono::seconds(2));

    print_message("周期性定时器已启动，每2秒执行一次");

    // 让程序运行一段时间
    asio::system_timer timer2(io, std::chrono::seconds(10));
    timer2.async_wait([&timer](const std::error_code& ec) { print_message("10秒过去了");  timer.cancel(); });

    io.run();
    
    return 0;
}


