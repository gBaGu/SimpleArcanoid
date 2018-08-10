#include "Ball.h"

#include <algorithm>
#include <iterator>

#include "Setting.h"
#include "MyMath.h"


const float Ball::DEFAULT_RADIUS = 10.0f;
const float Ball::DEFAULT_VELOCITY = 8.0f;

Ball::Ball(float x, float y)
	: CircleShape(DEFAULT_RADIUS),
	velocity(-DEFAULT_VELOCITY, -DEFAULT_VELOCITY)
{
	setPosition(x, y);
	setFillColor(sf::Color::Red);
	setOrigin(DEFAULT_RADIUS, DEFAULT_RADIUS);
}

void Ball::stop()
{
	velocity = sf::Vector2f(0, 0);
}

void Ball::update()
{
	move(velocity);

	//Check for window border collision
	if (getMinX() < 0)
	{
		velocity.x = DEFAULT_VELOCITY;
	}
	else if (getMaxX() > WINDOW_WIDTH)
	{
		velocity.x = -DEFAULT_VELOCITY;
	}
	if (getMinY() < 0)
	{
		velocity.y = DEFAULT_VELOCITY;
	}
}

bool Ball::checkCollision(const sf::RectangleShape& p) //TODO: replace RectangleShape with Rect
{
	if (!isIntersecting(p))
	{
		return false;
	}

	//TODO: recalculate velocity vector
	auto ballVelocity = getVelocity();
	ballVelocity.y = -ballVelocity.y;

	if (getPosition().x < p.getPosition().x)
	{
		ballVelocity.x = -Ball::DEFAULT_VELOCITY;
	}
	else if (getPosition().x > p.getPosition().x)
	{
		ballVelocity.x = Ball::DEFAULT_VELOCITY;
	}

	setVelocity(ballVelocity);
	return true;
}

bool Ball::isIntersecting(const sf::RectangleShape& p) const
{
	auto center = getPosition();
	auto radius = getRadius();
	if (isInsideByCrossingNumber(center, p))
	{
		return true;
	}

	std::vector<sf::Vector2f> paddlePoints;
	for (int i = 0; i < p.getPointCount(); i++)
	{
		auto point = p.getPoint(i);
		auto transform = p.getTransform();
		paddlePoints.push_back(transform.transformPoint(point));
	}
	if (paddlePoints.empty())
	{
		return false;
	}

	std::vector<Segment<float>> paddleSides;
	for (auto it = paddlePoints.begin(); it != std::prev(paddlePoints.end()); it++)
	{
		paddleSides.emplace_back(*it, *(it + 1));
	}
	paddleSides.emplace_back(paddlePoints.back(), paddlePoints.front());

	return std::any_of(paddleSides.begin(), paddleSides.end(),
		[center, radius](Segment<float> s) { return distance(center, s) < radius; });
}