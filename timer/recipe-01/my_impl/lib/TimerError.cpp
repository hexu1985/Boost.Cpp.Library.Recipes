#include "TimerError.hpp"

const char* TimerCategory::name() const noexcept {
    return "TimerCategory";
}

std::string TimerCategory::message(int ev) const {
    switch (static_cast<TimerErrc>(ev)) {
        case TimerErrc::success: return "Success";
        case TimerErrc::operation_aborted: return "Timer was cancelled";
        default: return "Unknown error";
    }
}

std::error_condition TimerCategory::default_error_condition (int ev) const noexcept {
    switch (static_cast<TimerErrc>(ev)) {
        case TimerErrc::success: return std::error_condition{TimerErrc::success};
        case TimerErrc::operation_aborted: return std::error_condition{TimerErrc::operation_aborted};
        default: return std::error_condition{TimerErrc::unknown_error};
    }
}

bool TimerCategory::equivalent (const std::error_code& code, int condition) const noexcept {
    return *this==code.category() &&
        static_cast<int>(default_error_condition(code.value()).value())==condition;
}

const TimerCategory& timer_category() {
    static TimerCategory category;
    return category;
}

std::error_code make_error_code(TimerErrc e) {
    return {static_cast<int>(e), timer_category()};
}

std::error_condition make_error_condition(TimerErrc e) {
    return {static_cast<int>(e), timer_category()};
}
