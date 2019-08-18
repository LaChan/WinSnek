#pragma once

class SnakeHead
{

public:
	float x = 0.0f;
	float y = 0.0f;
	float lastX = 0.0f;
	float lastY = 0.0f;
	float speed = 0.0f;
	int cellsPerSec = 15;
	int lastWholeCell = -1;
	int colour = 0;
	enum Direction { STOP, LEFT, RIGHT, UP, DOWN };
	Direction dir;
};