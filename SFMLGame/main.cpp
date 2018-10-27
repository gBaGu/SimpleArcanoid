#include <iostream>
#include <memory>

#include "BricksLayout.h"
#include "Game.h"
#include "Setting.h"


int main()
{
	std::srand(std::time(0));
	Point tl{ 0, 0 }, br{ static_cast<float>(WINDOW_WIDTH), WINDOW_HEIGHT / 2 };
	Point margin{ 20, 20 };
	std::shared_ptr<BricksLayout> bl = std::make_shared<RectangleBricksLayout>(tl, br, margin);
	Game game("SFML Game", 60, bl);
	game.run();
	return 0;
}