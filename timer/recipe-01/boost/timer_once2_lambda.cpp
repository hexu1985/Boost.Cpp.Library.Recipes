#include <iostream>
#include <chrono>
#include <boost/asio.hpp>

#include "print_message.hpp"

using boost::asio::io_context;
using boost::asio::steady_timer;
using namespace std::placeholders;

void greet(const std::string& name, const boost::system::error_code& /*e*/) {
    print_message("Hello, "+name+"!");
}

int main() {
    io_context io;
    
    std::string name = "Alice";
    steady_timer timer(io, std::chrono::seconds(5));
    timer.async_wait([name](const boost::system::error_code& e) { greet(name, e); });

    print_message("定时器已启动，等待5秒...");
    io.run();
    
    return 0;
}

