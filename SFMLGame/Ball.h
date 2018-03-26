#pragma once
#include <SFML\Graphics.hpp>

#include "Paddle.h"

class Paddle;


class Ball : public sf::CircleShape
{
public:
	static const float DEFAULT_RADIUS;
	static const float DEFAULT_VELOCITY;

	Ball(float x, float y);

	void update();
	void setVelocity(sf::Vector2f v) { velocity = v; }
	void checkCollision(const Paddle& p);

	sf::Vector2f getVelocity() const { return velocity; }
	float getMinX() const { return getPosition().x - getRadius(); }
	float getMaxX() const { return getPosition().x + getRadius(); }
	float getMinY() const { return getPosition().y - getRadius(); }
	float getMaxY() const { return getPosition().y + getRadius(); }
	bool isIntersecting(const Paddle& p) const;

private:
	sf::Vector2f velocity;
};