#include <iostream>
#include <chrono>
#include <functional>
#include <boost/asio.hpp>

#include "print_message.hpp"

using namespace boost;
using namespace std::placeholders;

void periodic_task(asio::system_timer& timer, const boost::system::error_code& ec) {
    if (ec == asio::error::operation_aborted) {
        print_message("任务取消了");
        return;
    }

    print_message("周期性任务执行");
    // 再次设置定时器以实现周期性执行
    timer.expires_after(std::chrono::seconds(2));
    // also can use
    // timer.expires_at(timer.expiry() + std::chrono::seconds(2));
    timer.async_wait([&timer](const boost::system::error_code& ec) { periodic_task(timer, ec); });
}

int main() {
    asio::io_context io;
    
    // 第一次启动定时器
    asio::system_timer timer(io, std::chrono::seconds(2));
    timer.async_wait(std::bind(periodic_task, std::ref(timer), _1));

    print_message("周期性定时器已启动，每2秒执行一次");

    // 让程序运行一段时间
    asio::system_timer timer2(io, std::chrono::seconds(10));
    timer2.async_wait([&timer](const boost::system::error_code& ec) { print_message("10秒过去了"); timer.cancel(); });

    io.run();
    
    return 0;
}


