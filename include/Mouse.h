#pragma once

#include <memory>
#include <iostream>
#include "TreemapCanvas.h"
#include "ProjectionCanvas.h"
#include "StreamgraphCanvas.h"
#include "Controller.h"

#define NONE 0;
#define P 1;
#define T 2;
#define S 3;

using namespace std;

class Mouse
{
public:
	Mouse();
	int click (int button, int state, int _x, int _y, int *pos);
	void setWindowSize(int W, int H);
	void updateMouse(int _x, int _y);

	int rawLastX, rawLastY;
	int rawX, rawY;
	int W_, H_;
	int x, y, canvas;
	int button, state=1;
	int lastX = 0, lastY = 0, lastCanvas= 0;
	int lastButton, lastState;
};
