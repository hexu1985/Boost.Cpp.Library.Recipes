#include <iostream>
#include <chrono>
#include <thread>
#include <cctype>
#include <boost/asio.hpp>

#include "print_message.hpp"

using boost::asio::io_context;
using boost::asio::steady_timer;

void task(const boost::system::error_code& /*e*/) {
    print_message("任务执行了");
}

int main() {
    io_context io;
    
    steady_timer timer(io, std::chrono::seconds(10));
    timer.async_wait(task);

    // start io_context
    std::thread io_thread([&io]() { io.run(); });

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

    io_thread.join();
    
    return 0;
}

