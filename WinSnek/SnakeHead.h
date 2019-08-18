#pragma once

class SnakeHead
{

public:
	float x = 0.0f;
	float y = 0.0f;
	float speed = 0.0f;
	int colour = 0;

	// We're using floats for x,y,speed because at the rate the screen updates,
	// maybe 800x a second, moving 1 whole pixel per frame would
	// put us 770 pixels off screen after 1 second
};
