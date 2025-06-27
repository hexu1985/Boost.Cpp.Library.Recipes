#pragma once

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>

#include "Alarm.hpp"

class AlarmService {
public:
    using clock_type = std::chrono::system_clock;
    using duration = clock_type::duration;
    using time_point = clock_type::time_point;

    AlarmService();
    ~AlarmService();

    void insert_alarm(Alarm* alarm);
    void remove_alarm(Alarm* alarm);

    static AlarmService* get_service(); 

private:
    void insert_alarm_nts(Alarm* alarm);
    void remove_alarm_nts(Alarm* alarm);

    void run();
    void stop();

    bool is_alam_list_empty();
    bool is_in_alarm_thread();

private:
    Alarm* alarm_head_=nullptr;
    std::mutex alarm_mutex_;
    std::condition_variable alarm_cond_;
    time_point current_alarm_=time_point::max();
    std::thread alarm_thread_;
    std::atomic<bool> stop_{false};
};
