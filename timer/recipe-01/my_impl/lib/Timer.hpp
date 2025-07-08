#pragma once

#include <memory>
#include <chrono>
#include <functional>
#include <system_error>

class Alarm;

class Timer {
public:
    using clock_type = std::chrono::system_clock;
    using duration = clock_type::duration;
    using time_point = clock_type::time_point;

public:
    // Constructor
    Timer();

    // Constructor to set a particular expiry time as an absolute time. 
    Timer(const time_point& expiry_time);

    // Constructor to set a particular expiry time relative to now. 
    Timer(const duration& expiry_time);

    // Destroys the timer.
    ~Timer();

    // Set the timer's expiry time relative to now. 
    void expires_after(const duration& expiry_time);

    // Set the timer's expiry time as an absolute time. 
    void expires_at(const time_point& expiry_time);

    // Get the timer's expiry time as an absolute time. 
    time_point expiry() const;

    // Start an asynchronous wait on the timer.
    void async_wait(std::function<void(const std::error_code&)> callback);

    // Perform a blocking wait on the timer. 
    void wait();

    // Cancel any asynchronous operations that are waiting on the timer. 
    void cancel();

private:
    std::unique_ptr<Alarm> alarm_ptr_;
};
