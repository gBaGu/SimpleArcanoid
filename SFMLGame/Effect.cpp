#include "Effect.h"


bool Effect::isExpired() const
{
	auto dur = default_clock::now() - initTime_;
	return dur > duration_;
}