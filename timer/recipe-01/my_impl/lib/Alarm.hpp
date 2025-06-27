#pragma once

#include <chrono>
#include <mutex>
#include <functional>

struct Alarm {
    using clock_type = std::chrono::system_clock;
    using duration = clock_type::duration;
    using time_point = clock_type::time_point;

    enum StateCode
    {
        INACTIVE = 0,
        ASYNC_WAITING,
        SYNC_WAITING,
    };

    Alarm* next=nullptr;
    StateCode state{INACTIVE};
    std::mutex mutex;
    time_point time=time_point::max();
    std::function<void()> callback;

    void reset();
    void wait();
    void wait_async();
};

