#pragma once

#include <memory>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <queue>

#include "Treemap.h"
#include "TreemapCanvas.h"
#include "SunburstCanvas.h"
#include "ProjectionCanvas.h"
#include "StreamgraphCanvas.h"
#include "SpectrographCanvas.h"
#include "Mouse.h"
#include "MetricRank.h"
#include "Controller.h"

extern Controller controller;
extern unsigned Rt;
extern EntityData *entityData;

void display();
void reshape(int w, int h);
void mouse_click(int button, int state, int x, int y);
void render();
void setCanvassesSizes(int w, int h);
void idle();
void drawHoveringLabel();
void drawSelectionBox();
void renderHoverString(int x, int y, string str);
void drawRt();
void calculateAnimationStep();
