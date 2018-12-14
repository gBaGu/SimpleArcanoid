#pragma once
#include <vector>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

#include "Attribute.h"
#include "MyMath.h"


class Object
{
public:
	Object(sf::Vector2f velocity, float speed);
	virtual ~Object() = 0;

	void draw(sf::RenderWindow& window) const;
	virtual void update();

	float getBaseSpeed() const { return speed_.getBase(); }
	sf::Vector2f getPosition() const { return shape_->getPosition(); }
	float getSpeed() const { return speed_.getTotal(); }
	sf::Vector2f getVelocity() const { return velocity_; }

	void changeSpeed(float diff);
	void changeSpeed(float diff, size_t seconds);
	void setFillColor(sf::Color color);
	void setPosition(float x, float y);
	void setPosition(sf::Vector2f position);
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
	CircleObject(const CircleObject& other);

	float getRadius() const { return circle_.getRadius(); }

	void setRadius(float radius);

private:
	sf::CircleShape circle_;
};


class RectangleObject : public Object
{
public:
	RectangleObject(sf::Vector2f velocity, float speed, sf::Vector2f center, sf::Vector2f size);
	RectangleObject(const RectangleObject& other);

	auto getGlobalBounds() const { return rectangle_.getGlobalBounds(); }
	std::vector<sf::Vector2f> getPoints() const;
	std::vector<Segment<float>> getSides() const;

private:
	sf::RectangleShape rectangle_;
};