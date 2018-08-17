#pragma once
#include <functional>

#include "MyTime.h"


class Effect
{
public:
	Effect(std::function<void()> f, duration_t duration)
		: undo_(f), duration_(duration) {}
	~Effect() { undo_(); }

	void extend() { initTime_ = default_clock::now(); }
	bool isExpired() const;

private:
	std::function<void()> undo_;
	duration_t duration_;
	time_point_t initTime_ = default_clock::now();
};