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

void Ball::hitAffected(const std::vector<collision_ptr>& collisions,
	std::vector<std::shared_ptr<Brick>>& bricks)
{
	if (collisions.empty())
	{
		return;
	}
	std::vector<std::shared_ptr<Brick>> affected;
	std::set<std::shared_ptr<Brick>> tmpBricks(std::begin(bricks), std::end(bricks));
	for (const auto& collision : collisions)
	{
		std::vector<decltype(tmpBricks)::iterator> toErase;
		for (auto it = std::begin(tmpBricks); it != std::end(tmpBricks); it++)
		{
			if (*it == collision->brick || inAOE(*it, collision->point))
			{
				affected.emplace_back(std::move(*it));
				toErase.push_back(it);
			}
		}
		std::for_each(std::begin(toErase), std::end(toErase),
			[&tmpBricks](auto it) { tmpBricks.erase(it); });
	}

	std::for_each(std::begin(affected), std::end(affected),
		[](auto& brick) { brick->takeHit(); });
	
	auto newVelocity = calculateVelocityAfterCollision(collisions);
	setVelocity(newVelocity);
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

sf::Vector2f Ball::calculateVelocityAfterCollision(const std::vector<collision_ptr>& collisions) const
{
	auto center = getPosition();
	auto ballVelocity = getVelocity();
	sf::Vector2f newVelocity(0.0f, 0.0f);
	for (const auto& collision : collisions)
	{
		sf::Vector2f n = collision->point - center;
		auto v = ballVelocity - n * (2 * scalarMultiplication(ballVelocity, n) / scalarMultiplication(n, n));
		newVelocity += v;
	}
	return newVelocity;
}

Ball::collision_ptr Ball::getCollisionPoint(std::shared_ptr<Brick> brick) const
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

bool Ball::inAOE(std::shared_ptr<Brick> brick, sf::Vector2f point) const
{
	//TODO: implement
	//return call_to_math_function(rect, point, radius);
	return false;
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