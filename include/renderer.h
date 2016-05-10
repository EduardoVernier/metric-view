#ifndef RENDERER_H
#define RENDERER_H

#include <stdio.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include "Mouse.h"
#include "Canvas.h"

void display();
void reshape(int w, int h);
void mouse_click(int button, int state, int x, int y);
void render();

//Canvas *p_canvas;
//Canvas *t_canvas;

#endif
