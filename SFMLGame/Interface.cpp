#include "Interface.h"


namespace interface
{
	TemporaryText::TemporaryText(sf::Vector2f velocity, float speed, sf::Vector2f center,
		unsigned int charSize, const std::string& text,
		const sf::Font& font, duration_t duration)
		: TextObject(velocity, speed, center, charSize, text, font), duration_(duration)
	{
		setFillColor(sf::Color::Red);
	}

	bool TemporaryText::isExpired() const
	{
		auto dur = default_clock::now() - initTime_;
		return dur > duration_;
	}
}