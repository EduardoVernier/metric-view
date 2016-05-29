#include "../include/TreemapCanvas.h"

TreemapCanvas::TreemapCanvas (Point tl, Point br, EntityTree et)
{
  setSize(tl, br);
  cout << "OOOOOOOOOOOOOOOOOOOOOO" << endl;
  entityTree = et;

  for (vector<BaseEntity*>::iterator it = entityTree.sortedEntities.begin(); it != entityTree.sortedEntities.end(); ++it)
  {
    cout << (*it)->getName() << endl;
  }
}

void TreemapCanvas::drawCanvas()
{
  //vector<BaseEntity*> items = entityTree->sortedEntities;
  //
  // for (vector<BaseEntity*>::iterator it = entityTree->sortedEntities.begin(); it != entityTree->sortedEntities.end(); ++it)
  // {
  //   cout << (*it)->getName() << endl;
  //   if ((*it)->isPackage() == 0)
  //   {
  //     cout << 'c';
  //     drawEntity(*it);
  //   }
  //
  // }
}

void TreemapCanvas::drawEntity(BaseEntity *e)
{
  double x0 = e->getCoord(0)+1;
  double y0 = e->getCoord(1)+1;
  double x1 = e->getCoord(2)-1;
  double y1 = e->getCoord(0)-1;

  glColor3f(1.0f, 1.0f, 1.0f);
  glRectd(x0,y0,x1,y1);
  cout << 'd';
}
