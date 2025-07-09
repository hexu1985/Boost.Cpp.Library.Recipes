#include <iostream>
#include <thread>
#include <chrono>
#include "Timer.hpp"

int main() {
    Timer timer; // 默认构造，未设置超时时间

    // 检查默认的 expiry()
    auto expiry_time = timer.expiry();
    std::time_t t = std::chrono::system_clock::to_time_t(expiry_time);
    std::cout << "Default expiry: " << std::ctime(&t); // 通常是 epoch (1970-01-01)

    // 直接调用 async_wait() 不会有任何反应
    timer.async_wait([](const std::error_code& ec) {
        std::cout << "This will NOT be called!\n";
    });

    // 必须显式设置超时时间
    timer.expires_after(std::chrono::seconds(1));
    timer.async_wait([](const std::error_code& ec) {
        if (!ec) {
            std::cout << "Timer expired!\n";
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}
