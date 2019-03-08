#include "Object.h"

#include <algorithm>
#include <valarray>
#include <iterator>

#include "MyMath.h"


Object::Object(sf::Vector2f velocity, float speed)
	: show_(true), velocity_(velocity), speed_(speed)
{
	normalizeVelocity();
}

Object::~Object()
{

}

void Object::draw(sf::RenderWindow& window) const
{
	if (show_)
	{
		window.draw(*drawable_);
	}
}

void Object::update()
{
	prevCenter_ = transformable_->getPosition();
	transformable_->move(velocity_ * speed_.getTotal());
	speed_.removeExpired();
}

void Object::changeSpeed(float diff)
{
	speed_.change(diff);
}

void Object::changeSpeed(float diff, size_t seconds)
{
	speed_.change(diff, duration_t(seconds));
}

void Object::setPosition(float x, float y)
{
	transformable_->setPosition(x, y);
}

void Object::setPosition(sf::Vector2f position)
{
	transformable_->setPosition(position);
}

void Object::setSpeed(float speed)
{
	speed_.set(speed);
}

void Object::setVelocity(sf::Vector2f velocity)
{
	velocity_ = velocity;
	normalizeVelocity();
}

void Object::normalizeVelocity()
{
	if (velocity_ != sf::Vector2f(0.0f, 0.0f))
	{
		velocity_ = normalize(velocity_);
	}
}


CircleObject::CircleObject(sf::Vector2f velocity, float speed, sf::Vector2f center, float radius)
	: Object(velocity, speed), circle_(radius)
{
	setDrawable(&circle_);
	setTransformable(&circle_);
	circle_.setPosition(center);
	circle_.setOrigin(radius, radius);
}

CircleObject::CircleObject(const CircleObject& other)
	: Object(other.getVelocity(), other.getBaseSpeed()), circle_(other.circle_)
{
	setDrawable(&circle_);
	setTransformable(&circle_);
}

float CircleObject::calculateDistance(sf::Vector2f point) const
{
	auto vec = point - getPosition();
	auto distCenterToPoint = length(vec);
	return distCenterToPoint > getRadius() ?
		distCenterToPoint :
		0.0f;
}

void CircleObject::setFillColor(sf::Color color)
{
	circle_.setFillColor(color);
}

void CircleObject::setRadius(float radius)
{
	circle_.setRadius(radius);
	circle_.setOrigin(radius, radius);
}


RectangleObject::RectangleObject(sf::Vector2f velocity, float speed, sf::Vector2f center, sf::Vector2f size)
	: Object(velocity, speed), rectangle_(size)
{
	setDrawable(&rectangle_);
	setTransformable(&rectangle_);
	rectangle_.setPosition(center);
	rectangle_.setOrigin(size.x / 2, size.y / 2);
}

RectangleObject::RectangleObject(const RectangleObject& other)
	: Object(other.getVelocity(), other.getBaseSpeed()), rectangle_(other.rectangle_)
{
	setDrawable(&rectangle_);
	setTransformable(&rectangle_);
}

float RectangleObject::calculateDistance(sf::Vector2f point) const
{
	auto sides = getSides();
	std::valarray<float> distances(sides.size());
	std::transform(std::begin(sides), std::end(sides),
		std::begin(distances),
		[&point](const auto& side) { return distance(point, side); });
	return distances.sum() > (rectangle_.getSize().x + rectangle_.getSize().y) ?
		distances.min() :
		0.0f;
}

std::vector<sf::Vector2f> RectangleObject::getPoints() const
{
	std::vector<sf::Vector2f> points;
	for (int i = 0; i < rectangle_.getPointCount(); i++)
	{
		auto point = rectangle_.getPoint(i);
		auto transform = rectangle_.getTransform();
		points.push_back(transform.transformPoint(point));
	}
	return points;
}

std::vector<Segment<float>> RectangleObject::getSides() const
{
	auto points = getPoints();
	if (points.empty())
	{
		throw std::runtime_error("sf::RectangleShape has no points.");
	}

	std::vector<Segment<float>> sides;
	for (auto it = points.begin(); it != std::prev(points.end()); it++)
	{
		sides.emplace_back(*it, *(it + 1));
	}
	sides.emplace_back(points.back(), points.front());
	return sides;
}

void RectangleObject::setFillColor(sf::Color color)
{
	rectangle_.setFillColor(color);
}


TextObject::TextObject(sf::Vector2f velocity, float speed, sf::Vector2f center,
	unsigned int charSize, const std::string& text, const sf::Font& font)
	: Object(velocity, speed), text_(text, font, charSize)
{
	setDrawable(&text_);
	setTransformable(&text_);
	text_.setPosition(center);
	auto textRect = text_.getLocalBounds();
	text_.setOrigin(textRect.width / 2, textRect.height / 2);
}

TextObject::TextObject(const TextObject& other)
	: Object(other.getVelocity(), other.getBaseSpeed()), text_(other.text_)
{
	setDrawable(&text_);
	setTransformable(&text_);
}

float TextObject::calculateDistance(sf::Vector2f point) const
{
	auto rect = text_.getGlobalBounds();
	std::vector<Segment<float>> sides = {
		Segment<float>({ rect.top, rect.left }, { rect.top, rect.left + rect.width }),
		Segment<float>({ rect.top, rect.left + rect.width }, { rect.top + rect.height, rect.left + rect.width }),
		Segment<float>({ rect.top + rect.height, rect.left + rect.width }, { rect.top + rect.height, rect.left }),
		Segment<float>({ rect.top + rect.height, rect.left }, { rect.top, rect.left })
	};
	std::valarray<float> distances(sides.size());
	std::transform(std::begin(sides), std::end(sides),
		std::begin(distances),
		[&point](const auto& side) { return distance(point, side); });
	return distances.sum() > (rect.width + rect.height) ?
		distances.min() :
		0.0f;
}

void TextObject::setCharSize(unsigned int charSize)
{
	text_.setCharacterSize(charSize);
}

void TextObject::setFillColor(sf::Color color)
{
	text_.setFillColor(color);
}

void TextObject::setText(const std::string& text)
{
	text_.setString(text);
}