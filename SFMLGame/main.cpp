#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include "Setting.h"
#include "Util.h"
#include "Ball.h"
#include "Paddle.h"


int main()
{
	using sf::Keyboard;

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML Game");
	window.setFramerateLimit(60);

	Ball ball(windowWidth / 2, windowHeight / 2);
	Paddle paddle(windowWidth / 2, windowHeight - 50);

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


		window.draw(ball);
		window.draw(paddle);
		window.display();
	}

	return 0;
}