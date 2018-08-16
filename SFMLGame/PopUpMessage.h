#pragma once
#include <string>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

#include "MyTime.h"


class PopUpMessage : public sf::Text
{
public:
	PopUpMessage(const sf::RenderWindow& window, const std::string& message,
		const sf::Font& font, duration_t duration);

	bool isExpired() const;

private:
	duration_t duration_;
	time_point_t initTime_ = default_clock::now();
};