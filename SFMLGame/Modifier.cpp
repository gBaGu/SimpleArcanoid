#include "Modifier.h"


const float Modifier::DEFAULT_WIDTH = 30.0f;
const float Modifier::DEFAULT_HEIGHT = 10.0f;
const float Modifier::DEFAULT_SPEED = 7;
const sf::Vector2f Modifier::DEFAULT_VELOCITY = sf::Vector2f(0.0f, 1.0f);
const size_t Modifier::DEFAULT_MODIFICATION_DURATION = 5; //seconds


Modifier::Modifier(float x, float y)
	: RectangleObject(DEFAULT_VELOCITY, DEFAULT_SPEED, { x, y }, sf::Vector2f(DEFAULT_WIDTH, DEFAULT_HEIGHT))
{
	setFillColor(sf::Color::Green);
}


const float ObjectSpeedPositiveModifier::DEFAULT_MULTIPLIER = 0.1f;

void ObjectSpeedPositiveModifier::trigger()
{
	auto speedDiff = -obj_->getBaseSpeed() * DEFAULT_MULTIPLIER;
	obj_->changeSpeed(speedDiff, DEFAULT_MODIFICATION_DURATION);
}