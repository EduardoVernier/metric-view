#ifndef INTERACTION_H
#define INTERACTION_H

#include <iostream>
#include <GL/glut.h>
#include <GL/glui.h>
#include <queue>

#include "Mouse.h"
#include "Treemap.h"
#include "TreemapCanvas.h"
#include "ProjectionCanvas.h"
#include "StreamgraphCanvas.h"

extern Mouse *mouse;
extern EntityTree *entityTree;
extern unsigned winWidth, winHeight;
extern BaseEntity *hover;
extern unsigned Rt;
extern int mainWindow;
extern TreemapCanvas *tCanvas;
extern ProjectionCanvas *pCanvas;
extern StreamgraphCanvas *sCanvas;
extern queue<short> windowQueue;

// Button/checkbox/listbox ids
enum UI_IDs
{
	COLORMETRIC_LB = 0,
	RADIUSMETRIC_LB,
	STREAMMETRIC_LB,
	COLORMAP_LB
};

void initializeUI();
void controlCB(int control);
void mouseClick(int button, int state, int x, int y);
void mousePassive(int x, int y);
void mouseUpdate(int x, int y);
void keyboard(unsigned char key, int x, int y);
void keyboardMod(int key, int x, int y);

#endif
