#pragma once
#include "MyTime.h"
#include "Object.h"


namespace interface
{
	class TemporaryText : public TextObject
	{
	public:
		TemporaryText(sf::Vector2f velocity, float speed, sf::Vector2f center,
			unsigned int charSize, const std::string& text,
			const sf::Font& font, duration_t duration);

		bool isExpired() const;

	private:
		duration_t duration_;
		time_point_t initTime_ = default_clock::now();
	};


	class Interface
	{
	public:
	private:

	};

};