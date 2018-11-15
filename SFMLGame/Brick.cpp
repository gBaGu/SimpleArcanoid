#include "Brick.h"


const float Brick::DEFAULT_WIDTH = 60.0f;
const float Brick::DEFAULT_HEIGHT = 20.0f;


Brick::Brick(float x, float y)
	: RectangleShape(sf::Vector2f(DEFAULT_WIDTH, DEFAULT_HEIGHT)),
	durability_(1)
{
	setPosition(x, y);
	setFillColor(sf::Color::Yellow);
	setOrigin(getSize().x / 2, getSize().y / 2);
}

Brick::~Brick()
{
	if (onDestroy_)
	{
		onDestroy_(getPosition().x, getPosition().y);
	}
}

void Brick::takeHit(int damage)
{
	durability_ -= damage;
}

void Brick::takeMassiveHit()
{
	durability_ = 0;
}