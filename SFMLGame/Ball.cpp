#include "Ball.h"

#include <algorithm>
#include <iterator>

#include "MyMath.h"
#include "Setting.h"


const float Ball::DEFAULT_RADIUS = 10.0f;
const float Ball::DEFAULT_SPEED = 5;
const sf::Vector2f Ball::DEFAULT_VELOCITY = sf::Vector2f(-1.0f, -1.0f);

Ball::Ball(float x, float y)
	: CircleObject(sf::Vector2f(0.0f, 0.0f), DEFAULT_SPEED,
		sf::Vector2f(x, y), DEFAULT_RADIUS)
{
	setFillColor(sf::Color::Red);
}

sf::Vector2f Ball::calculateReflection(sf::Vector2f point) const
{
	auto center = getPosition();
	auto ballVelocity = getVelocity();
	sf::Vector2f n = (point == center) ?
		point - getPrevCenter() :
		point - center;
	if (n == sf::Vector2f(0.0f, 0.0f))
	{
		return sf::Vector2f(rand() % 100, rand() % 100);
	}
	return ballVelocity - n * (2 * scalarMultiplication(ballVelocity, n) / scalarMultiplication(n, n));
}