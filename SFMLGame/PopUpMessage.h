#pragma once
#include <chrono>
#include <string>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>


class PopUpMessage : public sf::Text
{
public:
	//duration in seconds
	using duration_t = std::chrono::duration<int, std::ratio<1>>;
	using clock_t = std::chrono::system_clock;
	using time_point_t = std::chrono::time_point<clock_t>;

	PopUpMessage(const sf::RenderWindow& window, const std::string& message,
		const sf::Font& font, duration_t duration);

	bool isExpired() const;

private:
	duration_t duration_;
	time_point_t initTime_ = clock_t::now();
};