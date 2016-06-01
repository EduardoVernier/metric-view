#include "../include/ProjectionCanvas.h"

ProjectionCanvas::ProjectionCanvas(Point tl, Point br, EntityTree *et)
{
  setSize(tl, br);
	entityTree = et;
}

void ProjectionCanvas::drawCanvas()
{
  glColor3f(1.0f, 0.0f, 0.0f);
	glRectd(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
}
