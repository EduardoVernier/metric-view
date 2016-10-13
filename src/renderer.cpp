#include "../include/renderer.h"
#include "../include/GL/glui.h"

// Singletons
shared_ptr<Mouse> mouse = std::make_shared<Mouse>();
shared_ptr<Canvas> pCanvas = nullptr;
shared_ptr<Canvas> tCanvas = nullptr;
shared_ptr<Canvas> sbCanvas = nullptr;
shared_ptr<Canvas> stCanvas = nullptr;
shared_ptr<MetricRank> mRank = nullptr;
unique_ptr<Entity> hover = nullptr; // Drawing of hovering label

// Glut/GLui argument functions
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render();
	glFlush();
}

// Function called when window dimensions change
void reshape(int W, int H)
{
	calculateAnimationStep();
	glShadeModel(GL_SMOOTH);
	glViewport(controller.viewportXOffset, 0, W, H);
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
//	if (controller.windowQueue.size() < 5)
//		controller.windowQueue.push((short)glutGetWindow());
//	else
//	{
//		controller.windowQueue.pop();
//		controller.windowQueue.push((short)glutGetWindow());
//	}
	if ( glutGetWindow() != controller.mainWindow )
		glutSetWindow(controller.mainWindow);
	glutPostRedisplay();
}

// Drawing
void render()
{
	calculateAnimationStep();
	setCanvassesSizes(controller.winWidth, controller.winHeight);
	pCanvas->drawCanvas(Rt, controller.animationStep);

	if(controller.hierarchicalView == TREEMAP)
		tCanvas->drawCanvas(Rt, controller.animationStep);
	else if (controller.hierarchicalView == SUNBURST)
		sbCanvas->drawCanvas(Rt, controller.animationStep);

	if (controller.evolutionView == STREAMGRAPH)
	{
		stCanvas->drawCanvas(Rt, controller.animationStep);
	}
	else if (controller.evolutionView == SPECTROGRAPH)
	{
		SpectrographCanvas::getInstance().drawCanvas(Rt, controller.animationStep);
	}

	drawHoveringLabel();

	if (mouse->state == 0) // If mouse is being clicked
		drawSelectionBox();
	drawRt();
}

// Update objects when window size changes
void setCanvassesSizes(int W, int H)
{
	controller.winWidth = W;
	controller.winHeight = H;

	// Let mouse object know that window has changed size
	mouse->setWindowSize(W, H);

	// Vertices Top-Left and Bottom-Right of Projection canvas
	Point pTL {10, 10};
	Point pBR {10 + (W-30)/2.0, H-10.0};

	// Vertices of Treemap canvas
	Point tTL {20 + (W-30)/2.0, 10};
	Point tBR {W - 10.0, H - 10.0};


	// Instantiate if it's the first call, else just update size
	if (pCanvas == nullptr && tCanvas == nullptr)
	{
		pCanvas = std::make_shared<ProjectionCanvas> (pTL, pBR, entityData);
		tCanvas = std::make_shared<TreemapCanvas> (tTL, tBR, entityData);
		sbCanvas = std::make_shared<SunburstCanvas> (tTL, tBR, entityData);
		stCanvas = std::make_shared<StreamgraphCanvas> (pTL, pBR, entityData);
		SpectrographCanvas::getInstance().init(pTL, pBR, entityData);
		mRank = std::make_shared<MetricRank>(entityData);
	}
	else
	{
		// Define Streamgraph Canvas dimensions if necessary
		if (controller.evolutionView == STREAMGRAPH)
		{
			pBR.y -= controller.streamgraphHeight;
			tBR.y -= controller.streamgraphHeight;
			Point sTL {10, pBR.y + 10}, sBR {W-10.0, H-10.0};
			stCanvas->setSize(sTL, sBR);
		}
		else if (controller.evolutionView == SPECTROGRAPH)
		{
			double spectroHeight = SpectrographCanvas::getInstance().getHeight();
			pBR.y -= spectroHeight;
			tBR.y -= spectroHeight;
			Point sTL {10, pBR.y + 10}, sBR {W-10.0, H-10.0};
			SpectrographCanvas::getInstance().setSize(sTL, sBR);
		}

		pCanvas->setSize(pTL, pBR);
		tCanvas->setSize(tTL, tBR);
		sbCanvas->setSize(tTL, tBR);
	}
}

void drawHoveringLabel()
{
	if (hover!=NULL)
	{
		string s;
		if (hover->isEntity())
			s = hover->getPrefix() + "." + hover->getName();
		else
			s = hover->getName();

		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0, 0, 0, 0.6);

		if (mouse->rawX > 0.7*controller.winWidth)
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
	glColor3d(1, 1, 1);
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
	glColor3d(0, 0, 0);
	int x = controller.winWidth/2 - 30;
	glRasterPos2i(x, 35);
	const unsigned char* s = reinterpret_cast<const unsigned char *>(str.c_str());
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, s);
}


void calculateAnimationStep()
{
	double changeRate = 0.04; // Per render() call
	changeRate *= controller.accelerationRatio; // Set on gui
	if (controller.animationDirection == 0)
		return;
	else
		if (controller.animationDirection == 1)
		{
			if (controller.animationStep - 1 + changeRate < 0.00001) // Safe double comparisson
			{
				controller.animationStep += changeRate;
			}
			else
			{
				controller.animationDirection = 0; // Reset flag
				controller.animationStep = 1.0;
			}
		}
		else
		{
			if (controller.animationStep + 1 - changeRate > 0.00001)
			{
				controller.animationStep -= changeRate;
			}
			else
			{
				controller.animationDirection = 0;
				controller.animationStep = -1.0;
			}
		}
}
