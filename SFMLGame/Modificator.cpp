#include "Modificator.h"


const float Modificator::DEFAULT_WIDTH = 30.0f;
const float Modificator::DEFAULT_HEIGHT = 10.0f;
const float Modificator::DEFAULT_SPEED = 7;
const sf::Vector2f Modificator::DEFAULT_VELOCITY = sf::Vector2f(0.0f, 1.0f);
const duration_t Modificator::DEFAULT_MODIFICATION_DURATION = duration_t(5);


Modificator::Modificator(float x, float y)
	: RectangleShape(sf::Vector2f(DEFAULT_WIDTH, DEFAULT_HEIGHT)),
	speed_(DEFAULT_SPEED),
	velocity_(DEFAULT_VELOCITY)
{
	setPosition(x, y);
	setFillColor(sf::Color::Green);
	setOrigin(getSize().x / 2, getSize().y / 2);
}

void Modificator::update()
{
	move(velocity_ * speed_);
}


const float BallSpeedPositiveModificator::DEFAULT_MULTIPLIER = 0.1f;

void BallSpeedPositiveModificator::trigger()
{
	auto speedDiff = -ball_->getBaseSpeed() * DEFAULT_MULTIPLIER;
	ball_->changeSpeed(speedDiff, DEFAULT_MODIFICATION_DURATION);
}