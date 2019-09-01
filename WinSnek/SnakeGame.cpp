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
	_snakeHead.speed = 0.025f;
	_snakeHead.dir = _snakeHead.UP;
	_snakeHead.colour = HEAD_COLOUR;
	_fruit.x = (rand() % ScreenWidth());
	_fruit.y = (rand() % ScreenHeight());
	_fruit.colour = FG_CYAN | BG_BLACK;
	_fruit.normalColour = FG_CYAN | BG_BLACK;
	_fruit.bonusColour = FG_WHITE | BG_DARK_MAGENTA;
	_score = 0;
	_level = 1;
	
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

			// update snake direction - snake cannot reverse over itself, block opposite for each direction
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

			// check for teleport 
			if (_snakeHead.cellX >= ScreenWidth())
			{
				_snakeHead.x = 0.0;
				_snakeHead.cellX = 0;
			}
			else if (_snakeHead.cellX < 0)
			{
				_snakeHead.x = (float) ScreenWidth()-1;
				_snakeHead.cellX = ScreenWidth() - 1;
			}
			else if (_snakeHead.cellY >= ScreenHeight())
			{
				//using value of 1 to keep off scoreline
				_snakeHead.y = 1.0;
				_snakeHead.cellY = 1;
			}
			else if (_snakeHead.cellY < 1)
			{
				_snakeHead.y = (float) ScreenHeight()-1;
				_snakeHead.cellY = ScreenHeight() - 1;
			}

			// check for fruit collision
			if (_snakeHead.cellX == _fruit.x && _snakeHead.cellY == _fruit.y)
			{
	
				// randomly generate fruit at a new position  
				_fruit.x = (rand() % ScreenWidth());
				_fruit.y = (rand() % ScreenHeight());

				// bonus fruit?
				if (rand() % 6 + 1 == 5) {
					_fruit.colour = _fruit.bonusColour;
				}
				else {
					_fruit.colour = _fruit.normalColour;
				}

				// make sure new position isn't on the scoreline
				if (_fruit.y == 0) {
					_fruit.y += 1;
				}

				// update the score
				if (_fruit.colour == _fruit.bonusColour) {
					_score += (10 * _fruit.bonusMultiplier);
				}
				else {
					_score += 10;
				}

				// should we go up a level?
				if (_score > _level * 100) {
					_level = _level += 1;
					_snakeHead.speed += 0.020;
				}

				// add a new tail piece behind the last, or head if first time
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
			
			// check for snake collision
			for (unsigned int i = 0; i < _tailPieces.size(); i++) {
				SnakeTail& tailPiece = _tailPieces[i];
				if (_snakeHead.cellX == tailPiece.cellX && _snakeHead.cellY == tailPiece.cellY) {
					deadSnake = true;
					waitingForInput = true;
				}
			}

			// let's not redraw too much
			this_thread::sleep_for(chrono::milliseconds(10));

			// render the world
			RenderWorld();

			// requried by engine
			return true;
		}

		while (waitingForInput) {

			// give options on snake death
			isSpaceKeyHeld_ = (m_keys[VK_SPACE].bHeld || m_keys[VK_SPACE].bPressed) ? true : false;
			isEscapeKeyHeld_ = (m_keys[VK_ESCAPE].bHeld || m_keys[VK_ESCAPE].bPressed) ? true : false;
			Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, GROUND_COLOUR);
			DrawString(5, 7, L"GAME OVER");
			DrawString(3, 11, L"<Escape> Exit");
			DrawString(1, 14, L"<Space> Play again");
		
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
   return true;
}

void SnakeGame::RenderWorld()
{
	// Clear the screen by drawing GROUND colour
	Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, GROUND_COLOUR);
	
	// Draw the scoreline
	Fill(0, 0, ScreenWidth(), 1, PIXEL_SOLID, FG_BLACK);
	if (_fruit.colour == _fruit.normalColour) {
		DrawString(0, 0, L"Score: " + to_wstring(_score) + L" Level: " + to_wstring(_level));
	}
	else if (_fruit.colour == _fruit.bonusColour) {
		DrawString(0, 0, L"Score: " + to_wstring(_score) + L" **Bonus**");
	}

	//Moving the Snake, updating values

	//Save previous frame coords
	_snakeHead.lastY = _snakeHead.y;
	_snakeHead.lastX = _snakeHead.x;

	//Actually move the snake
	if (_snakeHead.dir == _snakeHead.UP)
	{
		_snakeHead.y -= _snakeHead.speed;

		for (unsigned int i = 0; i < _tailPieces.size(); i++) {
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

		for (unsigned int i = 0; i < _tailPieces.size(); i++) {
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

		for (unsigned int i = 0; i < _tailPieces.size(); i++) {
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

		for (unsigned int i = 0; i < _tailPieces.size(); i++) {
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
	_snakeHead.cellX = (float) round(_snakeHead.x);
	_snakeHead.cellY = (float) round(_snakeHead.y);

	//Are we now over halfway to a new cell in x-direction?
	if (round(_snakeHead.x) != round(_snakeHead.lastX)) {

		//update last cell value
		_snakeHead.lastCellX = (int)round(_snakeHead.lastX);

		for (unsigned int i = 0; i < _tailPieces.size(); i++) {
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

		for (unsigned int i = 0; i < _tailPieces.size(); i++) {
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
		Draw(tailPiece.cellX, tailPiece.cellY, PIXEL_SOLID, TAIL_COLOUR);
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