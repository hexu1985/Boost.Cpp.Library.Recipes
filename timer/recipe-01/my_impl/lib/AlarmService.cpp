#include "AlarmService.hpp"

#define CHECK_ALARM_THREAD_STOP()  if (stop_) return;

AlarmService::AlarmService() {
    alarm_head_ = new Alarm{};
    alarm_thread_ = std::thread(&AlarmService::run, this);
}

AlarmService::~AlarmService() {
    stop();
    alarm_thread_.join();
    delete alarm_head_;
}

void AlarmService::insert_alarm(Alarm* alarm) {
    if (is_in_alarm_thread()) {
        insert_alarm_nts(alarm);
    } else {
        std::lock_guard<std::mutex> lck{alarm_mutex_};
        insert_alarm_nts(alarm);
    }
}

void AlarmService::remove_alarm(Alarm* alarm) {
    if (is_in_alarm_thread()) {
        remove_alarm_nts(alarm);
    } else {
        std::lock_guard<std::mutex> lck{alarm_mutex_};
        remove_alarm_nts(alarm);
    }
}

void AlarmService::insert_alarm_nts(Alarm* alarm) {
    Alarm* link = alarm_head_;
    while (link->next != nullptr) {
        if (alarm->time < link->next->time) {
            break;
        }
        link = link->next;
    }

    alarm->next = link->next;
    link->next = alarm;

    if (alarm->time < current_alarm_) {
        current_alarm_ = alarm->time;
        alarm_cond_.notify_one();
    }
}

void AlarmService::remove_alarm_nts(Alarm* alarm) {
    Alarm* link = alarm_head_;
    while (link->next != nullptr) {
        if (link->next == alarm) {
            link->next = alarm->next;
            break;
        }
        link = link->next;
    }
}

void AlarmService::run() {
    Alarm* alarm;
    time_point now;
    bool expired;
    std::cv_status status;

    std::unique_lock<std::mutex> lock(alarm_mutex_);
    while (!stop_) {
        current_alarm_ = time_point::max();
        while (is_alam_list_empty()) {
            CHECK_ALARM_THREAD_STOP();
            alarm_cond_.wait(lock);
        }

        CHECK_ALARM_THREAD_STOP();

        alarm = alarm_head_->next;
        alarm_head_->next = alarm->next;
        now = clock_type::now();
        expired = false;

        if (now < alarm->time) {
            current_alarm_ = alarm->time;
            while (current_alarm_ == alarm->time) {
                CHECK_ALARM_THREAD_STOP();
                status = alarm_cond_.wait_until(lock, alarm->time);
                if (status == std::cv_status::timeout) {
                    expired = true;
                    break;
                }
            }
            CHECK_ALARM_THREAD_STOP();
            if (!expired) {
                insert_alarm_nts(alarm);
            }
        } else {
            expired = true;
        }

        if (expired) {
            lock.unlock();
            alarm->on_expired();
            lock.lock();
        }
    }
}

void AlarmService::stop() {
    stop_ = true;
    alarm_cond_.notify_one();
}

bool AlarmService::is_alam_list_empty() {
    return alarm_head_->next == nullptr;
}

bool AlarmService::is_in_alarm_thread() {
    return std::this_thread::get_id() == alarm_thread_.get_id();
}

AlarmService* AlarmService::get_service() {
    static AlarmService alarm_service;
    return &alarm_service;
}
