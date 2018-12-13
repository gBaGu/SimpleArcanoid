#include "Brick.h"


const float Brick::DEFAULT_WIDTH = 60.0f;
const float Brick::DEFAULT_HEIGHT = 20.0f;


Brick::Brick(float x, float y)
	: RectangleObject(sf::Vector2f(0.0f, 0.0f), 0.0f,
		sf::Vector2f(x, y), sf::Vector2f(DEFAULT_WIDTH, DEFAULT_HEIGHT)),
	Destroyable(1)
{
}

Brick::~Brick()
{
	if (onDestroy_)
	{
		auto center = getPosition();
		onDestroy_(center.x, center.y);
	}
}