#include "../include/Mouse.h"

extern int streamgraphFlag;
extern int streamgraphHeight;
extern TreemapCanvas *tCanvas;
extern ProjectionCanvas *pCanvas;
extern StreamgraphCanvas *sCanvas;

Mouse::Mouse()
{
	x = 0, y = 0, canvas = 0;
	button = 0, state = 1;
}

int Mouse::click (int _button, int _state, int _x, int _y, int *pos)
{
	button = _button;
	state = _state;
	rawX = _x;
	rawY = _y;
	lastCanvas = canvas;

	if (streamgraphFlag == 0)
	{
		if (_x > pCanvas->top_left.x && _x < pCanvas->bottom_right.x &&
				_y > pCanvas->top_left.y && _y < pCanvas->bottom_right.y)
		{
				canvas = P;
				x = _x-pCanvas->top_left.x;
				y = _y-pCanvas->top_left.y;
		}
		else if (_x > tCanvas->top_left.x && _x < tCanvas->bottom_right.x &&
				_y > tCanvas->top_left.y && _y < tCanvas->bottom_right.y)
		{
				canvas = T;
				x = _x-tCanvas->top_left.x;
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
		if (_x > pCanvas->top_left.x && _x < pCanvas->bottom_right.x &&
				_y > pCanvas->top_left.y && _y < pCanvas->bottom_right.y)
		{
				canvas = P;
				x = _x-pCanvas->top_left.x;
				y = _y-pCanvas->top_left.y;
		}
		else if (_x > tCanvas->top_left.x && _x < tCanvas->bottom_right.x &&
				_y > tCanvas->top_left.y && _y < tCanvas->bottom_right.y)
		{
				canvas = T;
				x = _x-tCanvas->top_left.x;
				y = _y-tCanvas->top_left.y;
		}
		else if (_x > sCanvas->top_left.x && _x < sCanvas->bottom_right.x &&
				_y > sCanvas->top_left.y && _y < sCanvas->bottom_right.y)
		{
				canvas = S;
				x = _x-sCanvas->top_left.x;
				y = _y-sCanvas->top_left.y;
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
		rawLastX = _x;
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
		cout << x << " " << y << " " << canvas << endl;
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
	rawX = _x;
	rawY = _y;

	if (streamgraphFlag == 0)
	{
		if (_x > pCanvas->top_left.x && _x < pCanvas->bottom_right.x &&
				_y > pCanvas->top_left.y && _y < pCanvas->bottom_right.y)
		{
				canvas = P;
				x = _x-pCanvas->top_left.x;
				y = _y-pCanvas->top_left.y;
		}
		else if (_x > tCanvas->top_left.x && _x < tCanvas->bottom_right.x &&
				_y > tCanvas->top_left.y && _y < tCanvas->bottom_right.y)
		{
				canvas = T;
				x = _x-tCanvas->top_left.x;
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
		if (_x > pCanvas->top_left.x && _x < pCanvas->bottom_right.x &&
				_y > pCanvas->top_left.y && _y < pCanvas->bottom_right.y)
		{
				canvas = P;
				x = _x-pCanvas->top_left.x;
				y = _y-pCanvas->top_left.y;
		}
		else if (_x > tCanvas->top_left.x && _x < tCanvas->bottom_right.x &&
				_y > tCanvas->top_left.y && _y < tCanvas->bottom_right.y)
		{
				canvas = T;
				x = _x-tCanvas->top_left.x;
				y = _y-tCanvas->top_left.y;
		}
		else if (_x > sCanvas->top_left.x && _x < sCanvas->bottom_right.x &&
				_y > sCanvas->top_left.y && _y < sCanvas->bottom_right.y)
		{
				canvas = S;
				x = _x-sCanvas->top_left.x;
				y = _y-sCanvas->top_left.y;
		}
		else
		{
				canvas = NONE;
				x = -1;
				y = -1;
		}
	}
}
