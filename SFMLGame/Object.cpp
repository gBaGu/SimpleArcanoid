#include "Object.h"

#include <iostream>

#include "MyMath.h"


Object::Object(sf::Vector2f velocity, float speed)
	: velocity_(velocity), speed_(speed)
{
	normalizeVelocity();
}

Object::~Object()
{

}

void Object::draw(sf::RenderWindow& window) const
{
	window.draw(*shape_);
}

void Object::update()
{
	shape_->move(velocity_ * speed_.getTotal());
	speed_.removeExpired();
}

void Object::changeSpeed(float diff)
{
	speed_.change(diff);
}

void Object::changeSpeed(float diff, size_t seconds)
{
	speed_.change(diff, duration_t(seconds));
}

void Object::setPosition(float x, float y)
{
	shape_->setPosition(x, y);
}

void Object::setPosition(sf::Vector2f position)
{
	shape_->setPosition(position);
}

void Object::setSpeed(float speed)
{
	speed_.set(speed);
}

void Object::setVelocity(sf::Vector2f velocity)
{
	velocity_ = velocity;
	normalizeVelocity();
}

void Object::normalizeVelocity()
{
	velocity_ = normalize(velocity_);
}


CircleObject::CircleObject(sf::Vector2f velocity, float speed, sf::Vector2f center, float radius)
	: Object(velocity, speed), circle_(radius)
{
	setShape(&circle_);
	circle_.setPosition(center);
	circle_.setOrigin(radius, radius);
	circle_.setFillColor(sf::Color::Red);
}

CircleObject::CircleObject(const CircleObject& other)
	: Object(other.getVelocity(), other.getBaseSpeed()), circle_(other.circle_)
{
	setShape(&circle_);
}

void CircleObject::setRadius(float radius)
{
	circle_.setRadius(radius);
	circle_.setOrigin(radius, radius);
}


RectangleObject::RectangleObject(sf::Vector2f velocity, float speed, sf::Vector2f center, sf::Vector2f size)
	: Object(velocity, speed), rectangle_(size)
{
	setShape(&rectangle_);
	rectangle_.setPosition(center);
	rectangle_.setOrigin(size.x / 2, size.y / 2);
	rectangle_.setFillColor(sf::Color::Yellow);
}

RectangleObject::RectangleObject(const RectangleObject& other)
	: Object(other.getVelocity(), other.getBaseSpeed()), rectangle_(other.rectangle_)
{
	setShape(&rectangle_);
}

std::vector<Segment<float>> RectangleObject::getSides() const
{
	std::vector<sf::Vector2f> points;
	for (int i = 0; i < rectangle_.getPointCount(); i++)
	{
		auto point = rectangle_.getPoint(i);
		auto transform = rectangle_.getTransform();
		points.push_back(transform.transformPoint(point));
	}
	if (points.empty())
	{
		throw std::runtime_error("sf::RectangleShape has no points.");
	}

	std::vector<Segment<float>> sides;
	for (auto it = points.begin(); it != std::prev(points.end()); it++)
	{
		sides.emplace_back(*it, *(it + 1));
	}
	sides.emplace_back(points.back(), points.front());
	return sides;
}