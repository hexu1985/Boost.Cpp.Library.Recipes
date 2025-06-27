#include "Timer.hpp"
#include "Alarm.hpp"

Timer::Timer(): alarm_ptr_(std::make_unique<Alarm>()) {
}

Timer::Timer(const time_point& expiry_time): alarm_ptr_(std::make_unique<Alarm>()) {
    alarm_ptr_->time = expiry_time;
}

Timer::Timer(const duration& expiry_time): alarm_ptr_(std::make_unique<Alarm>()) {
    alarm_ptr_->time = clock_type::now() + expiry_time;
}

Timer::~Timer() {
}

void Timer::wait() {
    return alarm_ptr_->wait();
}
