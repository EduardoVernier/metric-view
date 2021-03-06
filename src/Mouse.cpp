#include "../include/Mouse.h"

// Has to be here or the compiler will complain
extern Controller controller;
extern shared_ptr<TreemapCanvas> tCanvas;
extern shared_ptr<ProjectionCanvas> pCanvas;
extern shared_ptr<StreamgraphCanvas> stCanvas;

Mouse::Mouse() {

    x = 0, y = 0, canvas = 0;
    button = 0, state = 1;
}

int Mouse::click(int _button, int _state, int _x, int _y, int *pos) {

    double xMult;
    if (controller.displayControlWindow)
        xMult = ((double) W_ / (double) (W_ - controller.viewportXOffset)) * 0.985; // God that's awful
    else
        xMult = 1;

    button = _button;
    state = _state;
    rawX = (int) ((_x - controller.viewportXOffset) * xMult);
    rawY = _y;
    lastCanvas = canvas;

    ProjectionCanvas &pCanvas = ProjectionCanvas::getInstance();

    if (controller.evolutionView == HIDE) {
        if (rawX > pCanvas.top_left.x && rawX < pCanvas.bottom_right.x &&
            _y > pCanvas.top_left.y && _y < pCanvas.bottom_right.y) {
            canvas = P;
            x = (int) (rawX - pCanvas.top_left.x);
            y = (int) (_y - pCanvas.top_left.y);
        } else if (rawX > tCanvas->top_left.x && rawX < tCanvas->bottom_right.x &&
                   _y > tCanvas->top_left.y && _y < tCanvas->bottom_right.y) {
            canvas = T;
            x = (int) (rawX - tCanvas->top_left.x);
            y = (int) (_y - tCanvas->top_left.y);
        } else {
            canvas = NONE;
            x = -1;
            y = -1;
        }
    } else {
        if (rawX > pCanvas.top_left.x && rawX < pCanvas.bottom_right.x &&
            _y > pCanvas.top_left.y && _y < pCanvas.bottom_right.y) {
            canvas = P;
            x = (int) (rawX - pCanvas.top_left.x);
            y = (int) (_y - pCanvas.top_left.y);
        } else if (rawX > tCanvas->top_left.x && rawX < tCanvas->bottom_right.x &&
                   _y > tCanvas->top_left.y && _y < tCanvas->bottom_right.y) {
            canvas = T;
            x = (int) (rawX - tCanvas->top_left.x);
            y = (int) (_y - tCanvas->top_left.y);
        } else if (rawX > stCanvas->top_left.x && rawX < stCanvas->bottom_right.x &&
                   _y > stCanvas->top_left.y && _y < stCanvas->bottom_right.y) {
            canvas = E;
            x = (int) (rawX - stCanvas->top_left.x);
            y = (int) (_y - stCanvas->top_left.y);
        } else if (rawX > SpectrographCanvas::getInstance().top_left.x &&
                   rawX < SpectrographCanvas::getInstance().bottom_right.x &&
                   _y > SpectrographCanvas::getInstance().top_left.y &&
                   _y < SpectrographCanvas::getInstance().bottom_right.y) {
            canvas = E;
            x = (int) (rawX - stCanvas->top_left.x);
            y = (int) (_y - stCanvas->top_left.y);
        } else {
            canvas = NONE;
            x = -1;
            y = -1;
        }
    }

    if (button == 0 && state == 0) {
        rawLastX = rawX;
        rawLastY = _y;
        lastX = x;
        lastY = y;
    }

    if (button == 0 && state == 1) {
        if (x < lastX) {
            int temp = x;
            x = lastX;
            lastX = temp;
        }
        if (y < lastY) {
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

void Mouse::setWindowSize(int W, int H) {

    W_ = W;
    H_ = H;
}

void Mouse::updateMouse(int _x, int _y) {

    double xMult;
    if (controller.displayControlWindow)
        xMult = ((double) W_ / (double) (W_ - controller.viewportXOffset)) * 0.985; // God that's awful
    else
        xMult = 1;

    rawX = (int) ((_x - controller.viewportXOffset) * xMult);
    rawY = _y;

    ProjectionCanvas &pCanvas = ProjectionCanvas::getInstance();
    SpectrographCanvas &spCanvas = SpectrographCanvas::getInstance();

    if (controller.evolutionView == HIDE) {
        if (rawX > pCanvas.top_left.x && rawX < pCanvas.bottom_right.x &&
            _y > pCanvas.top_left.y && _y < pCanvas.bottom_right.y) {
            canvas = P;
            x = (int) (rawX - pCanvas.top_left.x);
            y = (int) (_y - pCanvas.top_left.y);
        } else if (rawX > tCanvas->top_left.x && rawX < tCanvas->bottom_right.x &&
                   _y > tCanvas->top_left.y && _y < tCanvas->bottom_right.y) {
            canvas = T;
            x = (int) (rawX - tCanvas->top_left.x);
            y = (int) (_y - tCanvas->top_left.y);
        } else {
            canvas = NONE;
            x = -1;
            y = -1;
        }
    } else {
        if (rawX > pCanvas.top_left.x && rawX < pCanvas.bottom_right.x &&
            _y > pCanvas.top_left.y && _y < pCanvas.bottom_right.y) {
            canvas = P;
            x = (int) (rawX - pCanvas.top_left.x);
            y = (int) (_y - pCanvas.top_left.y);
        } else if (rawX > tCanvas->top_left.x && rawX < tCanvas->bottom_right.x &&
                   _y > tCanvas->top_left.y && _y < tCanvas->bottom_right.y) {
            canvas = T;
            x = (int) (rawX - tCanvas->top_left.x);
            y = (int) (_y - tCanvas->top_left.y);
        } else if (rawX > stCanvas->top_left.x && rawX < stCanvas->bottom_right.x &&
                   _y > stCanvas->top_left.y && _y < stCanvas->bottom_right.y) {
            canvas = E;
            x = (int) (rawX - stCanvas->top_left.x);
            y = (int) (_y - stCanvas->top_left.y);
        } else {
            if (rawX > spCanvas.top_left.x &&
                rawX < spCanvas.bottom_right.x &&
                _y > spCanvas.top_left.y &&
                _y < spCanvas.bottom_right.y) {
                canvas = E;
                x = (int) (rawX - stCanvas->top_left.x);
                y = (int) (_y - stCanvas->top_left.y);
            } else {
                canvas = NONE;
                x = -1;
                y = -1;
            }
        }
    }
}
