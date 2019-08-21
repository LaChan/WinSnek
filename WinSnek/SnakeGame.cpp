#include "SnakeGame.h"


bool SnakeGame::OnUserCreate()
{
	_snake.x = _snake.y = (float)START_X;
	_snake.cellX = _snake.cellY = START_X;
	_snake.speed = 0.0010f;
	_snake.colour = HEAD_COLOUR;
	_snake.dir = _snake.UP;
	_fruit.x = (rand() % ScreenWidth());
	_fruit.y = (rand() % ScreenHeight());
	_fruit.colour = FG_GREEN | BG_DARK_RED;
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
	_snake.dir = _snake.UP;
	if (isDownKeyHeld_) 
	_snake.dir = _snake.DOWN;
	if (isRightKeyHeld_) 
	_snake.dir = _snake.RIGHT;
	if (isLeftKeyHeld_) 
	_snake.dir = _snake.LEFT;

	//check for teleport x
	if (_snake.x >= ScreenWidth())
	{
		_snake.x = 0;
	}
	else if (_snake.x < 0.0)
	{
		_snake.x = ScreenWidth();

	}

	//check for teleport y
	if (_snake.y >= ScreenHeight())
	{
		_snake.y = 0;
	}
	else if (_snake.y < 0.0)
	{
		_snake.y = ScreenHeight();
	}

	//check for fruit collison
	if (floor(_snake.x) == _fruit.x && floor(_snake.y) == _fruit.y) 
	{
		_score += 10;
		_fruit.x = (rand() % ScreenWidth());
		_fruit.y = (rand() % ScreenHeight());
	}

	//check if we moved a whole x -> cell
	if (floor(_snake.x) != floor(_snake.lastX))
	{
		_snake.cellX = floor(_snake.x);
		_snake.lastCellX = (int)floor(_snake.lastX);
	}

	//check if we moved a whole y -> cell
	if (floor(_snake.y) != floor(_snake.lastY))
	{
		_snake.cellY = floor(_snake.y);
		_snake.lastCellY = (int)floor(_snake.lastY);
	}

	_snake.lastX = _snake.x;
	_snake.lastY = _snake.y;

	// Draw the world
	RenderWorld();
	
	// OnUserUpdate has to return true for the engine to continue
	return true;
}

void SnakeGame::RenderWorld()
{

	// Clear the screen by drawing ground colour
	Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, GROUND_COLOUR); 

	DrawString(0, 0, L"Score: " + to_wstring(_score));
	DrawString(0, 1, L"x: " + to_wstring(_snake.x));
	DrawString(0, 2, L"y: " + to_wstring(_snake.y));
	DrawString(0, 3, L"lastx: " + to_wstring(_snake.lastX));
	DrawString(0, 4, L"lasty: " + to_wstring(_snake.lastY));
	DrawString(0, 5, L"cellX: " + to_wstring(_snake.cellX));
	DrawString(0, 6, L"cellY: " + to_wstring(_snake.cellY));
	DrawString(0, 7, L"lastcellX: " + to_wstring(_snake.lastCellX));
	DrawString(0, 8, L"lastcellY: " + to_wstring(_snake.lastCellY));

	//Draw the Snake
	Draw((int)_snake.x, (int)_snake.y, PIXEL_SOLID, _snake.colour);
	
	//Draw the Fruit
	Draw((int)_fruit.x, (int)_fruit.y, PIXEL_SOLID, _fruit.colour);

	//Move the Snake   
	if (_snake.dir == _snake.UP)
	{
		_snake.y -= _snake.speed;
	}
	else if (_snake.dir == _snake.DOWN)
	{
		_snake.y += _snake.speed;
	}
	else if (_snake.dir == _snake.RIGHT)
	{
		_snake.x += _snake.speed;
	}
	else if (_snake.dir = _snake.LEFT)
	{
		_snake.x -= _snake.speed;
	}

}

SnakeGame::SnakeGame()
{
}

SnakeGame::~SnakeGame()
{
}