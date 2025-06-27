#include "Alarm.hpp"
#include "TimerError.hpp"

#include <thread>

void Alarm::reset() {
    std::lock_guard<std::mutex> lck{mutex};
    state = INACTIVE;
    time = {};
    callback = {};
}

void Alarm::wait() {
    {
        std::lock_guard<std::mutex> lck{mutex};
        if (state != INACTIVE) {
            throw TimerError("invalid state");
        }
        state = SYNC_WAITING;
    }

    if (time <= std::chrono::system_clock::now()) {
        return;
    }

    std::this_thread::sleep_until(time);

    {
        std::lock_guard<std::mutex> lck{mutex};
        state = INACTIVE;
    }
}
