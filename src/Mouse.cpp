#include "../include/Mouse.h"

extern int streamgraphFlag;
extern int streamgraphHeight;

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

	if(streamgraphFlag) // If window layout has streamgraph
	{
		if (_y > 10 && _y < H_- 10 - streamgraphHeight)
		{
			if (_x > 10 && _x < 10 + (W_-30)/2)
			{
				canvas = P;
				x = _x-10;
				y = _y-10 - streamgraphHeight;
			}
			else if (_x > 20 + (W_-30)/2 && _x < W_-10)
			{
				canvas = T;
				x = _x - (20+(W_-30)/2);
				y = _y - 10 - streamgraphHeight;
			}
			else
			{
				canvas = NONE;
				x = 0;
				y = 0;
			}
		}
		else
		{
			canvas = NONE;
			x = 0;
			y = 0;
		}
	}
	else
	{
		if (_y > 10 && _y < H_-10)
		{
			if (_x > 10 && _x < 10 + (W_-30)/2)
			{
				canvas = P;
				x = _x-10;
				y = _y-10;
			}
			else if (_x > 20 + (W_-30)/2 && _x < W_-10)
			{
				canvas = T;
				x = _x - (20+(W_-30)/2);
				y = _y - 10;
			}
			else
			{
				canvas = NONE;
				x = 0;
				y = 0;
			}
		}
		else
		{
			canvas = NONE;
			x = 0;
			y = 0;
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

	if (_y > 10 && _y < H_-10)
	{
		if (_x > 10 && _x < 10 + (W_-30)/2)
		{
			canvas = P;
			x = _x-10;
			y = _y-10;
		}
		else if (_x > 20 + (W_-30)/2 && _x < W_-10)
		{
			canvas = T;
			x = _x - (20+(W_-30)/2);
			y = _y - 10;
		}
		else
		{
			canvas = NONE;
			x = 0;
			y = 0;
		}
	}
	else
	{
		canvas = NONE;
		x = 0;
		y = 0;
	}
}
