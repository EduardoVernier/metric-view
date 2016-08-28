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
#include "MetricRank.h"
#include "Controller.h"

extern Controller controller;

extern shared_ptr<Mouse> mouse;
extern EntityTree *entityTree;
extern Entity *hover;
extern unsigned Rt;
extern shared_ptr<TreemapCanvas> tCanvas;
extern shared_ptr<ProjectionCanvas> pCanvas;
extern shared_ptr<StreamgraphCanvas> sCanvas;
extern shared_ptr<MetricRank> mRank;

// Button/checkbox/listbox ids
enum UI_IDs
{
	COLORMETRIC_LB = 0,
	RADIUSMETRIC_LB,
	STREAMMETRIC_LB,
	COLORMAP_LB,
	ACCELERATION_LB
};

void initializeUI();
void controlCB(int control);
void mouseClick(int button, int state, int x, int y);
void mousePassive(int x, int y);
void mouseUpdate(int x, int y);
void keyboard(unsigned char key, int x, int y);
void keyboardMod(int key, int x, int y);

#endif
