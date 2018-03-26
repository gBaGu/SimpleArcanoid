#include "Util.h"


void testCollision(const Paddle& p, Ball& b)
{
	if (!isIntersectingSimple(p, b))
		return;

	auto ballVelocity = b.getVelocity();
	ballVelocity.y = -ballVelocity.y;

	if (b.getPosition().x < p.getPosition().x)
		ballVelocity.x = -Ball::DEFAULT_VELOCITY;
	else if (b.getPosition().x > p.getPosition().x)
		ballVelocity.x = Ball::DEFAULT_VELOCITY;

	b.setVelocity(ballVelocity);
}