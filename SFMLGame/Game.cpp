#include "Game.h"

#include <algorithm>
#include <iostream>

using sf::Keyboard;


Game::Game(const std::string& name, size_t maxFps)
	: window_(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), name),
	ball_(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
	paddle_(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50)
{
	window_.setFramerateLimit(maxFps);
	initBricks();
	try
	{
		if (!font_.loadFromFile("data/arial.ttf"))
		{
			std::cout << "Failed to load font" << std::endl;
		}
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}

void Game::run()
{
	running_ = true;
	while (running_)
	{
		sf::Event event;
		while (window_.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window_.close();
				running_ = false;
			}
		}

		window_.clear(sf::Color::Black);
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
		{
			running_ = false;
		}

		update();
		draw();
		window_.display();
	}
}

void Game::draw()
{
	window_.draw(ball_);
	window_.draw(paddle_);
	for (const auto& brick : bricks_)
	{
		window_.draw(brick);
	}
	if (message_)
	{
		window_.draw(*message_);
	}
}

sf::RectangleShape Game::getWindowRect() const
{
	sf::RectangleShape r(sf::Vector2f(window_.getSize().x, window_.getSize().y));
	return r;
}

void Game::initBricks()
{
	bricks_.clear();
	bricks_.reserve(COUNT_BRICKS_X * COUNT_BRICKS_Y);
	for (int iX = 0; iX < COUNT_BRICKS_X; iX++)
	{
		for (int iY = 0; iY < COUNT_BRICKS_Y; iY++)
		{
			bricks_.emplace_back((iX + 1) * (Brick::DEFAULT_WIDTH + 3) + 22,
				(iY + 2) * (Brick::DEFAULT_HEIGHT + 3));
		}
	}
}

bool Game::isBallDropped() const
{
	return ball_.getPosition().y > WINDOW_HEIGHT;
}

bool Game::isDifficultyTimerUp() const
{
	auto dur = default_clock::now() - lastUpdateDifficultyTime_;
	return dur > updateDifficultyDuration_;
}

void Game::update()
{
	ball_.update();
	paddle_.update();
	if (!ball_.isIntersecting(getWindowRect()))
	{
		ball_.stop();
	}
	if (gameOver_)
	{
		return;
	}

	if (isDifficultyTimerUp())
	{
		updateDifficulty();
	}
	ball_.checkCollision(paddle_);
	auto brokenBrick = std::find_if(bricks_.begin(), bricks_.end(),
		[this](const Brick& brick) { return ball_.checkCollision(brick); });
	if (brokenBrick != bricks_.end())
	{
		bricks_.erase(brokenBrick);
	}

	if (message_)
	{
		if (message_->isExpired())
		{
			message_.reset();
		}
	}
	if (isBallDropped())
	{
		message_ = std::make_unique<PopUpMessage>(window_, "Game Over",
			font_, duration_t(5));
		gameOver_ = true;
	}
}

void Game::updateDifficulty()
{
	auto newBallSpeed = ball_.getSpeed() * 1.1;
	ball_.setSpeed(newBallSpeed);
	message_ = std::make_unique<PopUpMessage>(window_, "Speed up!",
		font_, duration_t(1));
	lastUpdateDifficultyTime_ = default_clock::now();
}