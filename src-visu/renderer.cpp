#include "../include/renderer.h"

extern int main_window;

// Singletons
Mouse  *mouse = new Mouse();
Canvas *pCanvas = NULL;
Canvas *tCanvas = NULL;

// Glut/GLui argument functions
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render();
    glFlush();
}

void reshape(int w, int h)
{
	  glViewport(0, 0, w, h);
	  glMatrixMode(GL_PROJECTION);
	  glLoadIdentity();
	  // Map abstract coords directly to window coords.
	  glOrtho(0, w, 0, h, -1, 1);
	  glScalef(1, -1, 1);
	  glTranslatef(0, -h, 0);
	  // Update canvasses sizes
	  setCanvassesSizes(w, h);
}

void idle()
{
	if ( glutGetWindow() != main_window )
		glutSetWindow(main_window);
	glutPostRedisplay();
}

// Drawing
void render()
{
	pCanvas->drawCanvas();
	tCanvas->drawCanvas();
}

void setCanvassesSizes(int W, int H)
{
	// Let mouse object know that window has changed size
	mouse->setWindowSize(W, H);

	// Vertices Top-Left and Bottom-Right of Projection canvas
	Point pTL, pBR;
	pTL.x = 10; pTL.y = 10;
	pBR.x = 10 + (W-30)/2; pBR.y = H-10;

	// Vertices of Treemap canvas
	Point tTL, tBR;
	tTL.x = 20 + (W-30)/2; tTL.y = 10;
	tBR.x = W-10; tBR.y = H-10;

	// Instantiate if it's the first call, else just update size
	if (pCanvas == NULL && tCanvas == NULL)
	{
		pCanvas = new Canvas(pTL, pBR);
		tCanvas = new Canvas(tTL, tBR);
	}
	else
	{
		pCanvas->setSize(pTL, pBR);
		tCanvas->setSize(tTL, tBR);
	}

	pCanvas->drawCanvas();
	tCanvas->drawCanvas();
}
