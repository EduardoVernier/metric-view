#include "../include/Mouse.h"

// Has to be here or the compiler will complain
extern Controller controller;
extern shared_ptr<TreemapCanvas> tCanvas;
extern shared_ptr<ProjectionCanvas> pCanvas;
extern shared_ptr<StreamgraphCanvas> stCanvas;

Mouse::Mouse()
{
	x = 0, y = 0, canvas = 0;
	button = 0, state = 1;
}

int Mouse::click (int _button, int _state, int _x, int _y, int *pos)
{
	double xMult;
	if(controller.displayControlWindow)
		xMult = ((double)W_/(double)(W_-controller.viewportXOffset))*0.985; // God that's awful
	else
		xMult = 1;

	button = _button;
	state = _state;
	rawX = (_x - controller.viewportXOffset) * xMult;
	rawY = _y;
	lastCanvas = canvas;

	if (controller.evolutionView == HIDE)
	{
		if (rawX > pCanvas->top_left.x && rawX < pCanvas->bottom_right.x &&
				_y > pCanvas->top_left.y && _y < pCanvas->bottom_right.y)
		{
				canvas = P;
				x = rawX-pCanvas->top_left.x;
				y = _y-pCanvas->top_left.y;
		}
		else if (rawX > tCanvas->top_left.x && rawX < tCanvas->bottom_right.x &&
				_y > tCanvas->top_left.y && _y < tCanvas->bottom_right.y)
		{
				canvas = T;
				x = rawX-tCanvas->top_left.x;
				y = _y-tCanvas->top_left.y;
		}
		else
		{
				canvas = NONE;
				x = -1;
				y = -1;
		}
	}
	else
	{
		if (rawX > pCanvas->top_left.x && rawX < pCanvas->bottom_right.x &&
				_y > pCanvas->top_left.y && _y < pCanvas->bottom_right.y)
		{
				canvas = P;
				x = rawX-pCanvas->top_left.x;
				y = _y-pCanvas->top_left.y;
		}
		else if (rawX > tCanvas->top_left.x && rawX < tCanvas->bottom_right.x &&
				_y > tCanvas->top_left.y && _y < tCanvas->bottom_right.y)
		{
				canvas = T;
				x = rawX-tCanvas->top_left.x;
				y = _y-tCanvas->top_left.y;
		}
		else if (rawX > stCanvas->top_left.x && rawX < stCanvas->bottom_right.x &&
				_y > stCanvas->top_left.y && _y < stCanvas->bottom_right.y)
		{
				canvas = S;
				x = rawX-stCanvas->top_left.x;
				y = _y-stCanvas->top_left.y;
		}
		else
		{
				canvas = NONE;
				x = -1;
				y = -1;
		}
	}

	if (button == 0 && state == 0)
	{
		rawLastX = rawX;
		rawLastY = _y;
		lastX = x;
		lastY = y;
	}

	if (button == 0 && state == 1)
	{
		if (x < lastX)
		{
			int temp = x;
			x = lastX;
			lastX = temp;
		}
		if (y < lastY)
		{
			int temp = y;
			y = lastY;
			lastY = temp;
		}
		pos[0] = lastX;
		pos[1] = lastY;
		pos[2] = x;
		pos[3] = y;
		return canvas;
	}

	return 0;
}

void Mouse::setWindowSize(int W, int H)
{
	W_ = W;
	H_ = H;
}

void Mouse::updateMouse(int _x, int _y)
{

	double xMult;
	if(controller.displayControlWindow)
		xMult = ((double)W_/(double)(W_-controller.viewportXOffset))*0.985; // God that's awful
	else
		xMult = 1;

	rawX = (_x - controller.viewportXOffset)*xMult;
	rawY = _y;

	if (controller.evolutionView == HIDE)
	{
		if (rawX > pCanvas->top_left.x && rawX < pCanvas->bottom_right.x &&
				_y > pCanvas->top_left.y && _y < pCanvas->bottom_right.y)
		{
				canvas = P;
				x = rawX -pCanvas->top_left.x;
				y = _y-pCanvas->top_left.y;
		}
		else if (rawX > tCanvas->top_left.x && rawX < tCanvas->bottom_right.x &&
				_y > tCanvas->top_left.y && _y < tCanvas->bottom_right.y)
		{
				canvas = T;
				x = rawX -tCanvas->top_left.x;
				y = _y-tCanvas->top_left.y;
		}
		else
		{
				canvas = NONE;
				x = -1;
				y = -1;
		}
	}
	else
	{
		if (rawX > pCanvas->top_left.x && rawX < pCanvas->bottom_right.x &&
				_y > pCanvas->top_left.y && _y < pCanvas->bottom_right.y)
		{
				canvas = P;
				x = rawX -pCanvas->top_left.x;
				y = _y-pCanvas->top_left.y;
		}
		else if (rawX > tCanvas->top_left.x && rawX < tCanvas->bottom_right.x &&
				_y > tCanvas->top_left.y && _y < tCanvas->bottom_right.y)
		{
				canvas = T;
				x = rawX -tCanvas->top_left.x;
				y = _y-tCanvas->top_left.y;
		}
		else if (rawX > stCanvas->top_left.x && rawX < stCanvas->bottom_right.x &&
				_y > stCanvas->top_left.y && _y < stCanvas->bottom_right.y)
		{
				canvas = S;
				x = rawX -stCanvas->top_left.x;
				y = _y-stCanvas->top_left.y;
		}
		else
		{
				canvas = NONE;
				x = -1;
				y = -1;
		}
	}
}
