#pragma once
#include <memory>
#include <vector>

#include <SFML\Graphics.hpp>

#include "Attribute.h"
#include "Brick.h"
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

	struct Collision
	{
		sf::Vector2f point_;
		std::shared_ptr<Brick> brick_;

		Collision(sf::Vector2f point, std::shared_ptr<Brick> brick)
			: point_(point), brick_(brick) {}
	};

	SFMLGAME_API Ball(float x, float y);
	SFMLGAME_API ~Ball();

	/*
	checks if there is a collision between Ball and RectangleShape
	if there is one, modifies the velocity of Ball and returns true
	else returns false
	*/
	bool checkCollision(const sf::RectangleShape& rs);
	void changeSpeed(float diff);
	void changeSpeed(float diff, duration_t dur);
	void hitAffected(const std::vector<std::shared_ptr<Ball::Collision>>& collisions,
		std::vector<std::shared_ptr<Brick>>& bricks);
	void setRadius(float r, bool updateOrigin);
	void setSpeed(float speed) { speed_.set(speed); }
	void setVelocity(sf::Vector2f velocity);
	void stop();
	void update();
	
	/*returns a collision point if there is one*/
	std::shared_ptr<Collision> getCollisionPoint(std::shared_ptr<Brick> brick) const;
	float getBaseSpeed() const { return speed_.getBase(); }
	float getMinX() const { return getPosition().x - getRadius(); }
	float getMaxX() const { return getPosition().x + getRadius(); }
	float getMinY() const { return getPosition().y - getRadius(); }
	float getMaxY() const { return getPosition().y + getRadius(); }
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
};