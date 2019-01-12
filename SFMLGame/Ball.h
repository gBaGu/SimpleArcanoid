#pragma once
#include <memory>

#include <SFML\Graphics.hpp>

#include "Attribute.h"
#include "MyTime.h"
#include "Object.h"

#ifdef SFMLGAME_EXPORTS  
#define SFMLGAME_API __declspec(dllexport)   
#else  
#define SFMLGAME_API __declspec(dllimport)   
#endif


class SizeController
{
	struct Change
	{
		sf::Vector2f oldScale;
		Timer timer;
	};

public:
	SizeController(sf::CircleShape& shape);

	void increase(sf::Vector2f scaleFactor);
	void removeExpired();

private:
	sf::CircleShape& shape_;
	std::unique_ptr<Change> change_;
};


class Ball : public CircleObject
{
public:
	static const float DEFAULT_RADIUS;
	static const float DEFAULT_SPEED;
	static const sf::Vector2f DEFAULT_VELOCITY;

	Ball(float x, float y);

	sf::Vector2f calculateReflection(sf::Vector2f point) const;
	auto getAreaOfHit() const { return areaOfHit_.getTotal(); }

	void changeAreaOfHit(int diff, size_t seconds);
	void grow();
	virtual void update();

private:
	Attribute<int> areaOfHit_;
	SizeController sizeCtrl_;
};