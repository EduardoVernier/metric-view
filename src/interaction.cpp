#include "../include/interaction.h"

extern Mouse *mouse;
extern Treemap *treemap;
extern unsigned winWidth, winHeight;
extern BaseEntity *hover;
extern int mxdown, mydown, mx, my, mclicked;
extern unsigned Rt;
void mouseClick(int button, int state, int x, int y)
{

	int drag[4] = {0,0,0,0};
  switch (mouse->click(button, state, x, y, drag))
	{
		case 1:
			treemap->getTree()->getEntitiesByPositionOnProjection(drag, Rt);
			break;
		case 2:
			treemap->getTree()->getEntitiesByPositionOnTreemap(drag, 1);
			break;
	}
	mouse->updateMouse(x, y);
}

void mousePassive (int x, int y)
{
	mouse->updateMouse(x, y);
	if (x >= 20 + (winWidth-30)/2.0 && y >= 10)
	{
		int nx = x -(20 + (winWidth-30)/2.0);
		int ny = y - 10;
		int drag[4] = {nx,ny,nx,ny};
		treemap->getTree()->getEntitiesByPositionOnTreemap(drag, 0);
		hover = treemap->getTree()->hovered;
	}
	else
	{
		hover = NULL;
	}
}

void mouseUpdate(int x, int y)
{
	mouse->updateMouse(x, y);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'q': exit(0); break;
		case 'z': if (Rt > 0) --Rt; break;
		case 'x': if (1) ++Rt; break; // TODO: Fix this
	}
}
