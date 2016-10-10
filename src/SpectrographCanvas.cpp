#include "../include/SpectrographCanvas.h"

bool pairCompare(const std::pair<double, Entity*>& a, const std::pair<double, Entity*>& b) {
	return a.first < b.first;
}

SpectrographCanvas::SpectrographCanvas() {}

SpectrographCanvas& SpectrographCanvas::getInstance()
{
	static SpectrographCanvas instance {};
 	return instance;
}

void SpectrographCanvas::init(Point tl, Point br, EntityTree *et)
{
	setSize(tl, br);
	entityTree = et;
	initialWidth  = br.x - tl.x;
	initialHeight = br.y - tl.y;
}

double SpectrographCanvas::getHeight()
{
	double desiredHeight = entityTree->selected.size()*MAX_LINE_HEIGHT;
	return (desiredHeight < 500) ? desiredHeight : 500; // Set 500 as limit for now
}

void SpectrographCanvas::drawCanvas(unsigned Rt, double animationStep)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glRectd(top_left.x, top_left.y, bottom_right.x, bottom_right.y);

	glPushMatrix();
	glTranslated(top_left.x, top_left.y, 0);

	double cellHeight = (getHeight()-10)/sortedSelectedEntities.size();
	double cellWidth = currentWidth/getInstance().entityTree->nRevisions;
	int sMetric = getInstance().entityTree->getStreamMetric();

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

			glRectd(j*cellWidth, i*cellHeight,
					(j+1)*cellWidth, (i+1)*cellHeight);

		}
		// Draw hover indication
		if(entityTree->hovered != NULL)
			if (e->getName() == entityTree->hovered->getName())
		{
			glColor3f(colorHover.R, colorHover.G, colorHover.B);

			glRectd(-2, i*cellHeight,
					-9, (i+1)*cellHeight);
		}
	}

	// Draw separation
	if (getHeight() < 450)
	{
		for (unsigned i = 1; i < sortedSelectedEntities.size(); ++i)
		{
				glColor4f(1,1,1,1);
				glBegin(GL_LINES);
				glVertex2d(0, i*cellHeight);
				glVertex2d(currentWidth, i*cellHeight);
				glEnd();
		}
	}

	// Draw current Rt "window"
	glColor4f(1,1,1,0.8);
	glBegin(GL_LINE_STRIP);
	double Rtp = (animationStep!=0.0 && animationStep > 0)? Rt+animationStep : (Rt+2) + (animationStep);
	glVertex3d((Rtp-1)*cellWidth, 0, 0);
	glVertex3d((Rtp-1)*cellWidth, currentHeight, 0);
	glVertex3d((Rtp)*cellWidth, currentHeight, 0);
	glVertex3d((Rtp)*cellWidth, 0, 0);
	glVertex3d((Rtp-1)*cellWidth, 0, 0);
	glEnd();

	glPopMatrix();
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
	double y = drag[1] - top_left.y + 20;
	double cellHeight = (getHeight()-10)/sortedSelectedEntities.size();
	unsigned index = (unsigned) floor(y / cellHeight);
	if (index >= 0 && index < sortedSelectedEntities.size())
	{
		entityTree->hovered = sortedSelectedEntities[index].second;
	}
}

