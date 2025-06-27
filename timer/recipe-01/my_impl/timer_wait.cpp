#include <iostream>
#include <chrono>
#include <string>
#include "Timer.hpp"

#include "print_message.hpp"

int main() {
    for (int i = 0; i < 5; ++i) {
        Timer timer(std::chrono::seconds(1));
        timer.wait();
        print_message("Hello, world! " + std::to_string(i));
    }
    
    return 0;
}
