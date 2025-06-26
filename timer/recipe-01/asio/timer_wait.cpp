#include <iostream>
#include <chrono>
#include <string>
#include <asio.hpp>
#include "print_message.hpp"

using asio::io_context;
using asio::steady_timer;

int main() {
    io_context io;
    
    for (int i = 0; i < 5; ++i) {
        steady_timer timer(io, std::chrono::seconds(1));
        timer.wait();
        print_message("Hello, world! " + std::to_string(i));
    }
    
    return 0;
}
