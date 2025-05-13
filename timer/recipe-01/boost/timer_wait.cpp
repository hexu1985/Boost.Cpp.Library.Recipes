#include <iostream>
#include <chrono>
#include <boost/asio.hpp>

using boost::asio::io_context;
using boost::asio::steady_timer;

int main() {
    io_context io;
    
    for (int i = 0; i < 5; ++i) {
        steady_timer timer(io, std::chrono::seconds(1));
        timer.wait();
        std::cout << "Hello, world! " << i << std::endl;
    }
    
    return 0;
}
