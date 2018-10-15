#pragma once
#include <chrono>

//duration in seconds
using duration_t = std::chrono::duration<int, std::ratio<1>>;
using default_clock = std::chrono::system_clock;
using time_point_t = std::chrono::time_point<default_clock>;


class Timer
{
public:
	Timer(duration_t duration)
		: duration_(duration)
	{}

	void extend() { initTime_ = default_clock::now(); }
	bool isExpired() const
	{
		auto dur = default_clock::now() - initTime_;
		return dur > duration_;
	}

private:
	duration_t duration_;
	time_point_t initTime_ = default_clock::now();
};