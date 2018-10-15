#pragma once
#include <algorithm>
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
		: base_(value)
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
	std::vector<Change> changes_;
};

template<class T>
T Attribute<T>::getTotal() const
{
	auto value = base_;
	for (const auto& change : changes_)
	{
		value += change.diff;
	}
	return value;
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