#include "../include/Treemap.h"

using namespace std;

Treemap::Treemap(EntityTree *et, double _width, double _height)
{
	entityTree = et;
	width = _width;
	height = _height;

	// Take all first level BaseEntities and start treemapping algorithm
	vector<BaseEntity*> data;
	for (vector<BaseEntity*>::iterator b = entityTree->sortedEntities.begin() ; b != entityTree->sortedEntities.end(); ++b)
		if ((*b)->getLevel() == 1 && (*b)->getScore() != 0)
		{
			data.push_back(*b);
		}
	treemapMultidimensional(&data, (double)_width, (double)_height, 0, 0);
}

// Use recursion to compute single dimentional treemaps from a hierarchical dataset
void Treemap::treemapMultidimensional(vector<BaseEntity*> *data, double width, double height, double xOffset, double yOffset)
{
	// Make a copy of data, as the original is destroyed during treemapSingledimensional computation
	vector<BaseEntity*> dataCopy;
	for (vector<BaseEntity*>::iterator b = data->begin() ; b != data->end(); ++b)
		dataCopy.push_back(*b);

	// Compute single level treemap
	treemapSingledimensional(data, width, height, xOffset, yOffset);

	// For every package from data computed above, compute treemap for their BaseEntity children
	for (vector<BaseEntity*>::iterator b = dataCopy.begin() ; b != dataCopy.end(); ++b)
	{
		if ((*b)->isPackage())
		{
			vector<BaseEntity*> newData;
			((Package*)(*b))->sortEntities();
			for (vector<BaseEntity*>::iterator child = ((Package*)(*b))->sortedEntities.begin(); child != ((Package*)(*b))->sortedEntities.end(); ++child)
			{
				newData.push_back(*child);
			}
			// Calculate new containers dimentions
			double newWidth = (*b)->getCoord(2) - (*b)->getCoord(0);
			double newHeight = (*b)->getCoord(3) - (*b)->getCoord(1);
			double newxOff = (*b)->getCoord(0);
			double newyOff = (*b)->getCoord(1);
			// Recursive call
			treemapMultidimensional(&newData, newWidth, newHeight, newxOff, newyOff);
		}
	}
}

// Wrapper for squarity algorithm
void Treemap::treemapSingledimensional(vector<BaseEntity*> *data, double width, double height, double xOffset, double yOffset)
{
	// Bruls' algorithm takes into account that the data is normalized
	normalize(data, width * height);
	// Initialize current row and container
	vector<BaseEntity*> currentRow;
	Container container (width, height, xOffset, yOffset);
	squarify(data, &currentRow, container);
}

// Implementation of Bruls' Squarified Treemap algorithm
void Treemap::squarify(vector<BaseEntity*> *data, vector<BaseEntity*> *currentRow, Container container)
{
	// If all elements have been placed, save coordinates into BaseEntity objects
	if (data->size() == 0)
	{
		container.saveCoordinates(currentRow, sumNormalizedScores(currentRow));
		return;
	}

	// Test if new element should be included in current row
	if (improvesRatio (currentRow, ((*data)[0])->getNormalizedScore(), container.shortestEdge))
	{
		currentRow->push_back((*data)[0]);
		data->erase(data->begin());
		// Element accepted into row, test for next element
		squarify(data, currentRow, container);
	}
	else
	{
		// New row must be created, subtract area of previous row from container
		Container newContainer = container.cutArea(sumNormalizedScores(currentRow));
		// Save current row coordinates into the objects
		container.saveCoordinates(currentRow, sumNormalizedScores(currentRow));
		// Start algorithm again with reamining elements
		vector<BaseEntity*> newCurrentRow;
		squarify(data, &newCurrentRow, newContainer);
	}
}

// Test if adding a new entity to row improves ratios (get closer to 1)
int Treemap::improvesRatio(vector<BaseEntity*> *currentRow, double nextEntity, int length)
{
	if (currentRow->size() == 0)
	{
		return 1;
	}

	double minCurrent = FLT_MAX, maxCurrent = FLT_MIN;
	double minNew = FLT_MAX, maxNew = FLT_MIN;

	for (unsigned i = 0; i < currentRow->size(); ++i)
	{
		if ((*currentRow)[i]->getNormalizedScore() > maxCurrent)
			maxCurrent = (*currentRow)[i]->getNormalizedScore();

		if ((*currentRow)[i]->getNormalizedScore() < minCurrent)
			minCurrent = (*currentRow)[i]->getNormalizedScore();
	}

	minNew = (nextEntity < minCurrent)? nextEntity : minCurrent;
	maxNew = (nextEntity > maxCurrent)? nextEntity : maxCurrent;

	double sumCurrent = sumNormalizedScores(currentRow);
	double sumNew = sumCurrent + nextEntity;

	double currentRatio = fmax(pow(length, 2) * maxCurrent / pow(sumCurrent, 2), pow(sumCurrent, 2) / (pow(length, 2) * minCurrent));
	double newRatio = fmax(pow(length, 2) * maxNew / pow(sumNew, 2), pow(sumNew, 2) / (pow(length, 2) * minNew));

	//cout << currentRatio << " >= " << newRatio << endl;
	return currentRatio >= newRatio;
}

// Add normalized score to BaseEntities
void Treemap::normalize(vector<BaseEntity*> *data, int area)
{
	int sum = sumScores(data);
	for (vector<BaseEntity*>::iterator b = data->begin() ; b != data->end(); ++b)
	{
		double newScore = (*b)->getScore() * ((double)area / sum);
		(*b)->setNormalizedScore(newScore);
		//cout << (*b)->getName() << " " << (*b)->getScore() << " " << (*b)->getNormalizedScore() << endl;
	}
	//cout << endl;
}

// Sum score of a list of BaseEntities
int Treemap::sumScores(vector<BaseEntity*> *data)
{
	int sum = 0;
	for (vector<BaseEntity*>::iterator b = data->begin() ; b != data->end(); ++b)
		sum += (*b)->getScore();
	return sum;
}

// Sum normalized score of a list of BaseEntities
double Treemap::sumNormalizedScores(vector<BaseEntity*> *data)
{
	int sum = 0;
	for (vector<BaseEntity*>::iterator b = data->begin() ; b != data->end(); ++b)
		sum += (*b)->getNormalizedScore();
	return sum;
}

// When the canvas changes size, all must be recomputed
void Treemap::updateSize (double _width, double _height)
{
	width = _width;
	height = _height;

	vector<BaseEntity*> data;
	for (vector<BaseEntity*>::iterator b = entityTree->sortedEntities.begin(); b != entityTree->sortedEntities.end(); ++b)
	{
		if ((*b)->getLevel() == 1 && (*b)->getScore() != 0)
		{
			data.push_back(*b);
		}
	}
	treemapMultidimensional(&data, (double)_width, (double)_height, 0, 0);
}
