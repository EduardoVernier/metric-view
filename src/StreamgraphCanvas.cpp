#include "../include/StreamgraphCanvas.h"

StreamgraphCanvas::StreamgraphCanvas(Point tl, Point br, EntityData *et)
{
	setSize(tl, br);
	entityData = et;
}

// Draw Canvas with fixed height
void StreamgraphCanvas::drawCanvas(unsigned Rt, double animationStep)
{
	glColor3d(1.0f, 1.0f, 1.0f);
	glRectd(top_left.x, top_left.y, bottom_right.x, bottom_right.y);

	int sMetric = controller.streamMetricIndex;
	vector<double> normMetricValueSum;

	// Filling vector
	double maxSum = 0;
	for (unsigned i = 0; i < entityData->nRevisions; ++i)
	{
		double sum = 0;
		for (auto e : entityData->selected)
		{
			sum += e->data[i][sMetric];
		}
		normMetricValueSum.push_back(sum);
		if (sum > maxSum)
			maxSum = sum;
	}

	// Normalize based on canvas height and max value
	for (unsigned i = 0; i < normMetricValueSum.size(); ++i)
	{
		normMetricValueSum[i] *= (0.95*controller.streamgraphHeight)/maxSum;
	}

	hoveredIndex = -1;
	// Compute y coords for each line for each member
	double yPos [entityData->nRevisions][entityData->selected.size()+1];
	for (unsigned t = 0; t < entityData->nRevisions; ++t)
	{
		yPos[t][0] = controller.streamgraphHeight/2 - normMetricValueSum[t]/2;
		int i = 1;
		for (auto b = entityData->selected.begin(); b != entityData->selected.end(); ++b)
		{
			yPos[t][i] = yPos[t][i-1] + ((Entity*)(*b))->data[t][sMetric]*((0.90*controller.streamgraphHeight)/maxSum);
			++i;
			if (entityData->hovered != NULL && (*b)->getName() == entityData->hovered->getName())
				hoveredIndex = b - entityData->selected.begin();
		}
	}


	// Draw colored polygons
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	unsigned i = 0;
	for (auto b : entityData->selected)
	{
		glBegin(GL_QUADS);
		for(unsigned t = 0; t < entityData->nRevisions; ++t)
		{
			double value = b->normalizedData[t][sMetric];
			Color c (1,1,1);
			switch (controller.sColormapIndex)
			{
				case 0:
					c = sequentialColormap(value);
					break;
				case 2:
					if (t > 0)
					{
						double pValue = b->normalizedData[t-1][sMetric];
						c = divergentColormap(value-pValue);
					}
					break;
				default:break;
			}

			glColor3d(c.R,c.G,c.B);
			if (t > 0)
			{
				glVertex3d(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1)), top_left.y + yPos[t][i], 0);
				glVertex3d(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1)), top_left.y + yPos[t][i+1], 0);
			}

			glVertex3d(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1)), top_left.y + yPos[t][i+1], 0);
			glVertex3d(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1)), top_left.y + yPos[t][i], 0);
		}
		glEnd();
		++i;
	}

	// Draw lines
	glEnable(GL_LINE_SMOOTH);
	for (unsigned i = 0; i < entityData->selected.size()+1; ++i)
	{
		glColor3f(0,0,0);
		glBegin(GL_LINE_STRIP);
		for (unsigned t = 0; t < entityData->nRevisions; ++t)
		{
			glVertex3f(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1)), top_left.y + yPos[t][i], 0);
		}
		glEnd();
	}

	// Redraw lines with highlight
	if (hoveredIndex != -1)
	{
		glLineWidth(3);
		glColor3f(colorHover.R, colorHover.G, colorHover.B);
		glBegin(GL_LINE_STRIP);
		for (unsigned t = 0; t < entityData->nRevisions; ++t)
		{
			glVertex3f(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1)), top_left.y + yPos[t][hoveredIndex], 0);
		}
		glEnd();
		glBegin(GL_LINE_STRIP);
		for (unsigned t = 0; t < entityData->nRevisions; ++t)
		{
			glVertex3f(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1)), top_left.y + yPos[t][hoveredIndex+1], 0);
		}
		glEnd();
		glLineWidth(1);
	}

	// Paint highlighted
	if (hoveredIndex != -1)
	{
		glColor4f(colorHover.R, colorHover.G, colorHover.B, 0.2);
		glBegin(GL_QUAD_STRIP);
		for (unsigned t = 0; t < entityData->nRevisions; ++t)
		{
			glVertex3f(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1)), top_left.y + yPos[t][hoveredIndex], 0);
			glVertex3f(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1)), top_left.y + yPos[t][hoveredIndex+1], 0);
		}
		glEnd();
	}

	// Draw vertical lines
	glColor4f(0,0,0,0.1);
	for (unsigned t = 0; t < entityData->nRevisions; ++t)
	{
		glBegin(GL_LINE_STRIP);
		glVertex3f(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1)), top_left.y, 0);
		glVertex3f(top_left.x + t*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1)), bottom_right.y, 0);
		glEnd();
	}
	glDisable(GL_BLEND);

	// Draw red line
	glColor4f(1,0,0,0.8);
	glBegin(GL_LINE_STRIP);
	double Rtp = (animationStep!=0.0 && animationStep > 0)? Rt+animationStep : (Rt+2) + (animationStep);
	glVertex3f(top_left.x + (Rtp)*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1)), top_left.y, 0);
	glVertex3f(top_left.x + (Rtp)*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1)), bottom_right.y, 0);
	glEnd();

}


