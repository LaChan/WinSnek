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

	//check for teleport x
	if (_snakeHead.x >= ScreenWidth())
	{
		_snakeHead.x = 0;
	}
	else if (_snakeHead.x < 0.0)
	{
		_snakeHead.x = ScreenWidth();
	}

	//check for teleport y
	if (_snakeHead.y >= ScreenHeight())
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
		_score += 10;
		_fruit.x = (rand() % ScreenWidth());
		_fruit.y = (rand() % ScreenHeight());
		int endX = 0;
		int endY = 0;
		if (_tailPieces.size() > 0 && (_snakeHead.dir == _snakeHead.UP)) {
			int endX = _tailPieces.back().cellX;
			int endY = _tailPieces.back().cellY - 1;
		}
		else if (_tailPieces.size() > 0 && (_snakeHead.dir == _snakeHead.DOWN)) {
			int endX = _tailPieces.back().cellX;
			int endY = _tailPieces.back().cellY + 1;
		}
		else if (_tailPieces.size() > 0 && (_snakeHead.dir == _snakeHead.RIGHT)) {
			int endX = _tailPieces.back().cellX + 1;
			int endY = _tailPieces.back().cellY;
		}
		else if (_tailPieces.size() > 0 && (_snakeHead.dir == _snakeHead.LEFT)) {
			int endX = _tailPieces.back().cellX -1;
			int endY = _tailPieces.back().cellY;
		}
		else {
			int endX = _snakeHead.cellX;
			int endY = _snakeHead.cellY;
			
		}
		_tailPieces.push_back(SnakeTail(endX, endY));
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
	
	DrawString(0, 2, L"vector 0 cellX" + to_wstring(vec0cell));
	DrawString(0, 2, L"vector 1 cellX" + to_wstring(vec0cell));

	/*
	DrawString(0, 3, L"head last cell x: " + to_wstring(_snakeHead.lastCellX));
	DrawString(0, 4, L"head last cell y: " + to_wstring(_snakeHead.lastCellY));
	*/
	_snakeHead.lastY = _snakeHead.y;
	_snakeHead.lastX = _snakeHead.x;

	//Move the Snake   
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

	_snakeHead.cellX = round(_snakeHead.x);
	_snakeHead.cellY = round(_snakeHead.y);
	
	if (round(_snakeHead.x) != round(_snakeHead.lastX)) {
		
		_snakeHead.lastCellX = (int)round(_snakeHead.lastX);

		for (unsigned int i = 0; i < _tailPieces.size(); i++) {
			SnakeTail& currentPiece = _tailPieces[i];
 			if (i == 0) {
				currentPiece.cellX = _snakeHead.lastCellX;
				currentPiece.cellY = _snakeHead.cellY;
			}
			else {
				currentPiece.cellX = _tailPieces[i-1].lastCellX;
				currentPiece.cellY = _tailPieces[i-1].cellY;
			}
		}
	}

	if (round(_snakeHead.y) != round(_snakeHead.lastY)) {
		_snakeHead.lastCellY = (int)round(_snakeHead.lastY);
		for (unsigned int j = 0; j < _tailPieces.size(); j++) {
			SnakeTail& currentPiece2 = _tailPieces[j];
			if (j == 0) {
				currentPiece2.cellX = _snakeHead.cellX;
				currentPiece2.cellY = _snakeHead.lastCellY;
			}
			else {
				currentPiece2.cellX = _tailPieces[j-1].cellX;
				currentPiece2.cellY = _tailPieces[j-1].lastCellY;
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