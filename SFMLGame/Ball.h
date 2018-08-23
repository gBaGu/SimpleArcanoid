#pragma once
#include <memory>
#include <vector>

#include <SFML\Graphics.hpp>

#include "Effect.h"

#ifdef SFMLGAME_EXPORTS  
#define SFMLGAME_API __declspec(dllexport)   
#else  
#define SFMLGAME_API __declspec(dllimport)   
#endif


class Ball : public sf::CircleShape
{
public:
	static const float DEFAULT_RADIUS;
	static const float DEFAULT_SPEED;
	static const sf::Vector2f DEFAULT_VELOCITY;

	SFMLGAME_API Ball(float x, float y);
	SFMLGAME_API ~Ball();

	/*
	checks if there is a collision between Ball and RectangleShape
	if there is one, modifies the velocity of Ball and returns true
	else returns false
	*/
	bool checkCollision(const sf::RectangleShape& rs);
	void decreaseSpeed();
	void increaseSpeed();
	void setRadius(float r, bool updateOrigin);
	void setSpeed(float speed) { speed_ = speed; }
	void setVelocity(sf::Vector2f velocity);
	void stop();
	void update();

	float getSpeed() const { return speed_; }
	sf::Vector2f getVelocity() const { return velocity_; }
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
	float speed_;
	sf::Vector2f velocity_;

	std::vector<std::shared_ptr<Effect>> activeEffects_;
	std::weak_ptr<Effect> speedEffect_;
};