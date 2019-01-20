#pragma once
#include <algorithm>
#include <numeric>
#include <vector>

#include "MyTime.h"

template<class T>
class Attribute
{
	struct Change
	{
		T diff;
		Timer timer;
	};

public:
	Attribute(T value)
		: base_(value), min_()
	{
		max_ = std::numeric_limits<T>::max();
	}
	Attribute(T value, T min, T max)
		: base_(value), min_(min), max_(max)
	{}

	T getTotal() const;
	T getBase() const { return base_; }
	void change(T diff);
	void change(T diff, duration_t dur);
	void removeExpired();
	void set(T value);
	void setBase(T value);

private:
	T base_;
	T min_, max_;
	std::vector<Change> changes_;
};

template<class T>
T Attribute<T>::getTotal() const
{
	auto value = std::accumulate(std::begin(changes_), std::end(changes_), base_,
		[](const T& val, const Change& ch) { return val + ch.diff; });
	return std::max(min_, std::min(max_, value));
}

template<class T>
void Attribute<T>::change(T diff)
{
	setBase(base_ + diff);
}

template<class T>
void Attribute<T>::change(T diff, duration_t dur)
{
	Change change{ diff, Timer(dur) };
	changes_.push_back(change);
}

template<class T>
void Attribute<T>::removeExpired()
{
	changes_.erase(std::remove_if(changes_.begin(), changes_.end(),
			[](const auto& change) { return change.timer.isExpired(); }),
		changes_.end());
}

template<class T>
void Attribute<T>::set(T value)
{
	setBase(value);
	changes_.clear();
}

template<class T>
void Attribute<T>::setBase(T value)
{
	base_ = value;
}