#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

#include "Attribute.h"


class Object
{
public:
	Object(sf::Vector2f velocity, float speed);
	virtual ~Object() {}

	void draw(sf::RenderWindow& window) const;
	virtual void update() = 0;

	float getBaseSpeed() const { return speed_.getBase(); }
	sf::Vector2f getCenter() const { return shape_->getPosition(); }
	float getSpeed() const { return speed_.getTotal(); }
	sf::Vector2f getVelocity() const { return velocity_; }

	void changeSpeed(float diff);
	void changeSpeed(float diff, size_t seconds);
	void setSpeed(float speed);
	void setVelocity(sf::Vector2f velocity);

protected:
	//normalize velocity vector
	//normalize it after every change to make computing
	//based on it simple
	void normalizeVelocity();

	//set shape_ pointer to point to child shape
	void setShape(sf::Shape* shape) { shape_ = shape; }

private:
	//this is pointer to sf::Shape defined in child classes
	//child should set it manually
	//this is needed to make draw method not virtual
	sf::Shape* shape_;
	Attribute<float> speed_;
	sf::Vector2f velocity_;
};


class CircleObject : public Object
{
public:
	CircleObject(sf::Vector2f velocity, float speed, sf::Vector2f center, float radius);

	float getRadius() const { return circle_.getRadius(); }

	void setRadius(float radius);

private:
	sf::CircleShape circle_;
};


class RectangleObject : public Object
{
public:
	RectangleObject(sf::Vector2f velocity, float speed, sf::Vector2f center, sf::Vector2f size);

private:
	sf::RectangleShape rectangle_;
};