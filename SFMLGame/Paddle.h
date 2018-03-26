#pragma once
#include <SFML\Graphics.hpp>

#include "Ball.h"

class Ball;


class Paddle : public sf::RectangleShape
{
public:
	static const float DEFAULT_WIDTH;
	static const float DEFAULT_HEIGHT;
	static const float DEFAULT_VELOCITY;

	Paddle(float x, float y);

	void update();
	void setVelocity(sf::Vector2f v) { velocity = v; }

	sf::Vector2f getVelocity() const { return velocity; }
	float getMinX() const { return getPosition().x - getSize().x / 2; }
	float getMaxX() const { return getPosition().x + getSize().x / 2; }
	float getMinY() const { return getPosition().y - getSize().y / 2; }
	float getMaxY() const { return getPosition().y + getSize().y / 2; }

private:
	sf::Vector2f velocity;
};