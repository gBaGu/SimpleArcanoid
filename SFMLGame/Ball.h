#pragma once
#include <SFML\Graphics.hpp>


class Ball : public sf::CircleShape
{
public:
	static const float DEFAULT_RADIUS;
	static const float DEFAULT_VELOCITY;

	Ball(float x, float y);

	/*
	checks if there is a collision between Ball and RectangleShape
	if there is one, modifies the velocity of Ball and returns true
	else returns false
	*/
	bool checkCollision(const sf::RectangleShape& rs);
	void setRadius(float r, bool updateOrigin);
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
	bool isIntersecting(const sf::RectangleShape& rs) const;

private:
	sf::Vector2f velocity;
};