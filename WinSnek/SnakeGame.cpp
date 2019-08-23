#include "SnakeGame.h"
#include "olcConsoleGameEngineOOP.h"
#include "SnakeHead.h"
#include "SnakeTail.h"
#include "Fruit.h"
#include <vector>


bool SnakeGame::OnUserCreate()
{
	_snakeHead.x = _snakeHead.y = (float)START_X;
	_snakeHead.cellX = _snakeHead.cellY = START_X;
	_snakeHead.speed = 0.0015f;
	_snakeHead.dir = _snakeHead.UP;
	_snakeHead.colour = HEAD_COLOUR;
	_fruit.x = (rand() % ScreenWidth());
	_fruit.y = (rand() % ScreenHeight());
	_fruit.colour = FG_GREEN | BG_WHITE;
	_score = 0;
	srand((int)time(0)); // create random seed from current epoch time
	return true;
}

bool SnakeGame::OnUserUpdate(float fElapsedTime)
{
	// Get input
	isUpKeyHeld_ = (m_keys[VK_UP].bHeld || m_keys[VK_UP].bPressed) ? true : false;
	isDownKeyHeld_ = (m_keys[VK_DOWN].bHeld || m_keys[VK_DOWN].bPressed) ? true : false;
	isRightKeyHeld_ = (m_keys[VK_RIGHT].bHeld || m_keys[VK_RIGHT].bPressed) ? true : false;
	isLeftKeyHeld_ = (m_keys[VK_LEFT].bHeld || m_keys[VK_LEFT].bPressed) ? true : false;

	// update everything
	if (isUpKeyHeld_)
		_snakeHead.dir = _snakeHead.UP;
	if (isDownKeyHeld_)
		_snakeHead.dir = _snakeHead.DOWN;
	if (isRightKeyHeld_)
		_snakeHead.dir = _snakeHead.RIGHT;
	if (isLeftKeyHeld_)
		_snakeHead.dir = _snakeHead.LEFT;

	//check for teleport 
	if (_snakeHead.x > ScreenWidth())
	{
		_snakeHead.x = 0;
	}
	else if (_snakeHead.x < 0.0)
	{
		_snakeHead.x = ScreenWidth();
	}
	else if (_snakeHead.y > ScreenHeight())
	{
		_snakeHead.y = 0;
	}
	else if (_snakeHead.y < 0.0)
	{
		_snakeHead.y = ScreenHeight();
	}

	//check for fruit collison
	if (_snakeHead.cellX == _fruit.x && _snakeHead.cellY == _fruit.y)
	{
		//update the score
		_score += 10;

		//randomly generate fruit at a new position  
		_fruit.x = (rand() % ScreenWidth());
		_fruit.y = (rand() % ScreenHeight());
		
		//add a new tail piece
		_tailPieces.push_back(SnakeTail());

	}

	// Draw the world
	RenderWorld();
	
	// OnUserUpdate has to return true for the engine to continue
	return true;
	}

void SnakeGame::RenderWorld()
{
	// Clear the screen by drawing GROUND colour
	Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, GROUND_COLOUR);
	DrawString(0, 0, L"Score: " + to_wstring(_score));
	DrawString(0, 1, L"tail vector size " + to_wstring(_tailPieces.size()));

	//Move the Snake

	//Save previous frame coords
	_snakeHead.lastY = _snakeHead.y;
	_snakeHead.lastX = _snakeHead.x;

	//Actually move the snake
	if (_snakeHead.dir == _snakeHead.UP)
	{
		_snakeHead.y -= _snakeHead.speed;
	}
	else if (_snakeHead.dir == _snakeHead.DOWN)
	{
		_snakeHead.y += _snakeHead.speed;
	}
	else if (_snakeHead.dir == _snakeHead.RIGHT)
	{
		_snakeHead.x += _snakeHead.speed;
	}
	else if (_snakeHead.dir = _snakeHead.LEFT)
	{
		_snakeHead.x -= _snakeHead.speed;

	}

	//Round float coords to cell
	_snakeHead.cellX = round(_snakeHead.x);
	_snakeHead.cellY = round(_snakeHead.y);

	//Are we now over halfway to a new cell in x-plane?
	if (round(_snakeHead.x) != round(_snakeHead.lastX)) {

		//update last cell value
		_snakeHead.lastCellX = (int)round(_snakeHead.lastX);

		for (int i = 0; i < _tailPieces.size(); i++) {

			if (i == 0) {
				_tailPieces[i].lastCellX = _tailPieces[i].cellX;
				_tailPieces[i].cellX = _snakeHead.lastCellX;
				_tailPieces[i].lastCellY = _tailPieces[i].cellY;
				_tailPieces[i].cellY = _snakeHead.cellY;
				
			}
			else {
				_tailPieces[i].lastCellX = _tailPieces[i].cellX;
				_tailPieces[i].cellX = _tailPieces[i-1].lastCellX;
				_tailPieces[i].lastCellY = _tailPieces[i].cellY;
				_tailPieces[i].cellY = _tailPieces[i-1].lastCellY;

			}

		}
	}

	//Are we now over halfway to a new cell in y-plane?
	if (round(_snakeHead.y) != round(_snakeHead.lastY)) {

		//update last cell value
		_snakeHead.lastCellY = (int)round(_snakeHead.lastY);

		for (int i = 0; i < _tailPieces.size(); i++) {
			if (i == 0) {
				_tailPieces[i].lastCellX = _tailPieces[i].cellX;
				_tailPieces[i].cellX = _snakeHead.cellX;
				_tailPieces[i].lastCellY = _tailPieces[i].cellY;
				_tailPieces[i].cellY = _snakeHead.lastCellY;
			}
			else {
				_tailPieces[i].lastCellX = _tailPieces[i].cellX;
				_tailPieces[i].cellX = _tailPieces[i - 1].lastCellX;
				_tailPieces[i].lastCellY = _tailPieces[i].cellY;
				_tailPieces[i].cellY = _tailPieces[i - 1].lastCellY;
			}
		}
	}

	//Draw the Head
	Draw(_snakeHead.cellX, _snakeHead.cellY, PIXEL_SOLID, _snakeHead.colour);
	//Draw the Tail
	for (unsigned int i = 0; i < _tailPieces.size(); i++) {
		Draw(_tailPieces[i].cellX, _tailPieces[i].cellY, PIXEL_SOLID, _snakeHead.colour);
	}
	//Draw the Fruit
	Draw((int)_fruit.x, (int)_fruit.y, PIXEL_SOLID, _fruit.colour);

		
}

SnakeGame::SnakeGame()
{
}
SnakeGame::~SnakeGame()
{
}