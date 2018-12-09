#include "Object.h"

#include "MyMath.h"


Object::Object(sf::Vector2f velocity, float speed)
	: velocity_(velocity), speed_(speed)
{
	normalizeVelocity();
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