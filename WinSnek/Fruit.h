#pragma once

class Fruit
{

public:
	int x = 0;
	int y = 0;
	int colour = 0;
	int normalColour = 0;
	int bonusColour = 0;

	const int bonusMultiplier = 3;
	
	Fruit();
	~Fruit();
};

