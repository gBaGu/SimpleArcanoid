#include "Game.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>

using sf::Keyboard;


Game::Game(const std::string& name, size_t maxFps, std::shared_ptr<BricksLayout> bl)
	: window_(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), name)	
{
	window_.setFramerateLimit(maxFps);
	ball_ = std::make_shared<Ball>(100, 100/*WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2*/);
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

void Game::checkGameOver()
{
	if (isBallDropped())
	{
		message_ = std::make_unique<PopUpMessage>(window_, "Game Over",
			font_, duration_t(5));
		gameOver_ = true;
	}
}

void Game::clearBrokenBricks()
{
	auto bricksToRemove = std::remove_if(std::begin(bricks_), std::end(bricks_),
		[](const auto& brick) { return brick->isBroken(); });
	bricks_.erase(bricksToRemove, std::end(bricks_));
	auto collisionsToRemove = std::remove_if(std::begin(activeCollisions_), std::end(activeCollisions_),
		[](const auto& collision) { return collision->obj.expired(); });
	activeCollisions_.erase(collisionsToRemove, std::end(activeCollisions_));
}

void Game::draw()
{
	window_.draw(*ball_);
	paddle_->draw(window_);
	for (const auto& brick : bricks_)
	{
		brick->draw(window_);
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

std::pair<collision_ptr, collision_ptr> Game::findCollision(std::shared_ptr<RectangleObject> obj) const
{
	collision_ptr oldCollision = findOldCollision(obj);
	collision_ptr collision = nullptr;
	auto prevCenter = ball_->getPrevCenter();
	auto center = ball_->getPosition();
	auto radius = ball_->getRadius();
	auto rectSides = obj->getSides();
	std::vector<sf::Vector2f> sideIntersections;
	for (const auto& rectSide : rectSides)
	{
		auto intersection = getIntersectionPoint(Segment<float>(prevCenter, center),
			Segment<float>(rectSide.A, rectSide.B));
		if (intersection.first)
		{
			sideIntersections.push_back(intersection.second);
		}
		auto point = closestPoint(center, rectSide);
		if (length(point - center) < radius)
		{
			collision = oldCollision;
			if (!collision)
			{
				auto velocityAfter = ball_->calculateReflection(point);
				collision = std::make_shared<Collision>(point, velocityAfter, obj);
			}
		}
	}
	if (!sideIntersections.empty())
	{
		collision = oldCollision;
		if (!collision)
		{
			auto closestIntersection = std::min_element(std::begin(sideIntersections),
				std::end(sideIntersections),
				[prevCenter](const auto& l, const auto& r)
			{
				return length(l - prevCenter) < length(r - prevCenter);
			});
			auto velocityAfter = ball_->calculateReflection(*closestIntersection);
			collision = std::make_shared<Collision>(*closestIntersection, velocityAfter, obj);
		}
	}
	return std::make_pair(oldCollision, collision);
}

collision_ptr Game::findOldCollision(std::shared_ptr<Object> obj) const
{
	collision_ptr collision = nullptr;
	auto collisionIt = std::find_if(std::begin(activeCollisions_), std::end(activeCollisions_),
		[&obj](const auto& col)
	{
		return col->obj.lock() == obj;
	});
	if (collisionIt != std::end(activeCollisions_))
	{
		collision = *collisionIt;
	}
	return collision;
}

void Game::hitAffected()
{
	if (activeCollisions_.empty())
	{
		return;
	}
	std::vector<std::shared_ptr<Brick>> affected;
	std::set<std::shared_ptr<Brick>> tmpBricks(std::begin(bricks_), std::end(bricks_));
	for (const auto& collision : activeCollisions_)
	{
		std::vector<decltype(tmpBricks)::iterator> toErase;
		for (auto it = std::begin(tmpBricks); it != std::end(tmpBricks); it++)
		{
			if (*it == collision->obj.lock()/*TODO: || inAOE(*it, collision->point)*/)
			{
				affected.emplace_back(std::move(*it));
				toErase.push_back(it);
			}
		}
		std::for_each(std::begin(toErase), std::end(toErase),
			[&tmpBricks](auto it) { tmpBricks.erase(it); });
	}

	std::for_each(std::begin(affected), std::end(affected),
		[](auto& brick) { brick->takeDamage(); });
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

	updateCollisions();
	hitAffected();
	updateVelocity();
	clearBrokenBricks();
	
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
	checkGameOver();
}

void Game::updateCollision(std::shared_ptr<RectangleObject> obj)
{
	auto collision = findCollision(obj);
	if (!collision.first && collision.second)
	{
		activeCollisions_.push_back(collision.second);
	}
	else if (collision.first && !collision.second)
	{
		auto toRemove = std::remove(std::begin(activeCollisions_), std::end(activeCollisions_),
			collision.first);
		activeCollisions_.erase(toRemove, std::end(activeCollisions_));
	}
}

void Game::updateCollisions()
{
	for (const auto& brick : bricks_)
	{
		updateCollision(brick);
	}
	updateCollision(paddle_);
}

void Game::updateDifficulty()
{
	auto speedDiff = ball_->getBaseSpeed() * 0.1;
	ball_->changeSpeed(speedDiff);
	message_ = std::make_unique<PopUpMessage>(window_, "Speed up!",
		font_, duration_t(1));
	lastUpdateDifficultyTime_ = default_clock::now();
}

void Game::updateVelocity()
{
	if (activeCollisions_.empty())
	{
		return;
	}
	//Only update ball velocity for now
	sf::Vector2f newVelocity = std::accumulate(std::begin(activeCollisions_), std::end(activeCollisions_),
		sf::Vector2f(0.0f, 0.0f),
		[](sf::Vector2f v, collision_ptr collision) { return v + collision->velocityAfter; });
	ball_->setVelocity(newVelocity);
}