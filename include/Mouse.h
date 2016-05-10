#ifndef MOUSE_H
#define MOUSE_H

#include <iostream>

#define NONE 0;
#define P 1;
#define T 2;

class Mouse
{
public:
    Mouse();
    int click (int button, int state, int x, int y);
    void setWindowSize(int W, int H);

private:
    int W_, H_;
    int x_, y_, canvas_;
    int button_, state_;
};

#endif
