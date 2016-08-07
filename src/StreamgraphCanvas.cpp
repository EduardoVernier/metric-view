#include "../include/StreamgraphCanvas.h"

StreamgraphCanvas::StreamgraphCanvas(Point tl, Point br, EntityTree *et)
{
	setSize(tl, br);
	entityTree = et;
	initialWidth  = br.x - tl.x;
	initialHeight = br.y - tl.y;
}

// Draw Canvas with fixed height
void StreamgraphCanvas::drawCanvas(unsigned Rt)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glRectd(top_left.x, top_left.y, bottom_right.x, bottom_right.y);

	int sMetric = entityTree->getStreamMetric();
	vector<double> normMetricValueSum;

	// Filling vector
	unsigned maxSum = 0;
	for(unsigned i = 0; i < entityTree->nRevisions; ++i)
	{
		double sum = 0;
		for (vector<Entity*>::iterator b = entityTree->selected.begin(); b != entityTree->selected.end(); ++b)
		{
			if((*b)->isPackage() == 0 && (*b)->getName() != "")
			{
				sum += ((Entity*)(*b))->data[i][sMetric];
			}
		}
		normMetricValueSum.push_back(sum);
		if (sum > maxSum)
			maxSum = sum;
	}

	// Normalize based on canvas height and max value
	for (unsigned i = 0; i < normMetricValueSum.size(); ++i)
	{
		normMetricValueSum[i] *= (0.95*streamgraphHeight)/maxSum;
	}

	hoveredIndex = -1;
	// Compute y coords for each line for each member
	double yPos [entityTree->nRevisions][entityTree->selected.size()+1];
	for (unsigned t = 0; t < entityTree->nRevisions; ++t)
	{
		yPos[t][0] = streamgraphHeight/2 - normMetricValueSum[t]/2;
		int i = 1;
		for (vector<Entity*>::iterator b = entityTree->selected.begin(); b != entityTree->selected.end(); ++b)
		{
			if((*b)->isPackage() == 0 && (*b)->getName() != "")
			{
				yPos[t][i] = yPos[t][i-1] + ((Entity*)(*b))->data[t][sMetric]*((0.90*streamgraphHeight)/maxSum);
				++i;
				if(entityTree->hovered != NULL && (*b)->getName() == entityTree->hovered->getName())
					hoveredIndex = b - entityTree->selected.begin();
			}
		}
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Draw lines
	for(unsigned i = 0; i < entityTree->selected.size()+1; ++i)
	{
		glColor3f(0,0,0);
		glBegin(GL_LINE_STRIP);
		for (unsigned t = 0; t < entityTree->nRevisions; ++t)
		{
			glVertex3f(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityTree->nRevisions-1)), top_left.y + yPos[t][i], 0);
		}
		glEnd();
	}

	// Redraw lines with highlight
	if(hoveredIndex != -1)
	{
		glLineWidth(3);
		glColor3f(colorHover.R, colorHover.G, colorHover.B);
		glBegin(GL_LINE_STRIP);
		for (unsigned t = 0; t < entityTree->nRevisions; ++t)
		{
			glVertex3f(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityTree->nRevisions-1)), top_left.y + yPos[t][hoveredIndex], 0);
		}
		glEnd();
		glBegin(GL_LINE_STRIP);
		for (unsigned t = 0; t < entityTree->nRevisions; ++t)
		{
			glVertex3f(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityTree->nRevisions-1)), top_left.y + yPos[t][hoveredIndex+1], 0);
		}
		glEnd();
		glLineWidth(1);
	}

	// Paint highlighted
	if(hoveredIndex != -1)
	{
		glColor4f(colorHover.R, colorHover.G, colorHover.B, 0.2);
		glBegin(GL_QUAD_STRIP);
		for (unsigned t = 0; t < entityTree->nRevisions; ++t)
		{
			glVertex3f(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityTree->nRevisions-1)), top_left.y + yPos[t][hoveredIndex], 0);
			glVertex3f(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityTree->nRevisions-1)), top_left.y + yPos[t][hoveredIndex+1], 0);
		}
		glEnd();
	}

	// Draw vertical lines
	for (unsigned t = 0; t < entityTree->nRevisions; ++t)
	{
		if (t == Rt)
			glColor4f(1,0,0,0.5);
		else
			glColor4f(0,0,0,0.1);

		glBegin(GL_LINE_STRIP);
		glVertex3f(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityTree->nRevisions-1)), top_left.y, 0);
		glVertex3f(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityTree->nRevisions-1)), bottom_right.y, 0);
		glEnd();
	}
	glDisable(GL_BLEND);
}


// Highlight hovered entity on Streamgraph
void StreamgraphCanvas::getEntitiesOnStreamgraph(int *drag, unsigned click, unsigned ctrlDown)
{
	int x = drag[0], y = drag[1];
	int sMetric = entityTree->getStreamMetric();
	vector<double> normMetricValueSum;

	// Filling vector
	unsigned maxSum = 0;
	for(unsigned i = 0; i < entityTree->nRevisions; ++i)
	{
		double sum = 0;
		for (vector<Entity*>::iterator b = entityTree->selected.begin(); b != entityTree->selected.end(); ++b)
		{
			if((*b)->isPackage() == 0 && (*b)->getName() != "")
			{
				sum += ((Entity*)(*b))->data[i][sMetric];
			}
		}
		normMetricValueSum.push_back(sum);
		if (sum > maxSum)
			maxSum = sum;
	}

	// Normalize based on canvas height and max value
	for (unsigned i = 0; i < normMetricValueSum.size(); ++i)
	{
		normMetricValueSum[i] *= (0.95*streamgraphHeight)/maxSum;
	}

	// Compute y coords for each line for each member
	double yPos [entityTree->nRevisions][entityTree->selected.size()+1];
	for (unsigned t = 0; t < entityTree->nRevisions; ++t)
	{
		yPos[t][0] = streamgraphHeight/2 - normMetricValueSum[t]/2;
		int i = 1;
		for (vector<Entity*>::iterator b = entityTree->selected.begin(); b != entityTree->selected.end(); ++b)
		{
			if((*b)->isPackage() == 0 && (*b)->getName() != "")
			{
				yPos[t][i] = yPos[t][i-1] + ((Entity*)(*b))->data[t][sMetric]*((0.90*streamgraphHeight)/maxSum);
				++i;
			}
		}
	}

	// Find Ti and Tj
	int Ti = -1, Tj = -1;
	double p;
	for (int t = 0; t < (int) entityTree->nRevisions; ++t)
	{
		float t0 = t*(float(bottom_right.x - top_left.x)/float(entityTree->nRevisions-1));
		float t1 = (t+1)*(float(bottom_right.x - top_left.x)/float(entityTree->nRevisions-1));
		if (x > t0 && x < t1)
		{
			p = (x - t0)/(t1-t0);
			Ti = t;
			Tj = t+1;
			break;
		}
	}

	//cout << Ti << " " << Tj << " " << p << endl;
	// Find hovered entity
	if (Ti != -1)
	{
		for (int i = 0; i < (int)entityTree->selected.size(); ++i)
		{
			if((y > (1-p)*yPos[Ti][i] + p*yPos[Tj][i]) && (y < (1-p)*yPos[Ti][i+1] + p*yPos[Tj][i+1]))
			{
				entityTree->hovered = entityTree->selected[i];
				break;
			}
		}
	}
}



























