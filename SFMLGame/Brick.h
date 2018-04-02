#pragma once
#include <SFML\Graphics.hpp>


class Brick : public sf::RectangleShape
{
public:
	static const float DEFAULT_WIDTH;
	static const float DEFAULT_HEIGHT;

	Brick(float x, float y);

	float getMinX() const { return getPosition().x - getSize().x / 2; }
	float getMaxX() const { return getPosition().x + getSize().x / 2; }
	float getMinY() const { return getPosition().y - getSize().y / 2; }
	float getMaxY() const { return getPosition().y + getSize().y / 2; }

private:

};