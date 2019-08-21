#include "SnakeGame.h"


bool SnakeGame::OnUserCreate()
{
	_snake.x = _snake.y = (float)START_X;
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
		_snake.tailPieces += 1;
		_fruit.x = (rand() % ScreenWidth());
		_fruit.y = (rand() % ScreenHeight());
	}

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
	DrawString(0, 1, L"Tail: " + to_wstring(_snake.tailPieces));
	/*
	DrawString(0, 1, L"head x: " + to_wstring(_snake.x));
	DrawString(0, 2, L"head y: " + to_wstring(_snake.y));
	DrawString(0, 3, L"fruit x: " + to_wstring(_fruit.x));
	DrawString(0, 4, L"fruit y: " + to_wstring(_fruit.y));
	*/

	//Draw the Snake
	Draw((int)_snake.x, (int)_snake.y, PIXEL_SOLID, _snake.colour);

	int countX = 0;
	int countY = 0;
	
	//Draw the tail
	for (unsigned int i = 0; i <= _snake.tailPieces; i++)
	{
		
	};
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