#ifndef MOUSE_H
#define MOUSE_H

class Mouse
{
public:
    Mouse();
    int click (int button, int state, int x, int y);
    int x, y, button, state, canvas;
};

#endif
