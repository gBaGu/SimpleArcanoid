#include "Ball.h"

#include <algorithm>
#include <iterator>

#include "Setting.h"
#include "MyMath.h"


const float Ball::DEFAULT_RADIUS = 10.0f;
const float Ball::DEFAULT_VELOCITY = 8.0f;

Ball::Ball(float x, float y)
	: CircleShape(DEFAULT_RADIUS), velocity(-DEFAULT_VELOCITY, -DEFAULT_VELOCITY)
{
	setPosition(x, y);
	setFillColor(sf::Color::Red);
	setOrigin(DEFAULT_RADIUS, DEFAULT_RADIUS);
}

void Ball::update()
{
	move(velocity);

	if (getMinX() < 0)
		velocity.x = DEFAULT_VELOCITY;
	else if (getMaxX() > windowWidth)
		velocity.x = -DEFAULT_VELOCITY;

	if (getMinY() < 0)
		velocity.y = DEFAULT_VELOCITY;
	else if (getMaxY() > windowHeight)
		velocity.y = -DEFAULT_VELOCITY;
}

void Ball::checkCollision(const Paddle& p)
{
	if (!isIntersecting(p))
		return;

	auto ballVelocity = getVelocity();
	ballVelocity.y = -ballVelocity.y;

	if (getPosition().x < p.getPosition().x)
		ballVelocity.x = -Ball::DEFAULT_VELOCITY;
	else if (getPosition().x > p.getPosition().x)
		ballVelocity.x = Ball::DEFAULT_VELOCITY;

	setVelocity(ballVelocity);
}

bool Ball::isIntersecting(const Paddle& p) const
{
	std::vector<sf::Vector2f> paddlePoints;
	for (int i = 0; i < p.getPointCount(); i++)
	{
		auto point = p.getPoint(i);
		auto transform = p.getTransform();
		paddlePoints.push_back(transform.transformPoint(point));
	}
	if (paddlePoints.empty())
		return false;

	std::vector<Segment<float>> paddleSides;
	for (auto it = paddlePoints.begin(); it != paddlePoints.end() - 1 && it != paddlePoints.end(); it++)
	{
		paddleSides.emplace_back(*it, *(it + 1));
	}
	paddleSides.emplace_back(paddlePoints.back(), paddlePoints.front());

	auto center = getPosition();
	auto radius = getRadius();
	return std::any_of(paddleSides.begin(), paddleSides.end(),
		[center, radius](Segment<float> s) { return distance(center, s) < radius; });
}