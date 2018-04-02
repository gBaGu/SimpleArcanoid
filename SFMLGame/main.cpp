#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include <algorithm>

#include "Setting.h"
#include "Util.h"
#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"


int main()
{
	using sf::Keyboard;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Game");
	window.setFramerateLimit(60);

	Ball ball(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	Paddle paddle(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 50);
	std::vector<Brick> bricks;
	for (int iX = 0; iX < COUNT_BRICKS_X; iX++)
	{
		for (int iY = 0; iY < COUNT_BRICKS_Y; iY++)
		{
			bricks.emplace_back((iX + 1) * (Brick::DEFAULT_WIDTH + 3) + 22,
				(iY + 2) * (Brick::DEFAULT_HEIGHT + 3));
		}
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
			break;

		ball.update();
		paddle.update();
		ball.checkCollision(paddle);
		auto brokenBrick = std::find_if(bricks.begin(), bricks.end(),
			[&ball](const Brick& brick) { return ball.checkCollision(brick); });
		if (brokenBrick != bricks.end())
			bricks.erase(brokenBrick);

		window.draw(ball);
		window.draw(paddle);
		for (auto& brick : bricks)
		{
			window.draw(brick);
		}

		window.display();
	}

	return 0;
}