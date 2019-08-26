#include "SnakeGame.h"
#include "olcConsoleGameEngineOOP.h"
#include "SnakeHead.h"
#include "SnakeTail.h"
#include "Fruit.h"
#include <vector> 
#include <chrono>


bool SnakeGame::OnUserCreate()
{
	srand((int)time(0)); // create random seed from current time in epoch
	ResetBoard();
	return true;
}

void SnakeGame::ResetBoard()
{
	_snakeHead.x = _snakeHead.y = (float)START_X;
	_snakeHead.cellX = _snakeHead.cellY = START_X;
	_snakeHead.speed = 0.024f;
	_snakeHead.dir = _snakeHead.UP;
	_snakeHead.colour = HEAD_COLOUR;
	_fruit.x = (rand() % ScreenWidth());
	_fruit.y = (rand() % ScreenHeight());
	_fruit.colour = FG_CYAN | BG_WHITE;
	_score = 0;

	// empty the tail vector in case of restart
	_tailPieces.clear();

	//ensure fruit hasn't spawned on scoreline
	if (_fruit.y == 0) {
		_fruit.y += 1;
	}
}

bool SnakeGame::OnUserUpdate(float fElapsedTime)
{
		while (!deadSnake) {

			// input
			isUpKeyHeld_ = (m_keys[VK_UP].bHeld || m_keys[VK_UP].bPressed) ? true : false;
			isDownKeyHeld_ = (m_keys[VK_DOWN].bHeld || m_keys[VK_DOWN].bPressed) ? true : false;
			isRightKeyHeld_ = (m_keys[VK_RIGHT].bHeld || m_keys[VK_RIGHT].bPressed) ? true : false;
			isLeftKeyHeld_ = (m_keys[VK_LEFT].bHeld || m_keys[VK_LEFT].bPressed) ? true : false;

			// update snake direction
			// snake cannot reverse over itself, block opposite for each direction
			if (isUpKeyHeld_) {
				if (_snakeHead.dir != _snakeHead.DOWN) {
					_snakeHead.dir = _snakeHead.UP;
				}
			}
			if (isDownKeyHeld_) {
				if (_snakeHead.dir != _snakeHead.UP) {
					_snakeHead.dir = _snakeHead.DOWN;
				}
			}
			if (isRightKeyHeld_) {
				if (_snakeHead.dir != _snakeHead.LEFT) {
					_snakeHead.dir = _snakeHead.RIGHT;
				}
			}
			if (isLeftKeyHeld_) {
				if (_snakeHead.dir != _snakeHead.RIGHT) {
					_snakeHead.dir = _snakeHead.LEFT;
				}
			}

			//check for teleport 
			if (_snakeHead.cellX >= ScreenWidth())
			{
				_snakeHead.x = 0.0;
				_snakeHead.cellX = 0;
			}
			else if (_snakeHead.cellX < 0)
			{
				_snakeHead.x = ScreenWidth()-1;
				_snakeHead.cellX = ScreenWidth() - 1;
			}
			else if (_snakeHead.cellY >= ScreenHeight())
			{
				_snakeHead.y = 1.0;
				_snakeHead.cellY = 1;
			}
			// like fruit, make sure snake doesn't cross scoreline
			else if (_snakeHead.cellY < 1)
			{
				_snakeHead.y = ScreenHeight()-1;
				_snakeHead.cellY = ScreenHeight() - 1;
			}

			//check for fruit collison
			if (_snakeHead.cellX == _fruit.x && _snakeHead.cellY == _fruit.y)
			{
				//update the score
				_score += 10;

				//randomly generate fruit at a new position  
				_fruit.x = (rand() % ScreenWidth());
				_fruit.y = (rand() % ScreenHeight());
				
				//make sure new position isn't on the scoreline
				if (_fruit.y == 0) {
					_fruit.y += 1;
				}

				//add a new tail piece behind the head first, then last tail piece
				if (_tailPieces.size() == 0) {
					if (_snakeHead.dir == _snakeHead.UP) {
						_tailPieces.push_back(SnakeTail(_snakeHead.cellX, _snakeHead.cellY + 1));
					}
					else if (_snakeHead.dir == _snakeHead.DOWN) {
						_tailPieces.push_back(SnakeTail(_snakeHead.cellX, _snakeHead.cellY - 1));
					}
					else if (_snakeHead.dir == _snakeHead.RIGHT) {
						_tailPieces.push_back(SnakeTail(_snakeHead.cellX - 1, _snakeHead.cellY));
					}
					else if (_snakeHead.dir == _snakeHead.LEFT) {
						_tailPieces.push_back(SnakeTail(_snakeHead.cellX + 1, _snakeHead.cellY));
					}

				}
				else {

					if (_snakeHead.dir == _snakeHead.UP) {
						_tailPieces.push_back(SnakeTail(_tailPieces.back().cellX, _tailPieces.back().cellY + 1));
					}
					else if (_snakeHead.dir == _snakeHead.DOWN) {
						_tailPieces.push_back(SnakeTail(_tailPieces.back().cellX, _tailPieces.back().cellY - 1));
					}
					else if (_snakeHead.dir == _snakeHead.RIGHT) {
						_tailPieces.push_back(SnakeTail(_tailPieces.back().cellX - 1, _tailPieces.back().cellY));
					}
					else if (_snakeHead.dir == _snakeHead.LEFT) {
						_tailPieces.push_back(SnakeTail(_tailPieces.back().cellX + 1, _tailPieces.back().cellY));
					}
				}

			}
			
			//Did snake eat itself?
			for (unsigned int i = 0; i < _tailPieces.size(); i++) {
				SnakeTail& tailPiece = _tailPieces[i];
				if (_snakeHead.cellX == tailPiece.cellX && _snakeHead.cellY == tailPiece.cellY) {
					deadSnake = true;
					waitingForInput = true;
				}
			}

			this_thread::sleep_for(chrono::milliseconds(10));

			// Draw the world
			RenderWorld();

			// OnUserUpdate has to return true for the engine to continue
			return true;
		}

		while (waitingForInput) {

			isSpaceKeyHeld_ = (m_keys[VK_SPACE].bHeld || m_keys[VK_SPACE].bPressed) ? true : false;
			isEscapeKeyHeld_ = (m_keys[VK_ESCAPE].bHeld || m_keys[VK_ESCAPE].bPressed) ? true : false;
			Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, GROUND_COLOUR);
			DrawString(5, 10, L"Game Over");
			DrawString(0, 11, L"Press Escape to Exit");
			DrawString(0, 12, L"Press Space to Restart");

			if (isEscapeKeyHeld_) {
				exit(0);
			}
			else if (isSpaceKeyHeld_) {
				ResetBoard();
				RenderWorld();
				waitingForInput = false;
				deadSnake = false;
			}
			return true;
		}
}

