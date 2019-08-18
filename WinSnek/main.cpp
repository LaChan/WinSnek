#include <iostream>
#include "SnakeGame.h"

using namespace std;

int main()
{
	SnakeGame game;
	
	// 20 rows wide, 24 columns high, 
	// and make each "pixel" take up 24x24 of your high resolution screen's pixels.
	game.ConstructConsole(20, 24, 24, 24);
	
	game.Start();

	return 0;
}\`