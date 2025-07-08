#pragma once

#include <chrono>
#include <mutex>
#include <condition_variable>
#include <functional>

class AlarmService;

struct Alarm {
    using clock_type = std::chrono::system_clock;
    using duration = clock_type::duration;
    using time_point = clock_type::time_point;

    enum StateCode {
        kInactive,
        kPending, 
        kExpired,
        kCancelled
    };

    AlarmService* service=nullptr;
    Alarm* next=nullptr;
    StateCode state{kInactive};
    std::mutex mutex;
    time_point time{};
    std::function<void(const std::error_code&)> callback;

    Alarm(AlarmService* service_=nullptr);

    void on_expired();
    void on_cancelled();

    void cancel();
    void wait();
    void async_wait(std::function<void(const std::error_code&)> callback_);
    void update_time(const time_point& expiry_time);
};

