#include <iostream>

#include "Game.h"


int main()
{
	std::srand(std::time(0));
	Game game("SFML Game", 60);
	game.run();
	return 0;
}