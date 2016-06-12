#ifndef CANVAS_H
#define CANVAS_H

//#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/glui.h>

#include "Point.h"
#include "Color.h"

class Canvas
{
protected:
	Point top_left, bottom_right;
	double xOff, yOff;
	Color colorHover     = {0.0f,0.8f,0.8f};
	Color colorSelection = {0.0f,0.7f,0.3f};

public:
	void setSize(Point tl, Point br);
	virtual void drawCanvas(unsigned Rt) = 0;
};
#endif
