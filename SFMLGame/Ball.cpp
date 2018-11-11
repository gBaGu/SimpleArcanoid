#include "Ball.h"

#include <algorithm>
#include <iterator>
#include <set>

#include "Setting.h"
#include "MyMath.h"


const float Ball::DEFAULT_RADIUS = 10.0f;
const float Ball::DEFAULT_SPEED = 5;
const sf::Vector2f Ball::DEFAULT_VELOCITY = sf::Vector2f(-1.0f, -1.0f);

Ball::Ball(float x, float y)
	: CircleShape(DEFAULT_RADIUS),
	areaOfEffect_(0),
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
	//TODO: auto collisionPoints = getCollisions(bricks)
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
	//return collision points
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

void Ball::hitAffected(const std::vector<std::shared_ptr<Ball::Collision>>& collisions,
	std::vector<std::shared_ptr<Brick>>& bricks)
{
	std::vector<std::shared_ptr<Brick>> affected;
	std::set<std::shared_ptr<Brick>> tmpBricks(std::begin(bricks), std::end(bricks));
	for (const auto& collision : collisions)
	{
		//TODO: move bricks from tmp to affected
		//if brick from bricks == collision.brick
		//or if brick from bricks is in AOE
	}

	//TODO:: do damage to affected and recalculate velocity after hit
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

std::shared_ptr<Ball::Collision> Ball::getCollisionPoint(std::shared_ptr<Brick> brick) const
{
	auto center = getPosition();
	auto radius = getRadius();
	if (isInsideByCrossingNumber(center, *brick))
	{
		return std::make_shared<Collision>(center, brick);
	}

	std::vector<sf::Vector2f> rectPoints;
	for (int i = 0; i < brick->getPointCount(); i++)
	{
		auto point = brick->getPoint(i);
		auto transform = brick->getTransform();
		rectPoints.push_back(transform.transformPoint(point));
	}
	if (rectPoints.empty())
	{
		return nullptr;
	}

	std::vector<Segment<float>> rectSides;
	for (auto it = rectPoints.begin(); it != std::prev(rectPoints.end()); it++)
	{
		rectSides.emplace_back(*it, *(it + 1));
	}
	rectSides.emplace_back(rectPoints.back(), rectPoints.front());
	for (const auto& rectSide : rectSides)
	{
		auto point = closestPoint(center, rectSide);
		if (length(point - center) < radius)
		{
			return std::make_shared<Collision>(point, brick);
		}
	}
	return nullptr;
}

bool Ball::isIntersecting(const sf::RectangleShape& rs) const
{
	auto center = getPosition();
	auto radius = getRadius();
	//TODO: maybe move to the end of function
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