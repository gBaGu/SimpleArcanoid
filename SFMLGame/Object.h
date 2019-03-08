#pragma once
#include <vector>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

#include "Attribute.h"
#include "MyMath.h"


class Object
{
public:
	Object(sf::Vector2f velocity, float speed);
	virtual ~Object() = 0;

	void draw(sf::RenderWindow& window) const;
	virtual void update();

	virtual float calculateDistance(sf::Vector2f point) const = 0;
	auto getBaseSpeed() const { return speed_.getBase(); }
	auto getPosition() const { return transformable_->getPosition(); }
	auto getPrevCenter() const { return prevCenter_; }
	auto getSpeed() const { return speed_.getTotal(); }
	auto getVelocity() const { return velocity_; }

	void changeSpeed(float diff);
	void changeSpeed(float diff, size_t seconds);
	void setPosition(float x, float y);
	void setPosition(sf::Vector2f position);
	void setSpeed(float speed);
	void setVelocity(sf::Vector2f velocity);

protected:
	//normalize velocity vector
	//normalize it after every change to make computing
	//based on it simple
	void normalizeVelocity();

	//set drawable_ and transformable_ pointers to point to child object
	void setDrawable(sf::Drawable* drawable) { drawable_ = drawable; }
	void setTransformable(sf::Transformable* transformable) { transformable_ = transformable; }

private:
	bool show_;
	//this is pointer to sf::Shape defined in child classes
	//child should set it manually
	//this is needed to make draw method not virtual
	sf::Drawable* drawable_;
	sf::Transformable* transformable_;
	Attribute<float> speed_;
	sf::Vector2f velocity_;
	sf::Vector2f prevCenter_;
};


class CircleObject : public Object
{
public:
	CircleObject(sf::Vector2f velocity, float speed, sf::Vector2f center, float radius);
	CircleObject(const CircleObject& other);

	virtual float calculateDistance(sf::Vector2f point) const;
	auto getGlobalBounds() const { return circle_.getGlobalBounds(); }
	auto getRadius() const { return circle_.getRadius() * circle_.getScale().x; } //suppose scale.x and scale.y are equal

	void setFillColor(sf::Color color);
	void setRadius(float radius);

protected:
	auto& getCircle() { return circle_; }
	const auto& getCircle() const { return circle_; }

private:
	sf::CircleShape circle_;
};


class RectangleObject : public Object
{
public:
	RectangleObject(sf::Vector2f velocity, float speed, sf::Vector2f center, sf::Vector2f size);
	RectangleObject(const RectangleObject& other);

	virtual float calculateDistance(sf::Vector2f point) const;
	auto getGlobalBounds() const { return rectangle_.getGlobalBounds(); }
	std::vector<sf::Vector2f> getPoints() const;
	std::vector<Segment<float>> getSides() const;

	void setFillColor(sf::Color color);

protected:
	auto& getRectangle() { return rectangle_; }
	const auto& getRectangle() const { return rectangle_; }

private:
	sf::RectangleShape rectangle_;
};


class TextObject : public Object
{
public:
	TextObject(sf::Vector2f velocity, float speed, sf::Vector2f center,
		unsigned int charSize, const std::string& text, const sf::Font& font);
	TextObject(const TextObject& other);

	virtual float calculateDistance(sf::Vector2f point) const;
	auto getGlobalBounds() const { return text_.getGlobalBounds(); }
	
	void setCharSize(unsigned int charSize);
	void setFillColor(sf::Color color);
	void setText(const std::string& text);

private:
	sf::Text text_;
};