#include "Alarm.hpp"
#include "AlarmService.hpp"
#include "TimerError.hpp"

#include <thread>
#include <string>
#include <iostream>

Alarm::Alarm(AlarmService* service): service(service) {
    std::cout << "service: " << (void *) service << std::endl;
}

void Alarm::on_expired() {
    {
        std::lock_guard<std::mutex> lck{mutex};
        if (state != kPending) {
            return;
        }
        state = kExpired;
    }
    if (callback) {
        callback();
    }
}

void Alarm::on_cancelled() {
}

void Alarm::cancel() {
    {
        std::lock_guard<std::mutex> lck{mutex};
        if (state != kPending) {
            return;
        }
        state = kCancelled;
        service->remove_alarm(this);
    }

    on_cancelled();
}

void Alarm::wait() {
    time_point expiry_time;
    {
        std::lock_guard<std::mutex> lck{mutex};
        expiry_time = time;
    }
    std::this_thread::sleep_until(expiry_time);
}

void Alarm::async_wait(std::function<void()> callback_) {
    std::lock_guard<std::mutex> lck{mutex};
    if (state == kPending) {
        service->remove_alarm(this);
    }
    callback = std::move(callback_);
    service->insert_alarm(this);
    state = kPending;
}

void Alarm::update_time(const time_point& expiry_time) {
    {
        std::lock_guard<std::mutex> lck{mutex};
        if (state != kPending) {
            time = expiry_time;
            return;
        } 
        state = kCancelled;
        service->remove_alarm(this);
    }

    on_cancelled();

    {
        std::lock_guard<std::mutex> lck{mutex};
        time = expiry_time;
        state = kInactive;
    }
}

