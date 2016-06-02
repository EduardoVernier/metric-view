#include "../include/ProjectionCanvas.h"

ProjectionCanvas::ProjectionCanvas(Point tl, Point br, EntityTree *et)
{
  setSize(tl, br);
	entityTree = et;
  int shortEdge = ((br.x-tl.x) < (br.y-tl.y))? (br.x-tl.x) : (br.y-tl.y);
  entityTree->normalizeProjection(shortEdge);
}

void ProjectionCanvas::drawCanvas()
{
  glColor3f(1.0f, 1.0f, 1.0f);
	glRectd(top_left.x, top_left.y, bottom_right.x, bottom_right.y);
}
