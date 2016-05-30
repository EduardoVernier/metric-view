#include "../include/interaction.h"

extern Mouse *mouse;
extern Treemap *treemap;

void mouseClick(int button, int state, int x, int y)
{
	int drag[4] = {0,0,0,0};
    if(mouse->click(button, state, x, y, drag))
    	treemap->getTree()->getEntitiesByPosition(drag);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'q': exit(0);
    }
}
