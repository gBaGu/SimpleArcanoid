#pragma once
#include <functional>


class Destroyable
{
public:
	Destroyable(int durability)
		: durability_(durability) {}

	bool isBroken() const { return durability_ <= 0; }

	void destroy() { durability_ = 0; }
	void setOnDestroy(std::function<void(float, float)> f) { onDestroy_ = f; }
	void takeDamage(int damage = 1) { durability_ -= damage; }

protected:
	std::function<void(float, float)> onDestroy_ = nullptr;

private:
	int durability_;
};