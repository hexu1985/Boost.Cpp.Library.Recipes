#include <iostream>
#include <chrono>
#include <thread>
#include "Timer.hpp"

#include "print_message.hpp"

void hello(const std::error_code& ec) {
    if (!ec) {
        print_message("任务执行了");
    } else if (ec == TimerErrc::operation_aborted) {
        print_message("任务取消了");
    }
}

int main() {
    Timer timer(std::chrono::seconds(5));
    timer.async_wait(hello);

    print_message("定时器已启动，等待5秒...");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    
    return 0;
}

