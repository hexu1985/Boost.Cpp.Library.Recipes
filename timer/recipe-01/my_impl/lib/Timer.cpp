#include "Timer.hpp"
#include "Alarm.hpp"
#include "AlarmService.hpp"

Timer::Timer(): 
    alarm_ptr_(std::make_unique<Alarm>(AlarmService::get_service())) {
}

Timer::Timer(const time_point& expiry_time): 
    alarm_ptr_(std::make_unique<Alarm>(AlarmService::get_service())) {
    alarm_ptr_->time = expiry_time;
}

Timer::Timer(const duration& expiry_time):
    alarm_ptr_(std::make_unique<Alarm>(AlarmService::get_service())) {
    alarm_ptr_->time = clock_type::now() + expiry_time;
}

Timer::~Timer() {
}

void Timer::async_wait(std::function<void()> callback) {
    return alarm_ptr_->async_wait(std::move(callback));
}

void Timer::wait() {
    return alarm_ptr_->wait();
}
