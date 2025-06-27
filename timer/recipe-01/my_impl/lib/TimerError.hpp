#pragma once

#include <string>
#include <stdexcept>

// error in timer class
class TimerError: public std::runtime_error
{
public:
	TimerError(const std::string& msg = ""):
		std::runtime_error(msg)
	{}
};
