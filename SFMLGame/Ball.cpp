#include "Ball.h"

#include <algorithm>
#include <iterator>
#include <numeric>
#include <set>

#include "MyMath.h"
#include "Object.h"
#include "Setting.h"


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

void Ball::changeSpeed(float diff)
{
	speed_.change(diff);
}

void Ball::changeSpeed(float diff, duration_t dur)
{
	speed_.change(diff, dur);
}

void Ball::hitAffected(std::vector<std::shared_ptr<Brick>>& bricks)
{
	if (activeCollisions_.empty())
	{
		return;
	}
	std::vector<std::shared_ptr<Brick>> affected;
	std::set<std::shared_ptr<Brick>> tmpBricks(std::begin(bricks), std::end(bricks));
	for (const auto& collision : activeCollisions_)
	{
		std::vector<decltype(tmpBricks)::iterator> toErase;
		for (auto it = std::begin(tmpBricks); it != std::end(tmpBricks); it++)
		{
			if (*it == collision->obj.lock() || inAOE(*it, collision->point))
			{
				affected.emplace_back(std::move(*it));
				toErase.push_back(it);
			}
		}
		std::for_each(std::begin(toErase), std::end(toErase),
			[&tmpBricks](auto it) { tmpBricks.erase(it); });
	}

	std::for_each(std::begin(affected), std::end(affected),
		[](auto& brick) { brick->takeDamage(); });
	
	sf::Vector2f newVelocity = std::accumulate(std::begin(activeCollisions_), std::end(activeCollisions_),
		sf::Vector2f(0.0f, 0.0f),
		[](sf::Vector2f v, collision_ptr collision) { return v + collision->velocityAfter; });
	setVelocity(newVelocity);
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
	prevCenter_ = getPosition();
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

void Ball::updateCollision(std::shared_ptr<RectangleObject> obj)
{
	collision_ptr oldCollision = findCollision(obj);
	collision_ptr collision = nullptr;
	auto center = getPosition();
	auto radius = getRadius();
	auto rectSides = obj->getSides();
	std::vector<sf::Vector2f> sideIntersections;
	for (const auto& rectSide : rectSides)
	{
		auto intersection = getIntersectionPoint(Segment<float>(prevCenter_, center),
			Segment<float>(rectSide.A, rectSide.B));
		if (intersection.first)
		{
			sideIntersections.push_back(intersection.second);
		}
		auto point = closestPoint(center, rectSide);
		if (length(point - center) < radius)
		{
			collision = oldCollision;
			if (!collision)
			{
				auto velocityAfter = calculateReflection(point);
				collision = std::make_shared<Collision>(point, velocityAfter, obj);
			}
		}
	}
	if (!sideIntersections.empty())
	{
		collision = oldCollision;
		if (!collision)
		{
			auto closestIntersection = std::min_element(std::begin(sideIntersections),
				std::end(sideIntersections),
				[this](const auto& l, const auto& r)
			{
				return length(l - prevCenter_) < length(r - prevCenter_);
			});
			auto velocityAfter = calculateReflection(*closestIntersection);
			collision = std::make_shared<Collision>(*closestIntersection, velocityAfter, obj);
		}
	}

	if (!oldCollision && collision)
	{
		activeCollisions_.push_back(collision);
	}
	else if (oldCollision && !collision)
	{
		auto toRemove = std::remove(std::begin(activeCollisions_), std::end(activeCollisions_),
			oldCollision);
		activeCollisions_.erase(toRemove, std::end(activeCollisions_));
	}
}

sf::Vector2f Ball::calculateReflection(sf::Vector2f point) const
{
	auto center = getPosition();
	auto ballVelocity = getVelocity();
	sf::Vector2f n = (point == center) ?
		point - prevCenter_ :
		point - center;
	return ballVelocity - n * (2 * scalarMultiplication(ballVelocity, n) / scalarMultiplication(n, n));
}

void Ball::clearBrokenBrickCollisions(const std::vector<std::shared_ptr<Brick>>& bricks)
{
	auto toRemove = std::remove_if(std::begin(activeCollisions_), std::end(activeCollisions_),
		[&bricks](const auto& collision)
	{
		return collision->obj.expired();
		/*return std::all_of(std::begin(bricks), std::end(bricks),
			[&collision](const auto& brick)
		{
			return collision->obj != brick;
		});*/
	});
	activeCollisions_.erase(toRemove, std::end(activeCollisions_));
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

Ball::collision_ptr Ball::findCollision(std::shared_ptr<RectangleObject> obj) const
{
	collision_ptr collision = nullptr;
	auto collisionIt = std::find_if(std::begin(activeCollisions_), std::end(activeCollisions_),
		[&obj](const auto& col)
	{
		return col->obj.lock() == obj;
	});
	if (collisionIt != std::end(activeCollisions_))
	{
		collision = *collisionIt;
	}
	return collision;
}