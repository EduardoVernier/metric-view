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


}



























