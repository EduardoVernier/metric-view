#include "../include/SpectrographCanvas.h"

bool pairCompare(const std::pair<double, Entity*>& firstElem, const std::pair<double, Entity*>& secondElem) {
	return firstElem.first < secondElem.first;

}

SpectrographCanvas::SpectrographCanvas(Point tl, Point br, EntityTree *et)
{
	setSize(tl, br);
	entityTree = et;
	initialWidth  = br.x - tl.x;
	initialHeight = br.y - tl.y;
}


int SpectrographCanvas::getHeight()
{
	double desiredHeight = entityTree->selected.size()*MAX_LINE_HEIGHT;
	return (desiredHeight < 500)? desiredHeight : 500;
}

void SpectrographCanvas::drawCanvas(unsigned Rt, double animationStep)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glRectd(top_left.x, top_left.y, bottom_right.x, bottom_right.y);

	double cellHeight = ((double) getHeight()-10)/sortedSelectedEntities.size();
	double cellWidth = (double) currentWidth/entityTree->nRevisions;
	int sMetric = entityTree->getStreamMetric();

	// Draw cells
	for (unsigned i = 0; i < sortedSelectedEntities.size(); ++i)
	{
		Entity* e = sortedSelectedEntities[i].second;
		for (unsigned j = 0; j < entityTree->nRevisions; ++j)
		{
			double min = entityTree->getSMMin();
			double max = entityTree->getSMMax();
			double normCValue = (e->data[j][sMetric]-min)/(max-min);
			Color c = sequentialColormap(normCValue);
			glColor3f(c.R,c.G,c.B);

			glRectd(top_left.x + j*cellWidth, top_left.y + i*cellHeight,
							top_left.x + (j+1)*cellWidth, top_left.y + (i+1)*cellHeight);

		}
		// Draw hover indication
		if(entityTree->hovered != NULL)
			if (e->getName() == entityTree->hovered->getName())
		{
			glColor3f(colorHover.R, colorHover.G, colorHover.B);

			glRectd(top_left.x - 8, top_left.y + i*cellHeight,
							top_left.x - 1, top_left.y + (i+1)*cellHeight);

		}
	}

	// Draw separation
	if (getHeight() < 450)
	{
		for (unsigned i = 0; i < sortedSelectedEntities.size(); ++i)
		{
				glColor4f(1,1,1,1);

				glBegin(GL_LINES);
				glVertex2f(top_left.x, top_left.y + i*cellHeight);
				glVertex2f(bottom_right.x, top_left.y + i*cellHeight);
				glEnd();
		}
	}

	// Draw current Rt "window"
	glColor4f(1,1,1,0.8);
	glBegin(GL_LINE_STRIP);
	double Rtp = (animationStep!=0.0 && animationStep > 0)? Rt+animationStep : (Rt+2) + (animationStep);
	glVertex3f(top_left.x + (Rtp-1)*(float(bottom_right.x - top_left.x)/float(entityTree->nRevisions)), top_left.y, 0);
	glVertex3f(top_left.x + (Rtp-1)*(float(bottom_right.x - top_left.x)/float(entityTree->nRevisions)), bottom_right.y, 0);
	glVertex3f(top_left.x + (Rtp)*(float(bottom_right.x - top_left.x)/float(entityTree->nRevisions)), bottom_right.y, 0);
	glVertex3f(top_left.x + (Rtp)*(float(bottom_right.x - top_left.x)/float(entityTree->nRevisions)), top_left.y, 0);
	glVertex3f(top_left.x + (Rtp-1)*(float(bottom_right.x - top_left.x)/float(entityTree->nRevisions)), top_left.y, 0);
	glEnd();

}


void SpectrographCanvas::updateLocalSelectedGroup()
{
	sortedSelectedEntities.clear();

	for (Entity* e : entityTree->selected)
	{
		double metricMean = 0.0;

		for (unsigned i = 0; i < entityTree->nRevisions; ++i)
		{
			metricMean += e->data[i][controller.colorMetricIndex];
		}

		pair<double, Entity*> selectedPair =  std::make_pair(metricMean/entityTree->nRevisions, e);
		sortedSelectedEntities.push_back(selectedPair);
	}

	sort(sortedSelectedEntities.begin(), sortedSelectedEntities.end(), pairCompare);
}

void SpectrographCanvas::getEntitiesOnSpectrograph(int *drag, unsigned click, bool ctrlDown)
{
	int y = drag[1] - top_left.y + 20;
	double cellHeight = ((double) getHeight()-10)/sortedSelectedEntities.size();
	int index = floor(y/cellHeight) - 1;
	if (index >= 0)
	{
		entityTree->hovered = sortedSelectedEntities[index].second;
	}
}
