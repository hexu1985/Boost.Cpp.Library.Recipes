#include <iostream>
#include <chrono>
#include <functional>
#include <asio.hpp>

#include "print_message.hpp"

using asio::io_context;
using asio::steady_timer;
using namespace std::placeholders;

void periodic_task(steady_timer& timer, const std::error_code& /*e*/) {
    print_message("周期性任务执行");
    // 再次设置定时器以实现周期性执行
    timer.expires_after(std::chrono::seconds(2));
    // also can use
    // timer.expires_at(timer.expiry() + std::chrono::seconds(2));
    timer.async_wait([&timer](const std::error_code& e) { periodic_task(timer, e); });
}

int main() {
    io_context io;
    
    // 第一次启动定时器
    steady_timer timer(io, std::chrono::seconds(2));
    timer.async_wait(std::bind(periodic_task, std::ref(timer), _1));

    print_message("周期性定时器已启动，每2秒执行一次");

    // 让程序运行一段时间
    steady_timer timer2(io, std::chrono::seconds(10));
    timer2.async_wait([](const std::error_code& e) { print_message("10秒过去了"); });

    io.run();
    
    return 0;
}


