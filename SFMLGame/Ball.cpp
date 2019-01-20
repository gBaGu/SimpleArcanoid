#include "Ball.h"

#include "MyMath.h"


SizeController::SizeController(sf::CircleShape& shape)
	: shape_(shape)
{

}

void SizeController::increase(sf::Vector2f scaleFactor)
{
	if (change_)
	{
		change_->timer.extend();
	}
	else
	{
		Change change{ shape_.getScale(), Timer(duration_t(5)) };
		shape_.scale(scaleFactor);
		change_ = std::make_unique<Change>(change);
	}
}

void SizeController::removeExpired()
{
	if (change_ && change_->timer.isExpired())
	{
		shape_.setScale(change_->oldScale);
		change_.reset();
	}
}


const float Ball::DEFAULT_RADIUS = 10.0f;
const float Ball::DEFAULT_SPEED = 5;
const sf::Vector2f Ball::DEFAULT_VELOCITY = sf::Vector2f(-1.0f, -1.0f);

Ball::Ball(float x, float y)
	: CircleObject(sf::Vector2f(0.0f, 0.0f), DEFAULT_SPEED,
		sf::Vector2f(x, y), DEFAULT_RADIUS),
		areaOfHit_(0, 0, 50),
		sizeCtrl_(getCircle())
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

void Ball::changeAreaOfHit(int diff, size_t seconds)
{
	areaOfHit_.change(diff, duration_t(seconds));
}

void Ball::grow()
{
	sizeCtrl_.increase({ 2.0f, 2.0f });
}

void Ball::update()
{
	Object::update();
	areaOfHit_.removeExpired();
	sizeCtrl_.removeExpired();
}