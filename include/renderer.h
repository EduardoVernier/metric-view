#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>
#include <GL/glut.h>
#include <GL/glui.h>

#include "Treemap.h"
#include "TreemapCanvas.h"
#include "ProjectionCanvas.h"
#include "Mouse.h"

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

#endif
