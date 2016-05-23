#include "../include/interaction.h"

extern Mouse *mouse;

void mouseClick(int button, int state, int x, int y)
{
    mouse->click(button, state, x, y);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'q': exit(0);
    }
}
