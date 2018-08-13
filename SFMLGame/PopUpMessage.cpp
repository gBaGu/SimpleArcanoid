#include "PopUpMessage.h"


PopUpMessage::PopUpMessage(const sf::RenderWindow& window, const std::string& message,
	const sf::Font& font, duration_t duration)
	: sf::Text(message, font, 50), duration_(duration)
{
	auto wSize = window.getSize();
	auto textRect = getLocalBounds();
	setOrigin(textRect.width / 2, textRect.height / 2);
	move(wSize.x / 2, wSize.y / 2);
	setFillColor(sf::Color::Red);
}

bool PopUpMessage::isExpired() const
{
	auto dur = clock_t::now() - initTime_;
	return dur > duration_;
}