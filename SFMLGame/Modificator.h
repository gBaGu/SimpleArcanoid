#pragma once
#include <memory>

#include <SFML\Graphics.hpp>

#include "Ball.h"
#include "MyTime.h"

//TODO: remove Ball from this file
// use Object instead


class Modificator : public sf::RectangleShape
{
public:
	static const float DEFAULT_WIDTH;
	static const float DEFAULT_HEIGHT;
	static const float DEFAULT_SPEED;
	static const sf::Vector2f DEFAULT_VELOCITY;
	static const size_t DEFAULT_MODIFICATION_DURATION;

	Modificator(float x, float y);

	virtual void trigger() = 0;
	void update();

private:
	float speed_;
	sf::Vector2f velocity_;
};


class BallSpeedPositiveModificator : public Modificator
{
public:
	static const float DEFAULT_MULTIPLIER;

	BallSpeedPositiveModificator(float x, float y, std::shared_ptr<Ball> ball)
		: Modificator(x, y), ball_(ball)
	{}

	virtual void trigger();

private:
	std::shared_ptr<Ball> ball_;
};