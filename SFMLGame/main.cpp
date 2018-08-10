#include <iostream>

#include "Game.h"


int main()
{
	Game game("SFML Game", 60);
	game.run();
	std::cout << "Game Over" << std::endl;

	std::cin.get();
	return 0;
}