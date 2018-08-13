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

	/*
	standart game loop: clear()->update()->draw()->display()
	*/
	void run();

private:
	/*
	draws all objects on window
	*/
	void draw();
	/*
	returns window rectangle
	*/
	sf::RectangleShape getWindowRect() const;
	/*
	fills vector of bricks
	*/
	void initBricks();
	/*
	checks if ball is out of screen
	*/
	bool isBallDropped() const;
	/*
	updates all objects
	*/
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