#include "../include/Mouse.h"

Mouse::Mouse()
{

}

int Mouse::click (int button, int state, int x, int y)
{
    button_ = button;
    state_ = state;

    if (y > 10 && y < H_-10)
    {
        if (x > 10 && x < 10 + (W_-30)/2)
        {
            canvas_ = P;
            x_ = x-10;
            y_ = y-10;
        }
        else if (x > 20 + (W_-30)/2 && x < W_-10)
        {
            canvas_ = T;
            x_ = x - (20+(W_-30)/2);
            y_ = y - 10;
        }
        else
        {
            canvas_ = NONE;
            x_ = 0;
            y_ = 0;
        }
    }
    else
    {
        canvas_ = NONE;
        x_ = 0;
        y_ = 0;
    }

    printf("%d %d - %d\n", x_,y_, canvas_);
    return 1;
}

void Mouse::setWindowSize(int W, int H)
{
    W_ = W;
    H_ = H;
}
