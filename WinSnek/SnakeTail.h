#pragma once
class SnakeTail
{
	
public:
	float x = 0.0f;
	float y = 0.0f;
	float lastX = 0.0f;
	float lastY = 0.0f;
	float speed = 0.0f;
	int cellX = 0;
	int cellY = 0;
	int cellsPerSec = 15;
	int lastCellX = -1;
	int lastCellY = -1;
	int colour = 0;
	enum Direction { STOP, LEFT, RIGHT, UP, DOWN };
	Direction dir;

	SnakeTail(int x, int y);
	~SnakeTail();
};
