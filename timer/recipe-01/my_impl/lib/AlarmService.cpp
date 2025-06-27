#include "AlarmService.hpp"

AlarmService::AlarmService() {
    alarm_head_ = new Alarm{};
}

void AlarmService::insert_alarm(Alarm* alarm) {
}

void AlarmService::remove_alarm(Alarm* alarm) {
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
}

