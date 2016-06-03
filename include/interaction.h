#ifndef INTERACTION_H
#define INTERACTION_H

#include <iostream>
#include <GL/glut.h>
#include <GL/glui.h>

#include "Mouse.h"
#include "Treemap.h"

void mouseClick(int button, int state, int x, int y);
void mousePassive(int x, int y);
void mouseUpdate(int x, int y);
void keyboard(unsigned char key, int x, int y);
void keyboardMod(int key, int x, int y);

#endif
