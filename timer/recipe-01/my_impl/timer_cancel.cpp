#include <iostream>
#include <chrono>
#include <thread>
#include <cctype>
#include "Timer.hpp"

#include "print_message.hpp"

void task(const std::error_code& ec) {
    if (!ec) {
        print_message("任务执行了");
    } else if (ec == TimerErrc::operation_aborted) {
        print_message("任务取消了");
    }
}

int main() {
    Timer timer(std::chrono::seconds(10));
    timer.async_wait(task);

    print_message("定时器已启动，你有5秒时间决定是否取消");
    std::this_thread::sleep_for(std::chrono::seconds(5));  // 等待5秒

    // 5秒后询问是否取消
    std::cout << "要取消定时器吗？(y/n): ";
    std::string user_input;
    std::cin >> user_input;
    if (std::tolower(user_input[0]) == 'y') {
        timer.cancel();
        print_message("定时器已取消");
    } else {
        print_message("定时器将继续运行");
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));
    
    return 0;
}