void SnakeGame::RenderWorld()
{
	// Clear the screen by drawing GROUND colour
	Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, GROUND_COLOUR);
	
	// Draw the scoreline
	Fill(0, 0, ScreenWidth(), 1, PIXEL_SOLID, FG_BLACK);
	DrawString(0, 0, L"Score: " + to_wstring(_score));

	//Moving the Snake, updating values

	//Save previous frame coords
	_snakeHead.lastY = _snakeHead.y;
	_snakeHead.lastX = _snakeHead.x;

	//Actually move the snake

	if (_snakeHead.dir == _snakeHead.UP)
	{
		_snakeHead.y -= _snakeHead.speed;

		for (int i = 0; i < _tailPieces.size(); i++) {
			SnakeTail& tailPiece = _tailPieces[i];
			if (i == 0) {
				tailPiece.lastX = tailPiece.x;
				tailPiece.x = _snakeHead.x;
				tailPiece.lastY = tailPiece.y;
				tailPiece.y = _snakeHead.lastY;
			}
			else {
				tailPiece.lastX = tailPiece.x;
				tailPiece.x = _tailPieces[i - 1].lastX;
				tailPiece.lastY = tailPiece.y;
				tailPiece.y = _tailPieces[i - 1].lastY;
			}
		}
	}
	else if (_snakeHead.dir == _snakeHead.DOWN)
	{
		_snakeHead.y += _snakeHead.speed;

		for (int i = 0; i < _tailPieces.size(); i++) {
			SnakeTail& tailPiece = _tailPieces[i];
			if (i == 0) {
				tailPiece.lastX = tailPiece.x;
				tailPiece.x = _snakeHead.x;
				tailPiece.lastY = tailPiece.y;
				tailPiece.y = _snakeHead.lastY;
			}
			else {
				tailPiece.lastX = tailPiece.x;
				tailPiece.x = _tailPieces[i - 1].lastX;
				tailPiece.lastY = tailPiece.y;
				tailPiece.y = _tailPieces[i - 1].lastY;
			}
		}
	}
	else if (_snakeHead.dir == _snakeHead.RIGHT)
	{
		_snakeHead.x += _snakeHead.speed;

		for (int i = 0; i < _tailPieces.size(); i++) {
			SnakeTail& tailPiece = _tailPieces[i];
			if (i == 0) {
				tailPiece.lastX = tailPiece.x;
				tailPiece.x = _snakeHead.lastX;
				tailPiece.lastY = tailPiece.y;
				tailPiece.y = _snakeHead.y;
			}
			else {
				tailPiece.lastX = tailPiece.x;
				tailPiece.x = _tailPieces[i - 1].lastX;
				tailPiece.lastY = tailPiece.y;
				tailPiece.y = _tailPieces[i - 1].lastY;
			}
		}
	}
	else if (_snakeHead.dir = _snakeHead.LEFT)
	{
		_snakeHead.x -= _snakeHead.speed;

		for (int i = 0; i < _tailPieces.size(); i++) {
			SnakeTail& tailPiece = _tailPieces[i];
			if (i == 0) {
				tailPiece.lastX = tailPiece.x;
				tailPiece.x = _snakeHead.lastX;
				tailPiece.lastY = tailPiece.y;
				tailPiece.y = _snakeHead.y;
			}
			else {
				tailPiece.lastX = tailPiece.x;
				tailPiece.x = _tailPieces[i - 1].lastX;
				tailPiece.lastY = tailPiece.y;
				tailPiece.y = _tailPieces[i - 1].lastY;
			}
		}

	}

	//Round float coords to cell
	_snakeHead.cellX = round(_snakeHead.x);
	_snakeHead.cellY = round(_snakeHead.y);

	//Are we now over halfway to a new cell in x-direction?
	if (round(_snakeHead.x) != round(_snakeHead.lastX)) {

		//update last cell value
		_snakeHead.lastCellX = (int)round(_snakeHead.lastX);

		for (int i = 0; i < _tailPieces.size(); i++) {
			SnakeTail& tailPiece = _tailPieces[i];
			if (i == 0) {
				tailPiece.lastCellX = tailPiece.cellX;
				tailPiece.cellX = _snakeHead.lastCellX;
				tailPiece.lastCellY = tailPiece.cellY;
				tailPiece.cellY = _snakeHead.cellY;
			}
			else {
				tailPiece.lastCellX = tailPiece.cellX;
				tailPiece.cellX = _tailPieces[i-1].lastCellX;
				tailPiece.lastCellY = tailPiece.cellY;
				tailPiece.cellY = _tailPieces[i-1].lastCellY;
			}
		}
	} 

	//Are we now over halfway to a new cell in y-direction?
	if (round(_snakeHead.y) != round(_snakeHead.lastY)) {

		//update last cell value
		_snakeHead.lastCellY = (int)round(_snakeHead.lastY);

		for (int i = 0; i < _tailPieces.size(); i++) {
			SnakeTail& tailPiece = _tailPieces[i];
			if (i == 0) {
				tailPiece.lastCellX = tailPiece.cellX;
				tailPiece.cellX = _snakeHead.cellX;
				tailPiece.lastCellY = tailPiece.cellY;
				tailPiece.cellY = _snakeHead.lastCellY;
			}
			else {
				tailPiece.lastCellX = tailPiece.cellX;
				tailPiece.cellX = _tailPieces[i - 1].lastCellX;
				tailPiece.lastCellY = tailPiece.cellY;
				tailPiece.cellY = _tailPieces[i - 1].lastCellY;
			}
		}
	}

	//Draw the World

	//Draw the Head
	Draw(_snakeHead.cellX, _snakeHead.cellY, PIXEL_SOLID, _snakeHead.colour);
	
	//Draw the Tail
	for (unsigned int i = 0; i < _tailPieces.size(); i++) {
		SnakeTail& tailPiece = _tailPieces[i];
		Draw(tailPiece.cellX, tailPiece.cellY, PIXEL_SOLID, _snakeHead.colour);
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