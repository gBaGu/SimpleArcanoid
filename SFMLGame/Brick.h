#pragma once
#include "Object.h"
#include "Destroyable.h"


class Brick : public RectangleObject, public Destroyable
{
public:
	static const float DEFAULT_WIDTH;
	static const float DEFAULT_HEIGHT;

	Brick(float x, float y);
	~Brick();
};