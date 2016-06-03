#ifndef MOUSE_H
#define MOUSE_H

#include <iostream>

#define NONE 0;
#define P 1;
#define T 2;

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
    int lastX, lastY, lastCanvas;
    int lastButton, lastState;
};

#endif
