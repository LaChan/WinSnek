#include "SnakeGame.h"

// Runs when you create "SnakeGame" because you extended olcConsoleGameEngineOOP
// This is where you put everything in their starting state/position
bool SnakeGame::OnUserCreate()
{
	_head.x = _head.y = (float)START_X;
	_head.speed = 0.02f;
	_head.colour = HEAD_COLOUR;

	return true;
}

// Runs many times a second in "SnakeGame" because you extended olcConsoleGameEngineOOP
// This is where your game action happens
bool SnakeGame::OnUserUpdate(float fElapsedTime)
{
	// Get input
	isUpKeyHeld_ = (m_keys[VK_UP].bHeld || m_keys[VK_UP].bPressed) ? true : false;
	isDownKeyHeld_ = (m_keys[VK_DOWN].bHeld || m_keys[VK_DOWN].bPressed) ? true : false;
	isRightKeyHeld_ = (m_keys[VK_RIGHT].bHeld || m_keys[VK_RIGHT].bPressed) ? true : false;
	isLeftKeyHeld_ = (m_keys[VK_LEFT].bHeld || m_keys[VK_LEFT].bPressed) ? true : false;

	// update everything
	if (isUpKeyHeld_) _head.y -= _head.speed;
	if (isDownKeyHeld_) _head.y += _head.speed;
	if (isRightKeyHeld_) _head.x += _head.speed;
	if (isLeftKeyHeld_) _head.x -= _head.speed;
	
	//check for teleport
	if (_head.y >= ScreenHeight())
	{
		_head.y = 0;
	}
	else if (_head.y < 0.0)
	{
		_head.y = ScreenHeight() - 1;
	}

	if (_head.x >= ScreenHeight())
	{
		_head.x = 0;
	}
	else if (_head.x < 0.0)
	{
		_head.x = ScreenHeight() - 1;
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

	DrawString(0, 0, L"UP: " + to_wstring(isUpKeyHeld_)); // Write score 3 pixels right, two pixels down (like arrays, start at 0)
	DrawString(0, 1, L"DN: " + to_wstring(isDownKeyHeld_)); // Write score 3 pixels right, two pixels down (like arrays, start at 0)
	// Draw the fly 
	Draw((int)_head.x, (int)_head.y, PIXEL_SOLID, _head.colour);
}

// The constructor and destructor. They always have to be declared, whether you use them or not.
SnakeGame::SnakeGame()
{
}

SnakeGame::~SnakeGame()
{
}