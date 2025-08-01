#include <iostream>
#include <chrono>
#include <string>
#include <asio.hpp>
#include "print_message.hpp"

int main() {
    asio::io_context io;
    
    for (int i = 0; i < 5; ++i) {
        asio::system_timer timer(io, std::chrono::seconds(1));
        timer.cancel();
        timer.wait();
        print_message("Hello, world! " + std::to_string(i));
    }
    
    return 0;
}
