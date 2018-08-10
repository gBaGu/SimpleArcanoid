#pragma once
#include <SFML\Graphics.hpp>


class Ball : public sf::CircleShape
{
public:
	static const float DEFAULT_RADIUS;
	static const float DEFAULT_VELOCITY;

	Ball(float x, float y);

	bool checkCollision(const sf::RectangleShape& p);
	void setVelocity(sf::Vector2f v) { velocity = v; }
	void stop();
	void update();

	sf::Vector2f getVelocity() const { return velocity; }
	float getMinX() const { return getPosition().x - getRadius(); }
	float getMaxX() const { return getPosition().x + getRadius(); }
	float getMinY() const { return getPosition().y - getRadius(); }
	float getMaxY() const { return getPosition().y + getRadius(); }
	/*
	returns:
	true if Ball is inside of p or intersecting it
	false if Ball is outside of p
	*/
	bool isIntersecting(const sf::RectangleShape& p) const;

private:
	sf::Vector2f velocity;
};