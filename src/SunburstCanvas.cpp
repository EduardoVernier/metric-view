#include "../include/SunburstCanvas.h"

template <typename T>
bool inBounds(const T& value, const T& low, const T& high)
{	return value >= low && high >= value; }

SunburstCanvas::SunburstCanvas (Point tl, Point br, EntityData *ed)
{
	setSize(tl, br);
	entityData = ed;

	innerRadius = 30;
	unitWidth = (2.0*PI)/(double)entityData->entities.size();
}

void SunburstCanvas::drawCanvas(unsigned Rt, double animationStep)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glRectd(top_left.x, top_left.y, bottom_right.x, bottom_right.y);

	double unitWidth = (2.0*PI)/(double)entityData->entities.size();
	double currentTheta = 0.0;
	for (auto b : entityData->sortedBaseEntities)
	{
		if(b->getName()=="")
			continue;

		drawSlice(b, Rt, currentTheta);

		// Increment theta to avoid overlap
		if (b->isEntity())
			currentTheta += unitWidth;
	}
}

void SunburstCanvas::drawSlice(BaseEntity* b, unsigned Rt, double currentTheta)
{
	// Compute polar coords given BaseEntity level and currentTheta
	double x = xOff + currentWidth/2;
	double y = yOff + currentHeight/2;

	double shortSide = (currentWidth < currentHeight)? currentWidth : currentHeight;
	double r = (shortSide/2 - innerRadius)/(entityData->depth+1);
	double r0 = b->getLevel()*r + innerRadius;
	double r1 = (b->getLevel()+1)*r + innerRadius;

	double theta0 = currentTheta;
	double theta1;
	if(b->isPackage())
		theta1 = currentTheta + ((Package*)b)->numberOfEntities*unitWidth;
	else
		theta1 = currentTheta + unitWidth;

	// Colored polygon
	Color c = getColor(controller.colormapIndex, b, Rt);
	glColor3f(c.R,c.G,c.B);
	glBegin(GL_QUADS);
	for (double t = theta0; t < theta1-0.01; t+=0.001)
	{
		glVertex3f(x + (r0 * cos(t)), y + (r0 * sin(t)), 0);
		glVertex3f(x + (r1 * cos(t)), y + (r1 * sin(t)), 0);
		glVertex3f(x + (r1 * cos(t+0.01)), y + (r1 * sin(t+0.01)), 0);
		glVertex3f(x + (r0 * cos(t+0.01)), y + (r0 * sin(t+0.01)), 0);
	}
	glEnd();


	// Line around it
	if(b == entityData->hovered)
	{
		glColor3f(colorHover.R, colorHover.G, colorHover.B);
		glLineWidth(4.0f);
	}
	else if (find (entityData->selected.begin(), entityData->selected.end(), b) != entityData->selected.end())
	{
		glColor3f(colorSelection.R, colorSelection.G, colorSelection.B);
		glLineWidth(4.0f);
	}
	else
		glColor3f(0,0,0);
	glBegin(GL_LINE_STRIP);

	glVertex3f(x + (r0 * cos(theta0)), y + (r0 * sin(theta0)), 0);

	// Upper arch
	for (double t = theta0; t <= theta1; t+=0.01)
		glVertex3f(x + (r1 * cos(t)), y + (r1 * sin(t)), 0);

	glVertex3f(x + (r0 * cos(theta1)), y + (r0 * sin(theta1)), 0);

	// Lower arch
	for (double t = theta1; t >= theta0; t-=0.01)
		glVertex3f(x + (r0 * cos(t)), y + (r0 * sin(t)), 0);

	glEnd();

	glLineWidth(1.0f);
}


void SunburstCanvas::getEntitiesByPosition(int *drag, unsigned click, bool ctrlDown)
{
	entityData->hovered = NULL;
	if (click && !ctrlDown)
		entityData->selected.clear();

	// Center x and y at middle of the frame
	double normX = drag[0] - currentWidth/2.0;
	double normY = currentHeight/2.0 - drag[1];

	// Convert cartesian coords to polar coords
	double r = sqrt(normX*normX + normY*normY);
	double theta = atan2(normY, normX);
	if (theta < 0)
		theta *= -1;
	else
		theta = 2*PI - theta;

	// Find element using construction algorithm
	double shortSide = (currentWidth < currentHeight)? currentWidth : currentHeight;
	double currentTheta = 0;
	for (auto b : entityData->sortedBaseEntities)
	{
		double ru = (shortSide/2 - innerRadius)/(entityData->depth+1);
		double r0 = b->getLevel()*ru + innerRadius;
		double r1 = (b->getLevel()+1)*ru + innerRadius;

		double theta0 = currentTheta;
		double theta1;
		if(b->isPackage())
			theta1 = currentTheta + ((Package*)b)->numberOfEntities*unitWidth;
		else
		{
			theta1 = currentTheta + unitWidth;
			currentTheta += unitWidth;
		}

		// If package/entity is within bounds
		if (inBounds(theta, theta0, theta1) && inBounds(r, r0, r1))
		{
			if (!click)
				entityData->hovered = b;
			else if (click)
			{
				if (b->isEntity())
				{
					if ((std::find(entityData->selected.begin(), entityData->selected.end(), b))!=entityData->selected.end())
						entityData->selected.erase(std::find(entityData->selected.begin(), entityData->selected.end(), b));
					else
						entityData->selected.push_back((Entity*)b);
				}
				else if (b->isPackage())
				{
					// Recursively push entities to selected vector
					function<void (Package*)> f;
					f = [&, this](Package* p) mutable
					{
						if (p == NULL)
							return;
						else
						{
							for (auto c : p->childrenVector)
								f(&c);

							for (auto e : p->entityVector)
							{
								for (auto ep : entityData->entities)
								{
									if (e.getName() == ep->getName())
									{
										if ((std::find(entityData->selected.begin(), entityData->selected.end(), ep))!=entityData->selected.end())
											entityData->selected.erase(std::find(entityData->selected.begin(), entityData->selected.end(), ep));
										else
											entityData->selected.push_back(ep);
									}
								}
							}
						}
					};
					f((Package*)b);
				}
			}

			return;
		}
	}
}

