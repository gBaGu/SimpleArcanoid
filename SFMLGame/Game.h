#pragma once
#include <string>
#include <memory>
#include <vector>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

#include "Ball.h"
#include "Brick.h"
#include "BricksLayout.h"
#include "Interface.h"
#include "Modifier.h"
#include "MyTime.h"
#include "Paddle.h"
#include "Setting.h"


struct Collision
{
	sf::Vector2f point; //need to calculate velocity and AOE
	sf::Vector2f velocityAfter;
	std::weak_ptr<Object> obj; //need to find if collision is already in list

	Collision(sf::Vector2f p, sf::Vector2f v, std::shared_ptr<Object> o)
		: point(p), velocityAfter(v), obj(o) {}
};

using collision_ptr = std::shared_ptr<Collision>;


class Game
{
public:
	Game(const std::string& name, size_t maxFps, std::shared_ptr<BricksLayout> bl);

	/*
	standart game loop: clear()->update()->draw()->display()
	*/
	void run();

	enum class State { Menu, Playing };

private:
	void checkGameOver();
	void clearBrokenBricks();
	/* draw()
	draws all objects on window
	*/
	void draw();
	/* getWindowRect()
	returns window rectangle
	*/
	sf::RectangleShape getWindowRect() const;
	/* findCollision()
	returns old collision and new collision
	*/
	std::pair<collision_ptr, collision_ptr> findCollision(std::shared_ptr<RectangleObject> obj) const;
	/* findCollision()
	returns old collision
	*/
	collision_ptr findOldCollision(std::shared_ptr<Object> obj) const;
	void hitAffected();
	/* initBricks()
	fills vector of bricks
	*/
	void initBricks(std::shared_ptr<BricksLayout> bl);
	/* isBallDropped()
	checks if ball is out of screen
	*/
	bool isBallDropped() const;
	bool isDifficultyTimerUp() const;
	bool isInAOE(sf::Vector2f hitPoint, std::shared_ptr<Object> obj) const;
	/* update()
	updates all objects:
	check for collisions, recalculate velocity,
	show popup if there is one
	*/
	void update();
	void updateCollision(std::shared_ptr<RectangleObject> obj);
	/**
	* For every brick in the game checks
	*     - if ball had a collision with it on last iteration
	*     - if ball has a collision with it now
	* updates the collision list respectively to this conditions
	*/
	void updateCollisions();
	void updateDifficulty();
	void updateVelocity();

	State state_;
	std::shared_ptr<sf::RenderWindow> window_;
	std::unique_ptr<interface::Interface> interface_;
	std::vector<collision_ptr> activeCollisions_;
	std::shared_ptr<Ball> ball_;
	std::shared_ptr<Paddle> paddle_;
	std::vector<std::shared_ptr<Brick>> bricks_;
	std::vector<std::shared_ptr<Modifier>> modifiers_;
	const duration_t updateDifficultyDuration_ = duration_t(10);
	time_point_t lastUpdateDifficultyTime_ = default_clock::now();
	bool running_ = false;
	bool gameOver_ = false;
};