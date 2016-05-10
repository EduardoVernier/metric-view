#include "../include/renderer.h"

// Singletons
Mouse *mouse = new Mouse();


void render()
{
}

void display()
{
	printf("hi");
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	render();
	glFlush();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
	printf("ho");
	glViewport(0.0f, 0.0f, (GLfloat)w, (GLfloat)h);
	glClearColor(0,0,0,0);
	glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
	//setCanvassesSizes(w, h);

}


// Interaction
void mouse_click(int button, int state, int x, int y)
{
    mouse->click(button, state, x, y);
}
