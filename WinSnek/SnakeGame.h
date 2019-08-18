#pragma once
#include "olcConsoleGameEngineOOP.h"
#include "SnakeHead.h"
#include "Fruit.h"

class SnakeGame : public olcConsoleGameEngineOOP
{
public:
	
	SnakeGame();
	~SnakeGame();

protected:
	int _score = 232995;

	virtual bool OnUserCreate();
	virtual bool OnUserUpdate(float fElapsedTime);

	bool isUpKeyHeld_ = false;
	bool isDownKeyHeld_ = false;
	bool isRightKeyHeld_ = false;
	bool isLeftKeyHeld_ = false;

	const int START_X = 10;
	const int HEAD_COLOUR = BG_DARK_BLUE | FG_DARK_RED;
	const int GROUND_COLOUR = FG_BLUE | BG_DARK_BLUE;	

	SnakeHead _head;
	Fruit _fruit;

	void RenderWorld();
};