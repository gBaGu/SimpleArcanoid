#pragma once
#include <memory>
#include <vector>

#include <SFML\Graphics.hpp>

#include "Attribute.h"
#include "MyTime.h"

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

	void changeSpeed(float diff);
	void changeSpeed(float diff, duration_t dur);
	void setSpeed(float speed) { speed_.set(speed); }
	void setVelocity(sf::Vector2f velocity);
	void stop();
	void update();
	
	sf::Vector2f calculateReflection(sf::Vector2f point) const;
	/*returns a collision point if there is one*/
	float getBaseSpeed() const { return speed_.getBase(); }
	float getMinX() const { return getPosition().x - getRadius(); }
	float getMaxX() const { return getPosition().x + getRadius(); }
	float getMinY() const { return getPosition().y - getRadius(); }
	float getMaxY() const { return getPosition().y + getRadius(); }
	sf::Vector2f getPrevCenter() const { return prevCenter_; }
	float getSpeed() const { return speed_.getTotal(); }
	sf::Vector2f getVelocity() const { return velocity_; }
	/*
	returns:
	true if Ball is inside of p or intersecting it
	false if Ball is outside of p
	*/
	SFMLGAME_API bool isIntersecting(const sf::RectangleShape& rs) const;

private:

	Attribute<float> areaOfEffect_;
	Attribute<float> speed_;
	sf::Vector2f velocity_;
	sf::Vector2f prevCenter_;
};