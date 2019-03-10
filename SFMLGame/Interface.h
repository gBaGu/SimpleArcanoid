#pragma once
#include <functional>
#include <memory>

#include <SFML\Graphics.hpp>

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

		void resetInitTime();
		void setDuration(duration_t duration);

	private:
		duration_t duration_;
		time_point_t initTime_ = default_clock::now();
	};


	class Button
	{
	public:
		Button(const std::function<void()>& handler)
			: handler_(handler) {}

		virtual bool isPressed() const = 0;

		void trigger() { if (handler_) handler_(); }

	private:
		std::function<void()> handler_;
	};


	class TextButton : public Button, public TextObject
	{
	public:
		TextButton(std::shared_ptr<sf::RenderWindow> window,
			sf::Vector2f center, unsigned int charSize,
			const std::string& text, const sf::Font& font,
			const std::function<void()>& handler);

		virtual bool isPressed() const;

	private:
		std::shared_ptr<sf::RenderWindow> window_;
	};


	class Interface
	{
	public:
		Interface(std::shared_ptr<sf::RenderWindow> window,
			const std::function<void()>& onPlay,
			const std::function<void()>& onExit);

		void draw(sf::RenderWindow& window) const;
		void update();

		void showMessage(const std::string& message);

		static const std::string FontFilePath;
		static const duration_t InfoMessageDuration;
		static const unsigned int InfoMessageCharSize;
		static const unsigned int MenuButtonCharSize;

	private:
		sf::Font font_;
		std::unique_ptr<TemporaryText> message_;
		std::unique_ptr<TextButton> play_;
		std::unique_ptr<TextButton> exit_;
	};

};