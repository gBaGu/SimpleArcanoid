#include "Ball.h"

#include <algorithm>
#include <iterator>

#include "Setting.h"
#include "MyMath.h"


const float Ball::DEFAULT_RADIUS = 10.0f;
const float Ball::DEFAULT_SPEED = 5;
const sf::Vector2f Ball::DEFAULT_VELOCITY = sf::Vector2f(-1.0f, -1.0f);

Ball::Ball(float x, float y)
	: CircleShape(DEFAULT_RADIUS),
	speed_(DEFAULT_SPEED),
	velocity_(DEFAULT_VELOCITY)
{
	velocity_ = normalize(velocity_);
	setPosition(x, y);
	setFillColor(sf::Color::Red);
	setOrigin(DEFAULT_RADIUS, DEFAULT_RADIUS);
}

Ball::~Ball()
{

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
		ballVelocity.x = std::min(ballVelocity.x, -ballVelocity.x);
	}
	else if (getPosition().x > rs.getPosition().x)
	{
		ballVelocity.x = std::max(ballVelocity.x, -ballVelocity.x);
	}

	setVelocity(ballVelocity);
	return true;
}

void Ball::changeSpeed(float diff)
{
	speed_.change(diff);
}

void Ball::changeSpeed(float diff, duration_t dur)
{
	speed_.change(diff, dur);
}

void Ball::setRadius(float r, bool updateOrigin)
{
	CircleShape::setRadius(getRadius() * 2);
	if (updateOrigin)
	{
		setOrigin(getRadius(), getRadius());
	}
}

void Ball::setVelocity(sf::Vector2f velocity)
{
	velocity_ = velocity;
	velocity_ = normalize(velocity_);
}

void Ball::stop()
{
	speed_.set(0.0);
}

void Ball::update()
{
	move(velocity_ * speed_.getTotal());

	//Check for window border collision
	if (getMinX() < 0)
	{
		velocity_.x = std::max(velocity_.x, -velocity_.x);
	}
	else if (getMaxX() > WINDOW_WIDTH)
	{
		velocity_.x = std::min(velocity_.x, -velocity_.x);
	}
	if (getMinY() < 0)
	{
		velocity_.y = std::max(velocity_.y, -velocity_.y);
	}

	speed_.removeExpired();
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