#pragma once
#include <SFML\Graphics.hpp>


class Ball : public sf::CircleShape
{
public:
	static const float DEFAULT_RADIUS;
	static const float DEFAULT_VELOCITY;

	Ball(float x, float y);

	void update();
	void setVelocity(sf::Vector2f v) { velocity = v; }
	bool checkCollision(const sf::RectangleShape& p);

	sf::Vector2f getVelocity() const { return velocity; }
	float getMinX() const { return getPosition().x - getRadius(); }
	float getMaxX() const { return getPosition().x + getRadius(); }
	float getMinY() const { return getPosition().y - getRadius(); }
	float getMaxY() const { return getPosition().y + getRadius(); }
	bool isIntersecting(const sf::RectangleShape& p) const;

private:
	sf::Vector2f velocity;
};