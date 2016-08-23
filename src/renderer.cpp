#include "../include/renderer.h"

extern int winWidth, winHeight;
extern int mainWindow;
extern EntityTree *entityTree;
extern unsigned Rt;
extern int streamgraphFlag;

// Singletons
Mouse *mouse = new Mouse();
Canvas *pCanvas = NULL;
Canvas *tCanvas = NULL;
Canvas *sCanvas = NULL;
MetricRank *mRank = NULL;

BaseEntity* hover = NULL; // Drawing of hovering label
int mxdown, mydown, mx, my, mclicked; // Mouse coordinates
int streamgraphHeight = 250;

queue<short> windowQueue;

int animationDirection = 0;
double animationStep = 1.0;
double accelerationRatio = 1;

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
	calculateAnimationStep();
	glShadeModel(GL_SMOOTH);
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
	// Ugly fix for misclicks
	if (windowQueue.size() < 5)
		windowQueue.push((short)glutGetWindow());
	else
	{
		windowQueue.pop();
		windowQueue.push((short)glutGetWindow());
	}
	glutPostRedisplay();
}

// Drawing
void render()
{
	calculateAnimationStep();
	setCanvassesSizes(winWidth, winHeight);
	pCanvas->drawCanvas(Rt, animationStep);
	tCanvas->drawCanvas(Rt, animationStep);
	if (streamgraphFlag)
		sCanvas->drawCanvas(Rt, animationStep);
	drawHoveringLabel();
	if (mouse->state == 0) // If mouse is being clicked
		drawSelectionBox();
	drawRt();
}

// Update objects when window size changes
void setCanvassesSizes(int W, int H)
{
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

	// Define Streamgraph Canvas dimentions if necessary
	if (streamgraphFlag)
	{
		pBR.y -= streamgraphHeight;
		tBR.y -= streamgraphHeight;
		Point sTL, sBR;
		sTL.x = 10; sTL.y = pBR.y + 10;
		sBR.x = W-10; sBR.y = H-10;
		sCanvas->setSize(sTL, sBR);
	}

	// Instantiate if it's the first call, else just update size
	if (pCanvas == NULL && tCanvas == NULL)
	{
		pCanvas = new ProjectionCanvas(pTL, pBR, entityTree);
		tCanvas = new TreemapCanvas(tTL, tBR, entityTree);
		sCanvas = new StreamgraphCanvas(tTL, tBR, entityTree);
		mRank   = new MetricRank(entityTree);
	}
	else
	{
		pCanvas->setSize(pTL, pBR);
		tCanvas->setSize(tTL, tBR);
	}

	pCanvas->drawCanvas(Rt, animationStep);
	tCanvas->drawCanvas(Rt, animationStep);
	if (streamgraphFlag)
		sCanvas->drawCanvas(Rt, animationStep);
}

void drawHoveringLabel()
{
	if (hover!=NULL)
	{
		string s = ((Entity*)hover)->getPrefix() + "." + hover->getName();
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0, 0, 0, 0.6);

		if (mouse->rawX > 0.7*winWidth)
		{
			glRecti(mouse->rawX, mouse->rawY-10, mouse->rawX - 9*(s.length()), mouse->rawY+2);
			glDisable (GL_BLEND);
			renderHoverString(mouse->rawX - 9*(s.length()), mouse->rawY, s);
		}
		else
		{
			glRecti(mouse->rawX, mouse->rawY-10, mouse->rawX + 9*(s.length()), mouse->rawY+2);
			glDisable (GL_BLEND);
			renderHoverString(mouse->rawX, mouse->rawY, s);
		}
	}
}

void renderHoverString(int x, int y, string str)
{
	glColor3f(1, 1, 1);
	glRasterPos2i(x, y);
	const unsigned char* s = reinterpret_cast<const unsigned char *>(str.c_str());
	glutBitmapString(GLUT_BITMAP_9_BY_15, s);
}

void drawSelectionBox()
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0, 0, 0, 0.4);
	glRecti(mouse->rawLastX, mouse->rawLastY, mouse->rawX, mouse->rawY);
	glDisable (GL_BLEND);
}

void drawRt()
{
	string str = to_string(Rt);
	glColor3f(0, 0, 0);
	int x = winWidth/2 - 30;
	glRasterPos2i(x, 35);
	const unsigned char* s = reinterpret_cast<const unsigned char *>(str.c_str());
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, s);
}


void calculateAnimationStep()
{
	double changeRate = 0.04; // per render() call
	changeRate *= accelerationRatio; // set on gui
	if (animationDirection == 0)
		return;
	else
		if (animationDirection == 1)
		{
			if (animationStep - 1 < 0.00001) // Safe double comparisson
			{
				animationStep += changeRate;
			}
			else
			{
				animationDirection = 0; // Reset flag
				animationStep = 1.0;
			}
		}
		else
		{
			if (animationStep + 1 > 0.00001) // Safe double comparisson
			{
				animationStep -= changeRate;
			}
			else
			{
				animationDirection = 0; // Reset flag
				animationStep = -1.0;
			}
		}
	//cout << animationStep << endl;
}
