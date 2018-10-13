#pragma once
#include <SFML\Graphics.hpp>


class Modificator : public sf::RectangleShape
{
public:
	static const float DEFAULT_WIDTH;
	static const float DEFAULT_HEIGHT;
	static const float DEFAULT_SPEED;
	static const sf::Vector2f DEFAULT_VELOCITY;

	Modificator(float x, float y);

	void update();

private:
	float speed_;
	sf::Vector2f velocity_;
};