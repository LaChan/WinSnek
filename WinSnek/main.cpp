#include <iostream>
#include "SnakeGame.h"

using namespace std;

int main()
{

	SnakeGame game;

	// 20x24 game board
	game.ConstructConsole(20, 24, 24, 24);
	game.Start();
		
	return 0;
}