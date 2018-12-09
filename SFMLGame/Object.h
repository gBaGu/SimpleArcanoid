#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

#include "Attribute.h"


class Object
{
public:
	Object(sf::Vector2f velocity, float speed)
		: velocity_(velocity), speed_(speed) {}
	virtual ~Object() {}

	void draw(sf::RenderWindow& window) const;
	virtual void update() = 0;

protected:
	//normalize velocity vector
	//normalize it after every change to make computing
	//based on it simple
	void normalizeVelocity();

	//set shape_ pointer to point to child shape
	void setShape(const sf::Shape const* shape) { shape_ = shape; }

private:
	//this is pointer to sf::Shape defined in child classes
	//child should set it manually
	//this is needed to make draw method not virtual
	const sf::Shape const* shape_;
	Attribute<float> speed_;
	sf::Vector2f velocity_;
};


class CircleObject : public Object
{
public:
	CircleObject(sf::Vector2f velocity, float speed, sf::Vector2f center, float radius);

	sf::Vector2f getCenter() const { return circle_.getPosition(); }
	float getRadius() const { return circle_.getRadius(); }

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