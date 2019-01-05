#pragma once
#include <SFML\Graphics.hpp>

#include "Object.h"


class Paddle : public RectangleObject
{
public:
	static const float DEFAULT_WIDTH;
	static const float DEFAULT_HEIGHT;
	static const float DEFAULT_SPEED;

	Paddle(float x, float y);
};