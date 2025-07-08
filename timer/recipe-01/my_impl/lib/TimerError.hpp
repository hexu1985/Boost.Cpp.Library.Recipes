#pragma once

#include <string>
#include <system_error>

enum class TimerErrc { 
    success=0, 
    operation_aborted,
    unknown_error=255
};

namespace std {
    template<> struct is_error_condition_enum<TimerErrc> : public true_type {};
}   // namespace std

class TimerCategory: public std::error_category {
public:
    const char* name() const noexcept override;
    std::string message(int ev) const override;
    std::error_condition default_error_condition (int ev) const noexcept override;
    bool equivalent (const std::error_code& code, int condition) const noexcept override;
};

const TimerCategory& timer_category();
std::error_code make_error_code(TimerErrc e); 
std::error_condition make_error_condition(TimerErrc e); 
