#include "../include/TreemapCanvas.h"

TreemapCanvas::TreemapCanvas (Point tl, Point br, EntityTree *et)
{
  setSize(tl, br);
  entityTree = et;
}

void TreemapCanvas::drawCanvas()
{
  vector<BaseEntity*> items = entityTree->sortedEntities;

  for (vector<BaseEntity*>::iterator it = entityTree->sortedEntities.begin(); it != entityTree->sortedEntities.end(); ++it)
  {
    if ((*it)->isPackage() == 0)
    {
      drawEntity(*it);
    }
    else
    {
      drawPackage(*it);
    }

  }
}

void TreemapCanvas::drawEntity(BaseEntity *e)
{
  double x0 = e->getCoord(0)+1 + xOff;
  double y0 = e->getCoord(1)+1 + yOff;
  double x1 = e->getCoord(2)-1 + xOff;
  double y1 = e->getCoord(3)-1 + yOff;

  glColor3f(1.0f, 0.0f, 0.0f);
  glRectd(x0,y0,x1,y1);
}

void TreemapCanvas::drawPackage(BaseEntity *e)
{
  double x0 = e->getCoord(0) + xOff;
  double y0 = e->getCoord(1) + yOff;
  double x1 = e->getCoord(2) + xOff;
  double y1 = e->getCoord(3) + yOff;

  glLineWidth(2.0f);
  glColor3f(0.0f, 0.0f, 1.0f);
  glBegin(GL_LINE_STRIP);
  glVertex2d(x0,y0);
  glVertex2d(x0,y1);
  glVertex2d(x1,y1);
  glVertex2d(x1,y0);
  glVertex2d(x0,y0);
  glEnd();

}
