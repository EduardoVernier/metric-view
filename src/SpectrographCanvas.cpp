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
	return entityTree->selected.size()*MAX_LINE_HEIGHT;
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
	}

	// Draw separation
	for (unsigned i = 0; i < sortedSelectedEntities.size(); ++i)
	{
			glColor4f(1,1,1,1);

			glBegin(GL_LINES);
			glVertex2f(top_left.x, top_left.y + i*cellHeight);
			glVertex2f(bottom_right.x, top_left.y + i*cellHeight);
			glEnd();
	}


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

