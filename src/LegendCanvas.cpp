#include "../include/LegendCanvas.h"

LegendCanvas::LegendCanvas() {}

LegendCanvas& LegendCanvas::getInstance()
{
	static LegendCanvas instance {};
	return instance;
}

void LegendCanvas::init(Point tl, Point br, EntityData *ed)
{
	setSize(tl, br);
	entityData = ed;
}

void LegendCanvas::drawCanvas(unsigned Rt, double animationStep)
{
	glColor3d(1.0f, 1.0f, 1.0f);
	glRectd(top_left.x, top_left.y, bottom_right.x, bottom_right.y);



}
