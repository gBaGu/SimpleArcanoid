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