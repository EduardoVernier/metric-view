#include "../include/interaction.h"

extern Mouse *mouse;
extern Treemap *treemap;
extern unsigned winWidth, winHeight;
extern BaseEntity *hover;
extern int mx, my;
void mouseClick(int button, int state, int x, int y)
{
	mx = x; my = y;
	int drag[4] = {0,0,0,0};
  if(mouse->click(button, state, x, y, drag))
  	treemap->getTree()->getEntitiesByPosition(drag);
}

void mousePassive (int x, int y)
{
	mx = x; my = y;
	if (x >= 20 + (winWidth-30)/2.0 && y >= 10)
	{
		int nx = x -(20 + (winWidth-30)/2.0);
		int ny = y - 10;
		int drag[4] = {nx,ny,nx,ny};
		vector<BaseEntity*> hover_v = treemap->getTree()->getEntitiesByPosition(drag);
		if (!hover_v.empty())
			hover = hover_v[0];
	}
	else
	{
		hover = NULL;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'q': exit(0);
	}
}
