#pragma once
#include <memory>
#include <vector>

#include <SFML\Graphics.hpp>

#include "Attribute.h"
#include "MyTime.h"
#include "Object.h"

#ifdef SFMLGAME_EXPORTS  
#define SFMLGAME_API __declspec(dllexport)   
#else  
#define SFMLGAME_API __declspec(dllimport)   
#endif


class Ball : public CircleObject
{
public:
	static const float DEFAULT_RADIUS;
	static const float DEFAULT_SPEED;
	static const sf::Vector2f DEFAULT_VELOCITY;

	Ball(float x, float y);

	sf::Vector2f calculateReflection(sf::Vector2f point) const;
};