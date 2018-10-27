#include "Game.h"

#include <algorithm>
#include <iostream>

using sf::Keyboard;


Game::Game(const std::string& name, size_t maxFps, std::shared_ptr<BricksLayout> bl)
	: window_(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), name)	
{
	window_.setFramerateLimit(maxFps);
	ball_ = std::make_shared<Ball>(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	paddle_ = std::make_shared<Paddle>(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50);
	initBricks(bl);
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
	window_.draw(*ball_);
	window_.draw(*paddle_);
	for (const auto& brick : bricks_)
	{
		window_.draw(*brick);
	}
	for (const auto& mod : modificators_)
	{
		window_.draw(*mod);
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

void Game::initBricks(std::shared_ptr<BricksLayout> bl)
{
	bricks_.clear();
	bricks_.reserve(COUNT_BRICKS_X * COUNT_BRICKS_Y);
	auto onDestroy = [ball = this->ball_, &mods = this->modificators_](float x, float y)
	{
		if (rand() % 100 < 10)
		{
			std::shared_ptr<Modificator> mod = std::make_shared<BallSpeedPositiveModificator>(x, y, ball);
			mods.push_back(mod);
		}
	};
	while (bl->hasNext())
	{
		auto brick = bl->getNext();
		brick->setOnDestroy(onDestroy);
		bricks_.push_back(brick);
	}
}

bool Game::isBallDropped() const
{
	return ball_->getPosition().y > WINDOW_HEIGHT;
}

bool Game::isDifficultyTimerUp() const
{
	auto dur = default_clock::now() - lastUpdateDifficultyTime_;
	return dur > updateDifficultyDuration_;
}

void Game::update()
{
	ball_->update();
	paddle_->update();
	for (auto& mod : modificators_)
	{
		mod->update();
	}
	if (!ball_->isIntersecting(getWindowRect()))
	{
		ball_->stop();
	}
	if (gameOver_)
	{
		return;
	}

	if (isDifficultyTimerUp())
	{
		updateDifficulty();
	}
	ball_->checkCollision(*paddle_);
	auto brokenBrick = std::find_if(bricks_.begin(), bricks_.end(),
		[this](const auto& brick) { return ball_->checkCollision(*brick); });
	if (brokenBrick != bricks_.end())
	{
		bricks_.erase(brokenBrick);
	}
	modificators_.erase(std::remove_if(modificators_.begin(), modificators_.end(),
		[this](const auto& mod)
	{
		auto modRect = mod->getGlobalBounds();
		auto windowRect = getWindowRect().getGlobalBounds();
		auto paddleRect = paddle_->getGlobalBounds();
		if (modRect.intersects(paddleRect))
		{
			mod->trigger();
			return true;
		}
		return !windowRect.intersects(modRect);
	}),
		modificators_.end());

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
	auto speedDiff = ball_->getBaseSpeed() * 0.1;
	ball_->changeSpeed(speedDiff);
	message_ = std::make_unique<PopUpMessage>(window_, "Speed up!",
		font_, duration_t(1));
	lastUpdateDifficultyTime_ = default_clock::now();
}