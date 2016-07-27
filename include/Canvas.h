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
	double xOff, yOff;
	Color colorHover     = {0.0f,0.8f,0.8f};
	Color colorSelection = {0.0f,0.7f,0.3f};
	int initialWidth, initialHeight;
	int currentWidth, currentHeight;
	int firstMeasurement = 1;

public:
	Point top_left, bottom_right;
	void setSize(Point tl, Point br);
	virtual void drawCanvas(unsigned Rt) = 0;
};
#endif
