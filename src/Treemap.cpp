#include "../include/Treemap.h"

using namespace std;

Treemap::Treemap(EntityTree et, int _width, int _height)
{

	entityTree = et;
	width = _width;
	height = _height;

	vector<BaseEntity*> data;
	for (vector<BaseEntity*>::iterator b = entityTree.sortedEntities.begin() ; b != entityTree.sortedEntities.end(); ++b)
		if((*b)->getLevel() == 1 && (*b)->getScore() != 0)
		{
			data.push_back(*b);
		}
	treemapMultidimensional(&data, (float)_width, (float)_height, 0, 0);
}

void Treemap::treemapMultidimensional(vector<BaseEntity*> *data, float width, float height, float xOffset, float yOffset)
{
	vector<BaseEntity*> dataCopy;
	for (vector<BaseEntity*>::iterator b = data->begin() ; b != data->end(); ++b)
		dataCopy.push_back(*b);

	treemapSingledimensional(data, width, height, xOffset, yOffset);

	for (vector<BaseEntity*>::iterator b = dataCopy.begin() ; b != dataCopy.end(); ++b)
	{
		if ((*b)->isPackage())
		{
			vector<BaseEntity*> newData;
			((Package*)(*b))->sortEntities();
			for (vector<BaseEntity*>::iterator child = ((Package*)(*b))->sortedEntities.begin() ; child != ((Package*)(*b))->sortedEntities.end(); ++child)
			{
				newData.push_back(*child);
			}

			float newWidth = (*b)->coords[2] - (*b)->coords[0];
			float newHeight = (*b)->coords[3] - (*b)->coords[1];
			float newxOff = (*b)->coords[0];
			float newyOff = (*b)->coords[1]; 
			treemapMultidimensional(&newData, newWidth, newHeight, newxOff, newyOff);
		}
		
	}

}


void Treemap::treemapSingledimensional(vector<BaseEntity*> *data, float width, float height, float xOffset, float yOffset)
{
	normalize(data, width * height);
	vector<BaseEntity*> *currentRow = new vector<BaseEntity*>;
	Container container (width, height, xOffset, yOffset);

	squarify(data, currentRow, container);

}


void Treemap::squarify(vector<BaseEntity*> *data, vector<BaseEntity*> *currentRow, Container container)
{

	if (data->size() == 0)
	{
		container.saveCoordinates(currentRow, sumNormalizedScores(currentRow));
		return;
	}

	if (improvesRatio (currentRow, ((*data)[0])->getNormalizedScore(), container.shortestEdge))
	{
		currentRow->push_back((*data)[0]);
		data->erase(data->begin());

		squarify(data, currentRow, container);
	}
	else
	{
		Container newContainer = container.cutArea(sumNormalizedScores(currentRow));

		// Save current row coordinates into the objects
		container.saveCoordinates(currentRow, sumNormalizedScores(currentRow));

		vector<BaseEntity*> *newCurrentRow = new vector<BaseEntity*>;
		squarify(data, newCurrentRow, newContainer);
	}
}

int Treemap::improvesRatio(vector<BaseEntity*> *currentRow, float nextEntity, int length)
{
	if (currentRow->size() == 0)
	{
		return 1;
	}

	float minCurrent = FLT_MAX, maxCurrent = FLT_MIN;
	float minNew = FLT_MAX, maxNew = FLT_MIN;

	for (unsigned i = 0; i < currentRow->size(); ++i)
	{
		if ((*currentRow)[i]->getNormalizedScore() > maxCurrent)
			maxCurrent = (*currentRow)[i]->getNormalizedScore();

		if((*currentRow)[i]->getNormalizedScore() < minCurrent)
			minCurrent = (*currentRow)[i]->getNormalizedScore();
	}
	
	minNew = (nextEntity < minCurrent)? nextEntity : minCurrent;
	maxNew = (nextEntity > maxCurrent)? nextEntity : maxCurrent;

	float sumCurrent = sumNormalizedScores(currentRow);
	float sumNew = sumCurrent + nextEntity;

	double currentRatio = fmax(pow(length, 2) * maxCurrent / pow(sumCurrent, 2), pow(sumCurrent, 2) / (pow(length, 2) * minCurrent));
	double newRatio = fmax(pow(length, 2) * maxNew / pow(sumNew, 2), pow(sumNew, 2) / (pow(length, 2) * minNew));

	//cout << currentRatio << " >= " << newRatio << endl;
	return currentRatio >= newRatio;
}

void Treemap::normalize(vector<BaseEntity*> *data, int area)
{
	int sum = sumScores(data);

	for (vector<BaseEntity*>::iterator b = data->begin() ; b != data->end(); ++b)
	{
		float newScore = (*b)->getScore() * ((float)area / sum);
		(*b)->setNormalizedScore(newScore);
		// cout << (*b)->getNormalizedScore() << endl;
	}
}

int Treemap::sumScores(vector<BaseEntity*> *data)
{
	int sum = 0;
	for (vector<BaseEntity*>::iterator b = data->begin() ; b != data->end(); ++b)
		sum += (*b)->getScore();
	return sum;
}

float Treemap::sumNormalizedScores(vector<BaseEntity*> *data)
{
	int sum = 0;
	for (vector<BaseEntity*>::iterator b = data->begin() ; b != data->end(); ++b)
		sum += (*b)->getNormalizedScore();
	return sum;
}
