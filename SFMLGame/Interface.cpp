#include "Interface.h"

#include <iostream>


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

	void TemporaryText::resetInitTime()
	{
		initTime_ = default_clock::now();
	}

	void TemporaryText::setDuration(duration_t duration)
	{
		duration_ = duration;
	}


	TextButton::TextButton(std::shared_ptr<sf::RenderWindow> window,
		sf::Vector2f center, unsigned int charSize,
		const std::string& text, const sf::Font& font,
		const std::function<void()>& handler)
		: TextObject({ 0, 0 }, 0, center, charSize, text, font), Button(handler),
		window_(window)
	{
		setFillColor(sf::Color::Red);
	}

	bool TextButton::isPressed() const
	{
		if (!isVisible())
		{
			return false;
		}

		return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
			getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window_)));
	}


	const std::string Interface::FontFilePath = "data/arial.ttf";
	const duration_t Interface::InfoMessageDuration = duration_t(1);
	const unsigned int Interface::InfoMessageCharSize = 50;
	const unsigned int Interface::MenuButtonCharSize = 40;

	Interface::Interface(std::shared_ptr<sf::RenderWindow> window,
		const std::function<void()>& onPlay,
		const std::function<void()>& onExit)
	{
		try
		{
			if (!font_.loadFromFile(FontFilePath))
			{
				std::cout << "Failed to load font from "
					<< FontFilePath << std::endl;
			}
		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}

		auto windowSize = window->getSize();
		message_ = std::make_unique<TemporaryText>(sf::Vector2f(0, 0), 0.0f,
			sf::Vector2f(windowSize / 2u), InfoMessageCharSize, "", font_,
			InfoMessageDuration);
		message_->setVisible(false);
		play_ = std::make_unique<TextButton>(window,
			sf::Vector2f(windowSize.x / 2, windowSize.y / 3),
			MenuButtonCharSize, "Play", font_, [this, onPlay]()
		{
			hideMenu();
			onPlay();
		});
		exit_ = std::make_unique<TextButton>(window,
			sf::Vector2f(windowSize.x / 2, (windowSize.y / 3) * 2),
			MenuButtonCharSize, "Exit", font_, onExit);
	}

	void Interface::draw(sf::RenderWindow& window) const
	{
		message_->draw(window);
		play_->draw(window);
		exit_->draw(window);
	}

	void Interface::update()
	{
		message_->setVisible(!message_->isExpired());
		if (play_->isPressed())
		{
			play_->trigger();
		}
		if (exit_->isPressed())
		{
			exit_->trigger();
		}
	}

	void Interface::hideMenu()
	{
		play_->setVisible(false);
		exit_->setVisible(false);
	}

	void Interface::showMenu()
	{
		play_->setVisible(true);
		exit_->setVisible(true);
	}

	void Interface::showMessage(const std::string& message)
	{
		message_->setText(message);
		message_->setVisible(true);
		message_->resetInitTime();
	}
}