// Highlight hovered entity on Streamgraph
void StreamgraphCanvas::getEntitiesOnStreamgraph(int *drag, unsigned click, bool ctrlDown)
{
	int x = drag[0], y = drag[1];
	int sMetric = controller.streamMetricIndex;
	vector<double> normMetricValueSum;

	// Filling vector
	unsigned maxSum = 0;
	for (unsigned i = 0; i < entityData->nRevisions; ++i)
	{
		double sum = 0;
		for (auto e : entityData->selected)
		{
			sum += ((Entity*)e)->data[i][sMetric];
		}
		normMetricValueSum.push_back(sum);
		if (sum > maxSum)
			maxSum = sum;
	}

	// Normalize based on canvas height and max value
	for (unsigned i = 0; i < normMetricValueSum.size(); ++i)
	{
		normMetricValueSum[i] *= (0.95*controller.streamgraphHeight)/maxSum;
	}

	// Compute y coords for each line for each member
	double yPos [entityData->nRevisions][entityData->selected.size()+1];
	for (unsigned t = 0; t < entityData->nRevisions; ++t)
	{
		yPos[t][0] = controller.streamgraphHeight/2 - normMetricValueSum[t]/2;
		int i = 1;
		for (auto e : entityData->selected)
		{
			yPos[t][i] = yPos[t][i-1] + e->data[t][sMetric]*((0.90*controller.streamgraphHeight)/maxSum);
			++i;
		}
	}

	// Find Ti and Tj
	int Ti = -1, Tj = -1;
	double p;
	for (int t = 0; t < (int) entityData->nRevisions; ++t)
	{
		float t0 = t*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1));
		float t1 = (t+1)*(float(bottom_right.x - top_left.x)/float(entityData->nRevisions-1));
		if (x > t0 && x < t1)
		{
			p = (x - t0)/(t1-t0);
			Ti = t;
			Tj = t+1;
			break;
		}
	}

	// Find hovered entity
	if (Ti != -1)
	{
		for (int i = 0; i < (int)entityData->selected.size(); ++i)
		{
			if ((y > (1-p)*yPos[Ti][i] + p*yPos[Tj][i]) && (y < (1-p)*yPos[Ti][i+1] + p*yPos[Tj][i+1]))
			{
				entityData->hovered = entityData->selected[i];
				break;
			}
		}
	}
}



























