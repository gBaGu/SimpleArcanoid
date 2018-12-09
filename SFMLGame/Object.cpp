#include "Object.h"

#include "MyMath.h"


void Object::draw(sf::RenderWindow& window) const
{
	window.draw(*shape_);
}

void Object::normalizeVelocity()
{
	velocity_ = normalize(velocity_);
}


CircleObject::CircleObject(sf::Vector2f velocity, float speed, sf::Vector2f center, float radius)
	: Object(velocity, speed), circle_(radius)
{
	setShape(&circle_);
	normalizeVelocity();
	circle_.setPosition(center);
	circle_.setOrigin(radius, radius);
	circle_.setFillColor(sf::Color::Red);
}


RectangleObject::RectangleObject(sf::Vector2f velocity, float speed, sf::Vector2f center, sf::Vector2f size)
	: Object(velocity, speed), rectangle_(size)
{
	setShape(&rectangle_);
	rectangle_.setPosition(center);
	rectangle_.setOrigin(size.x / 2, size.y / 2);
	rectangle_.setFillColor(sf::Color::Yellow);
}