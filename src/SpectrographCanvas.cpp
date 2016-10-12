#include "../include/SpectrographCanvas.h"

SpectrographCanvas::SpectrographCanvas() {}

SpectrographCanvas& SpectrographCanvas::getInstance()
{
	static SpectrographCanvas instance {};
 	return instance;
}

void SpectrographCanvas::init(Point tl, Point br, EntityData *et)
{
	setSize(tl, br);
	entityData = et;
}

double SpectrographCanvas::getHeight()
{
	double desiredHeight = entityData->selected.size() * MAX_LINE_HEIGHT;
	return (desiredHeight < 500) ? desiredHeight : 500; // Set 500 as limit for now
}

void SpectrographCanvas::drawCanvas(unsigned Rt, double animationStep)
{
	// Translate everything into frame
	glPushMatrix();
	glTranslated(top_left.x, top_left.y, 0);

	double cellHeight = (getHeight()-10)/sortedSelectedEntities.size();
	double cellWidth = currentWidth/getInstance().entityData->nRevisions;
	int sMetric = controller.streamMetricIndex;

	// Draw cells
	for (unsigned i = 0; i < sortedSelectedEntities.size(); ++i)
	{
		Entity* e = sortedSelectedEntities[i].second;
		for (unsigned j = 0; j < entityData->nRevisions; ++j)
		{
			double normCValue = e->normalizedData[j][sMetric];
			Color c = sequentialColormap(normCValue);
			glColor3d(c.R,c.G,c.B);

			glRectd(j*cellWidth, i*cellHeight,
					(j+1)*cellWidth, (i+1)*cellHeight);

		}
		// Draw hover indication
		if(entityData->hovered != NULL)
			if (e->getName() == entityData->hovered->getName())
		{
			glColor3d(colorHover.R, colorHover.G, colorHover.B);

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
	glColor4f(1,0,0,0.8);
	glBegin(GL_LINE_STRIP);
	double Rtp = (animationStep!=0.0 && animationStep > 0)? Rt+animationStep : (Rt+2) + (animationStep);
	glVertex2d((Rtp-1)*cellWidth, 0);
	glVertex2d((Rtp-1)*cellWidth, currentHeight);
	glVertex2d((Rtp)*cellWidth, currentHeight);
	glVertex2d((Rtp)*cellWidth, 0);
	glVertex2d((Rtp-1)*cellWidth, 0);
	glEnd();

	glPopMatrix();
}

bool pairCompare(const std::pair<double, Entity*>& a, const std::pair<double, Entity*>& b)
{
	return a.first < b.first;
}

void SpectrographCanvas::updateLocalSelectedGroup()
{
	sortedSelectedEntities.clear();

	for (Entity* entity : entityData->selected)
	{
		double metricMean = 0.0;

		for (unsigned i = 0; i < entityData->nRevisions; ++i)
		{
			metricMean += entity->data[i][controller.colorMetricIndex];
		}

		pair<double, Entity*> selectedPair =  std::make_pair(metricMean/entityData->nRevisions, entity);
		sortedSelectedEntities.push_back(selectedPair);
	}

	sort(sortedSelectedEntities.begin(), sortedSelectedEntities.end(), pairCompare);
}

void SpectrographCanvas::getEntitiesOnSpectrograph(int *drag, unsigned click, bool ctrlDown)
{
	double y = drag[1] - top_left.y + 20;
	double cellHeight = (getHeight()-10)/sortedSelectedEntities.size();
	unsigned index = (unsigned) floor(y / cellHeight) - 1;
	if (index >= 0 && index < sortedSelectedEntities.size())
	{
		entityData->hovered = sortedSelectedEntities[index].second;
	}
}
