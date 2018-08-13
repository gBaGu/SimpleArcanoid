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

bool Ball::checkCollision(const sf::RectangleShape& rs)
{
	if (!isIntersecting(rs))
	{
		return false;
	}

	//TODO: recalculate velocity vector
	auto ballVelocity = getVelocity();
	ballVelocity.y = -ballVelocity.y;

	if (getPosition().x < rs.getPosition().x)
	{
		ballVelocity.x = -Ball::DEFAULT_VELOCITY;
	}
	else if (getPosition().x > rs.getPosition().x)
	{
		ballVelocity.x = Ball::DEFAULT_VELOCITY;
	}

	setVelocity(ballVelocity);
	return true;
}

bool Ball::isIntersecting(const sf::RectangleShape& rs) const
{
	auto center = getPosition();
	auto radius = getRadius();
	if (isInsideByCrossingNumber(center, rs))
	{
		return true;
	}

	std::vector<sf::Vector2f> rectPoints;
	for (int i = 0; i < rs.getPointCount(); i++)
	{
		auto point = rs.getPoint(i);
		auto transform = rs.getTransform();
		rectPoints.push_back(transform.transformPoint(point));
	}
	if (rectPoints.empty())
	{
		return false;
	}

	std::vector<Segment<float>> rectSides;
	for (auto it = rectPoints.begin(); it != std::prev(rectPoints.end()); it++)
	{
		rectSides.emplace_back(*it, *(it + 1));
	}
	rectSides.emplace_back(rectPoints.back(), rectPoints.front());

	return std::any_of(rectSides.begin(), rectSides.end(),
		[center, radius](auto s) { return distance(center, s) < radius; });
}