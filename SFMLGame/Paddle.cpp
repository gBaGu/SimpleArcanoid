#include "Paddle.h"

#include <algorithm>

#include <SFML\Graphics.hpp>

#include "Setting.h"


const float Paddle::DEFAULT_WIDTH = 60.0f;
const float Paddle::DEFAULT_HEIGHT = 20.0f;
const float Paddle::DEFAULT_SPEED = 6.5f;


Paddle::Paddle(float x, float y)
	: RectangleObject(sf::Vector2f(0.0f, 0.0f), DEFAULT_SPEED,
		sf::Vector2f(x, y), sf::Vector2f(DEFAULT_WIDTH, DEFAULT_HEIGHT))
{
	setFillColor(sf::Color::Red);
}

void Paddle::update()
{
	using sf::Keyboard;
	auto points = getPoints();
	sf::Vector2f newVelocity(0.0f, 0.0f);
	if (Keyboard::isKeyPressed(Keyboard::Key::Left) &&
		!Keyboard::isKeyPressed(Keyboard::Key::Right) &&
		std::all_of(std::begin(points), std::end(points),
			[](const auto& point) { return point.x > 0; }))
	{
		newVelocity.x = -1.0f;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Key::Right) &&
		!Keyboard::isKeyPressed(Keyboard::Key::Left) &&
		std::all_of(std::begin(points), std::end(points),
			[](const auto& point) { return point.x < WINDOW_WIDTH; }))
	{
		newVelocity.x = 1.0f;
	}
	setVelocity(newVelocity);
	Object::update();
}