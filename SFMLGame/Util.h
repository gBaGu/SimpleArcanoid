#pragma once
#include "Ball.h"
#include "Paddle.h"


void testCollision(const Paddle& p, Ball& b);

template<typename T1, typename T2>
bool isIntersectingSimple(T1 a, T2 b)
{
	return a.getMaxX() >= b.getMinX() && a.getMinX() <= b.getMaxX() &&
		a.getMaxY() >= b.getMinY() && a.getMinY() <= b.getMaxY();
}