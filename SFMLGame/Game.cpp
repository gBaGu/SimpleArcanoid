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
	ball_->draw(window_);
	paddle_->draw(window_);
	for (const auto& brick : bricks_)
	{
		brick->draw(window_);
	}
	for (const auto& mod : modifiers_)
	{
		mod->draw(window_);
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
	using intersection_t = std::pair<bool, sf::Vector2f>;
	collision_ptr oldCollision = findOldCollision(obj);
	collision_ptr collision = nullptr;
	auto prevCenter = ball_->getPrevCenter();
	auto center = ball_->getPosition();
	auto radius = ball_->getRadius();
	auto rectSides = obj->getSides();
	std::vector<std::pair<sf::Vector2f, float>> byCenter, byRadius;
	for (const auto& rectSide : rectSides)
	{
		//fill byCenter
		auto intersection = getIntersectionPoint(Segment<float>(prevCenter, center),
			Segment<float>(rectSide.A, rectSide.B));
		if (intersection.first)
		{
			auto l = length(intersection.second - prevCenter);
			byCenter.emplace_back(intersection.second, l);
		}
		//fill byRadius
		if (byCenter.empty()) //there is no point to do this when we know
						//that ball`s center had intersected one of rect`s line
		{
			auto point = closestPoint(center, rectSide);
			auto l = length(point - center);
			if (l < radius)
			{
				byRadius.emplace_back(point, l);
			}
		}
	}
	if (!byCenter.empty())
	{ //we know there is a collision so if
		//we have already had collision with this obj on the previous step
		//return old collision
		collision = oldCollision;
		if (!collision)
		{
			auto closest = std::min_element(std::begin(byCenter), std::end(byCenter),
				[](const auto& l, const auto& r) { return l.second < r.second; });
			auto velocityAfter = ball_->calculateReflection(closest->first);
			collision = std::make_shared<Collision>(closest->first, velocityAfter, obj);
		}
	}
	else if (!byRadius.empty())
	{
		collision = oldCollision;
		if (!collision)
		{
			auto closest = std::min_element(std::begin(byRadius), std::end(byRadius),
				[](const auto& l, const auto& r) { return l.second < r.second; });
			auto velocityAfter = ball_->calculateReflection(closest->first);
			collision = std::make_shared<Collision>(closest->first, velocityAfter, obj);
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
			if (*it == collision->obj.lock() || isInAOE(collision->point, *it))
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
	auto onDestroy = [ball = this->ball_, &mods = this->modifiers_](float x, float y)
	{
		std::shared_ptr<Modifier> mod = nullptr;
		auto rand = std::rand() % 100;
		if (rand < 5)
		{
			mod = std::make_shared<ObjectSpeedPositiveModifier>(x, y, ball);
		}
		else if (rand < 10)
		{
			mod = std::make_shared<BallHitPositiveModifier>(x, y, ball);
		}
		else if (rand < 15)
		{
			mod = std::make_shared<BallSizePositiveModifier>(x, y, ball);
		}
		if (mod)
		{
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

bool Game::isInAOE(sf::Vector2f hitPoint, std::shared_ptr<Object> obj) const
{
	return obj->calculateDistance(hitPoint) < ball_->getAreaOfHit();
}

void Game::update()
{
	ball_->update();
	paddle_->update();
	for (auto& mod : modifiers_)
	{
		mod->update();
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
	
	modifiers_.erase(std::remove_if(modifiers_.begin(), modifiers_.end(),
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
		modifiers_.end());

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
	//BALL
	auto velocity = ball_->getVelocity();
	if (!ball_->getGlobalBounds().intersects(getWindowRect().getGlobalBounds()))
	{
		velocity = sf::Vector2f(0.0f, 0.0f);
	}
	else if (!activeCollisions_.empty())
	{
		velocity = std::accumulate(std::begin(activeCollisions_),
			std::end(activeCollisions_),
			sf::Vector2f(0.0f, 0.0f),
			[](sf::Vector2f v, collision_ptr collision) { return v + collision->velocityAfter; });
	}
	auto center = ball_->getPosition();
	auto radius = ball_->getRadius();
	if (center.x - radius < 0)
	{
		velocity.x = std::max(velocity.x, -velocity.x);
	}
	else if (center.x + radius > WINDOW_WIDTH)
	{
		velocity.x = std::min(velocity.x, -velocity.x);
	}
	if (center.y - radius < 0)
	{
		velocity.y = std::max(velocity.y, -velocity.y);
	}
	ball_->setVelocity(velocity);

	//PADDLE
	using sf::Keyboard;
	auto points = paddle_->getPoints();
	sf::Vector2f newVelocity(0.0f, 0.0f);
	if (Keyboard::isKeyPressed(Keyboard::Key::Left) &&
		!Keyboard::isKeyPressed(Keyboard::Key::Right) &&
		std::all_of(std::begin(points), std::end(points),
			[](const auto& point) { return point.x > 0; }))
	{
		newVelocity.x = -1.0f;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Key::Right) &&
		!Keyboard::isKeyPressed(Keyboard::Key::Left) &&
		std::all_of(std::begin(points), std::end(points),
			[](const auto& point) { return point.x < WINDOW_WIDTH; }))
	{
		newVelocity.x = 1.0f;
	}
	paddle_->setVelocity(newVelocity);
}