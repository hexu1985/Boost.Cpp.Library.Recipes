#include <iostream>
#include <chrono>
#include <functional>
#include <thread>
#include "Timer.hpp"

#include "print_message.hpp"

using namespace std::placeholders;

void periodic_task(Timer& timer, std::chrono::seconds interval, const std::error_code& ec);

void start_timer(Timer& timer, std::chrono::seconds interval) {
    // 再次设置定时器以实现周期性执行
    timer.expires_after(interval);
    // also can use
    // timer.expires_at(timer.expiry() + std::chrono::seconds(2));
    timer.async_wait([&timer, interval](const std::error_code& ec) { periodic_task(timer, interval, ec); });
}

void periodic_task(Timer& timer, std::chrono::seconds interval, const std::error_code& ec) {
    if (ec == TimerErrc::operation_aborted) {
        print_message("任务取消了");
        return;
    }

    print_message("周期性任务执行");
    start_timer(timer, interval);
}

int main() {
    // 第一次启动定时器
    Timer timer;
    start_timer(timer, std::chrono::seconds(2));

    print_message("周期性定时器已启动，每2秒执行一次");

    // 让程序运行一段时间
    Timer timer2(std::chrono::seconds(10));
    timer2.async_wait([&timer](const std::error_code& ec) { print_message("10秒过去了");  timer.cancel(); });

    std::this_thread::sleep_for(std::chrono::seconds(15));
    
    return 0;
}


