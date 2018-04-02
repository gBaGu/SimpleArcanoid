#include "Paddle.h"

#include "Setting.h"


const float Paddle::DEFAULT_WIDTH = 60.0f;
const float Paddle::DEFAULT_HEIGHT = 20.0f;
const float Paddle::DEFAULT_VELOCITY = 6.0f;


Paddle::Paddle(float x, float y)
	: RectangleShape(sf::Vector2f(DEFAULT_WIDTH, DEFAULT_HEIGHT))
{
	setPosition(x, y);
	setFillColor(sf::Color::Red);
	setOrigin(getSize().x / 2, getSize().y / 2);
}

void Paddle::update()
{
	move(velocity);

	using sf::Keyboard;
	if (Keyboard::isKeyPressed(Keyboard::Key::Left) &&
		!Keyboard::isKeyPressed(Keyboard::Key::Right) &&
		getMinX() > 0)
	{
		velocity.x = -DEFAULT_VELOCITY;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Key::Right) &&
		!Keyboard::isKeyPressed(Keyboard::Key::Left) &&
		getMaxX() < WINDOW_WIDTH)
	{
		velocity.x = DEFAULT_VELOCITY;
	}
	else
	{
		velocity.x = 0;
	}
}