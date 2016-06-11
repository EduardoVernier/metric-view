#include "../include/renderer.h"

extern int winWidth, winHeight;
extern int mainWindow;

// Singletons
Mouse  *mouse = new Mouse();
Canvas *pCanvas = NULL;
Canvas *tCanvas = NULL;
extern EntityTree *entityTree;
extern unsigned Rt;

BaseEntity* hover = NULL; // Drawing of hovering label
int mxdown, mydown, mx, my, mclicked; // Mouse coordinates

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
	if ( glutGetWindow() != mainWindow )
		glutSetWindow(mainWindow);
	glutPostRedisplay();
}

// Drawing
void render()
{
	pCanvas->drawCanvas(Rt);
	tCanvas->drawCanvas(0);
	drawHoveringLabel();
	if(mouse->state == 0) // If mouse is being clicked
		drawSelectionBox();
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
		pCanvas = new ProjectionCanvas(pTL, pBR, entityTree);
		tCanvas = new TreemapCanvas(tTL, tBR, entityTree);
	}
	else
	{
		pCanvas->setSize(pTL, pBR);
		tCanvas->setSize(tTL, tBR);
	}

	pCanvas->drawCanvas(Rt);
	tCanvas->drawCanvas(0);
}

void drawHoveringLabel()
{
		if (hover!=NULL)
		{
			string s = ((Entity*)hover)->getPrefix() + "." + hover->getName();
			glEnable (GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(0, 0, 0, 0.6);
			if (mouse->canvas == 2)
			{
				glRecti(mouse->rawX, mouse->rawY-10, mouse->rawX - 9*(s.length()), mouse->rawY+2);
				glDisable (GL_BLEND);
				((TreemapCanvas*)tCanvas)->renderString(mouse->rawX - 9*(s.length()), mouse->rawY, s, {1.0f,1.0f,1.0f});
			}
			else if (mouse->canvas == 1)
			{
				glRecti(mouse->rawX, mouse->rawY-10, mouse->rawX + 9*(s.length()), mouse->rawY+2);
				glDisable (GL_BLEND);
				((TreemapCanvas*)tCanvas)->renderString(mouse->rawX, mouse->rawY, s, {1.0f,1.0f,1.0f});
			}
		}
}

void drawSelectionBox()
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0, 0, 0, 0.3);
	glRecti(mouse->rawLastX, mouse->rawLastY, mouse->rawX, mouse->rawY);
	glDisable (GL_BLEND);
}
