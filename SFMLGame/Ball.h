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

	struct Collision //TODO: move to Game
	{
		sf::Vector2f point; //need to calculate velocity and AOE
		sf::Vector2f velocityAfter;
		std::weak_ptr<RectangleObject> obj; //need to find if collision is already in list

		Collision(sf::Vector2f p, sf::Vector2f v, std::shared_ptr<RectangleObject> o)
			: point(p), velocityAfter(v), obj(o) {}
	};

	using collision_ptr = std::shared_ptr<Collision>; //TODO: move to Game

	SFMLGAME_API Ball(float x, float y);
	SFMLGAME_API ~Ball();

	void changeSpeed(float diff);
	void changeSpeed(float diff, duration_t dur);
	void hitAffected(std::vector<std::shared_ptr<Brick>>& bricks); //TODO: move to Game
	void setSpeed(float speed) { speed_.set(speed); }
	void setVelocity(sf::Vector2f velocity);
	void stop();
	void update();
	void updateCollision(std::shared_ptr<RectangleObject> obj); //TODO: move to Game
	
	sf::Vector2f calculateReflection(sf::Vector2f point) const; //TODO: move to Game
	void clearBrokenBrickCollisions(const std::vector<std::shared_ptr<Brick>>& bricks);
	/*returns a collision point if there is one*/
	float getBaseSpeed() const { return speed_.getBase(); }
	float getMinX() const { return getPosition().x - getRadius(); }
	float getMaxX() const { return getPosition().x + getRadius(); }
	float getMinY() const { return getPosition().y - getRadius(); }
	float getMaxY() const { return getPosition().y + getRadius(); }
	float getSpeed() const { return speed_.getTotal(); }
	sf::Vector2f getVelocity() const { return velocity_; }
	bool inAOE(std::shared_ptr<Brick> brick, sf::Vector2f point) const; //TODO: move to Game
	/*
	returns:
	true if Ball is inside of p or intersecting it
	false if Ball is outside of p
	*/
	SFMLGAME_API bool isIntersecting(const sf::RectangleShape& rs) const; //TODO: move to Game

private:
	collision_ptr findCollision(std::shared_ptr<RectangleObject> obj) const; //TODO: move to Game

	Attribute<float> areaOfEffect_;
	Attribute<float> speed_;
	sf::Vector2f velocity_;
	sf::Vector2f prevCenter_;
	std::vector<collision_ptr> activeCollisions_; //TODO: move to Game
};