#pragma once
#include <memory>

#include <SFML\Graphics.hpp>

#include "Ball.h"
#include "MyTime.h"
#include "Object.h"


class Modifier : public RectangleObject
{
public:
	static const float DEFAULT_WIDTH;
	static const float DEFAULT_HEIGHT;
	static const float DEFAULT_SPEED;
	static const sf::Vector2f DEFAULT_VELOCITY;
	static const size_t DEFAULT_MODIFICATION_DURATION;

	Modifier(float x, float y);

	virtual void trigger() = 0;

private:
};


class ObjectSpeedPositiveModifier : public Modifier
{
public:
	static const float DEFAULT_MULTIPLIER;

	ObjectSpeedPositiveModifier(float x, float y, std::shared_ptr<Object> obj)
		: Modifier(x, y), obj_(obj)
	{}

	virtual void trigger();

private:
	std::shared_ptr<Object> obj_;
};