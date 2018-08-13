#pragma once
#include <string>
#include <memory>
#include <vector>

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"
#include "PopUpMessage.h"
#include "Setting.h"


class Game
{
public:
	Game(const std::string& name, size_t maxFps);

	void run();

private:
	void draw();
	sf::RectangleShape getWindowRect() const;
	void initBricks();
	bool isBallDropped() const;
	void update();

	sf::RenderWindow window_;
	sf::Font font_;
	std::unique_ptr<PopUpMessage> message_;
	Ball ball_;
	Paddle paddle_;
	std::vector<Brick> bricks_;
	bool running_ = false;
	bool gameOver_ = false;
};