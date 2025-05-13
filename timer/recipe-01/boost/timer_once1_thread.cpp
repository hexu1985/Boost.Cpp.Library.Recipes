#include <iostream>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

#include "print_message.hpp"

using boost::asio::io_context;
using boost::asio::steady_timer;

void hello(const boost::system::error_code& /*e*/) {
    print_message("Hello, World!");
}

int main() {
    io_context io;
    
    steady_timer timer(io, std::chrono::seconds(5));
    timer.async_wait(hello);

    print_message("定时器已启动，等待5秒...");

    std::thread io_thread([&io]() { io.run(); });
    io_thread.join();
    
    return 0;
}

