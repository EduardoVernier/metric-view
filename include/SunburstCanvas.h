#pragma once

#include <GL/glut.h>

#include "Canvas.h"
#include "EntityTree.h"
#include "Controller.h"

#define PI 3.14159265359

extern Controller controller;
using namespace std;

class SunburstCanvas : public Canvas
{
public:
	SunburstCanvas(Point tl, Point br, EntityTree *et);
	void drawCanvas(unsigned Rt, double animationStep);

private:
	void drawSlice(unsigned Rt, double r0, double theta0, double r1, double theta1);

	EntityTree *entityTree;

};
