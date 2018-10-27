#pragma once
#include <string>
#include <memory>
#include <vector>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

#include "Ball.h"
#include "Brick.h"
#include "BricksLayout.h"
#include "Modificator.h"
#include "MyTime.h"
#include "Paddle.h"
#include "PopUpMessage.h"
#include "Setting.h"


class Game
{
public:
	Game(const std::string& name, size_t maxFps, std::shared_ptr<BricksLayout> bl);

	/*
	standart game loop: clear()->update()->draw()->display()
	*/
	void run();

private:
	/* draw()
	draws all objects on window
	*/
	void draw();
	/* getWindowRect()
	returns window rectangle
	*/
	sf::RectangleShape getWindowRect() const;
	/* initBricks()
	fills vector of bricks
	*/
	void initBricks(std::shared_ptr<BricksLayout> bl);
	/* isBallDropped()
	checks if ball is out of screen
	*/
	bool isBallDropped() const;
	bool isDifficultyTimerUp() const;
	/* update()
	updates all objects:
	check for collisions, recalculate velocity,
	show popup if there is one
	*/
	void update();
	void updateDifficulty();

	sf::RenderWindow window_;
	std::shared_ptr<Ball> ball_;
	std::shared_ptr<Paddle> paddle_;
	std::vector<std::shared_ptr<Brick>> bricks_;
	std::vector<std::shared_ptr<Modificator>> modificators_;
	sf::Font font_;
	std::unique_ptr<PopUpMessage> message_;
	const duration_t updateDifficultyDuration_ = duration_t(10);
	time_point_t lastUpdateDifficultyTime_ = default_clock::now();
	bool running_ = false;
	bool gameOver_ = false;
};