#include <iostream>
#include <chrono>
#include <boost/asio.hpp>

#include "print_message.hpp"

using namespace boost;

void hello(const boost::system::error_code& ec) {
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
    io.run();
    
    return 0;
}

