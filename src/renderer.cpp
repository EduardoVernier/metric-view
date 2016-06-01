#include "../include/renderer.h"

extern int winWidth, winHeight;
extern int main_window;

// Singletons
Mouse  *mouse = new Mouse();
Canvas *pCanvas = NULL;
Canvas *tCanvas = NULL;
extern Treemap *treemap;

BaseEntity* hover = NULL; // Drawing of hovering label
int mx, my; // Mouse coordinates

// Glut/GLui argument functions
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render();
	glFlush();
}

// Function called when window dimentions change
void reshape(int W, int H)
{
	glViewport(0, 0, W, H);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Map abstract coords directly to window coords.
	glOrtho(0, W, 0, H, -1, 1);
	glScalef(1, -1, 1);
	glTranslatef(0, -H, 0);
	// Update canvasses sizes
	setCanvassesSizes(W, H);
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
	drawHoveringLabel();
}

// Update objects when window size changes
void setCanvassesSizes(int W, int H)
{
	cout << "Width: " << W << " Height: " << H << endl;
	winWidth = W;
	winHeight = H;


	// Let mouse object know that window has changed size
	mouse->setWindowSize(W, H);

	// Vertices Top-Left and Bottom-Right of Projection canvas
	Point pTL, pBR;
	pTL.x = 10; pTL.y = 10;
	pBR.x = 10 + (W-30)/2; pBR.y = H-10;

	// Vertices of Treemap canvas
	Point tTL, tBR;
	tTL.x = 20 + (W-30)/2.0; tTL.y = 10;
	tBR.x = W-10; tBR.y = H-10;

	// TODO: Fix update in window size
	// Update treemap given new width and height
	// treemap->updateSize((W-30)/2.0, H-20);

	// Instantiate if it's the first call, else just update size
	if (pCanvas == NULL && tCanvas == NULL)
	{
		pCanvas = new ProjectionCanvas(pTL, pBR, treemap->getTree());
		tCanvas = new TreemapCanvas(tTL, tBR, treemap->getTree());
	}
	else
	{
		pCanvas->setSize(pTL, pBR);
		tCanvas->setSize(tTL, tBR);
	}

	pCanvas->drawCanvas();
	tCanvas->drawCanvas();
}

void drawHoveringLabel()
{
		if (hover!=NULL)
		{
			string s = ((Entity*)hover)->getPrefix() + "." + hover->getName();
			glEnable (GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(0, 0, 0, 0.6);
			glRecti(mx, my-10, mx - 9*(s.length()), my+2);
			glDisable (GL_BLEND);

			((TreemapCanvas*)tCanvas)->renderString(mx - 9*(s.length()), my, s, {1.0f,1.0f,1.0f});
		}

}
