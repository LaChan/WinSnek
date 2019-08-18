#include "SnakeGame.h"


bool SnakeGame::OnUserCreate()
{
	_head.x = _head.y = (float)START_X;
	_head.speed = 0.0010f;
	_head.colour = HEAD_COLOUR;
	_head.dir = _head.UP;
	srand((int)time(0)); // create random seed from current epoch time
	_fruit.x = (rand() % ScreenWidth());
	_fruit.y = (rand() % ScreenHeight());
	_fruit.colour = FG_GREEN | BG_DARK_RED;
	_score = 0;
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
	_head.dir = _head.UP;
	if (isDownKeyHeld_) 
	_head.dir = _head.DOWN;
	if (isRightKeyHeld_) 
	_head.dir = _head.RIGHT;
	if (isLeftKeyHeld_) 
	_head.dir = _head.LEFT;

	//check for teleport x
	if (_head.x >= ScreenWidth())
	{
		_head.x = 0;
	}
	else if (_head.x < 0.0)
	{
		_head.x = ScreenWidth();

	}

	//check for teleport y
	if (_head.y >= ScreenHeight())
	{
		_head.y = 0;
	}
	else if (_head.y < 0.0)
	{
		_head.y = ScreenHeight();
	}

	//check for fruit collison
	if (floor(_head.x) == _fruit.x && floor(_head.y) == _fruit.y) 
	{
		_score += 10;
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
	Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, GROUND_COLOUR); //

	DrawString(0, 0, L"Score: " + to_wstring(_score));
	DrawString(0, 1, L"head x: " + to_wstring(_head.x));
	DrawString(0, 2, L"head y: " + to_wstring(_head.y));
	DrawString(0, 3, L"fruit x: " + to_wstring(_fruit.x));
	DrawString(0, 4, L"fruit y: " + to_wstring(_fruit.y));

	//Draw the Snake
	Draw((int)_head.x, (int)_head.y, PIXEL_SOLID, _head.colour);

	//Draw the Fruit
	Draw((int)_fruit.x, (int)_fruit.y, PIXEL_SOLID, _fruit.colour);

	//Move the Snake   
	if (_head.dir == _head.UP)
	{
		_head.y -= _head.speed;
	}
	else if (_head.dir == _head.DOWN)
	{
		_head.y += _head.speed;
	}
	else if (_head.dir == _head.RIGHT)
	{
		_head.x += _head.speed;
	}
	else if (_head.dir = _head.LEFT)
	{
		_head.x -= _head.speed;
	}

}

SnakeGame::SnakeGame()
{
}

SnakeGame::~SnakeGame()
{
}