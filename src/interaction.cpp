#include "../include/interaction.h"

extern Mouse *mouse;
extern EntityTree *entityTree;
extern unsigned winWidth, winHeight;
extern BaseEntity *hover;
extern int mxdown, mydown, mx, my, mclicked;
extern unsigned Rt;
unsigned ctrlDown = 0;

void mouseClick(int button, int state, int x, int y)
{
	int drag[4] = {0,0,0,0};
  switch (mouse->click(button, state, x, y, drag))
	{
		case 1:
			entityTree->getEntitiesByPositionOnProjection(drag, Rt, 1, ctrlDown);
			break;
		case 2:
			entityTree->getEntitiesByPositionOnTreemap(drag, 1, ctrlDown);
			break;
	}
	mouse->updateMouse(x, y);
}

void mousePassive (int x, int y)
{
	mouse->updateMouse(x, y);
	if (mouse->canvas == 2) // Hovering treemap
	{
		int drag[4] = {mouse->x,mouse->y,mouse->x,mouse->y};
		entityTree->getEntitiesByPositionOnTreemap(drag, 0, ctrlDown);
		hover = entityTree->hovered;
	}
	else if (mouse->canvas == 1) // Hovering projection
	{
		int drag[4] = {mouse->x,mouse->y,mouse->x,mouse->y};
		entityTree->getEntitiesByPositionOnProjection(drag, Rt, 0, ctrlDown);
		hover = entityTree->hovered;
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

void keyboardMod(int key, int x, int y)
{
	if (key == (int)114) // Check for ctrl
		ctrlDown = (ctrlDown == 0)? 1 : 0;
}
