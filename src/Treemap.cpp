#include "../include/Treemap.h"

using namespace std;

Treemap::Treemap(EntityTree et, int _width, int _height)
{

	entityTree = et;
	width = _width;
	height = _height;

	vector<BaseEntity*> data;
	for (vector<BaseEntity*>::iterator b = entityTree.sortedEntities.begin() ; b != entityTree.sortedEntities.end(); ++b)
		if((*b)->getLevel() == 1)
		{
			data.push_back(*b);
		}
	treemapMultidimensional(&data, _width, _height, 0, 0);
}

void Treemap::treemapMultidimensional(vector<BaseEntity*> *data, int width, int height, int xOffset, int yOffset)
{
	// Just for testing
	treemapSingledimensional(data, width, height, xOffset, yOffset);
}


void Treemap::treemapSingledimensional(vector<BaseEntity*> *data, int width, int height, int xOffset, int yOffset)
{
	normalize(data, width * height);
	vector<BaseEntity*> *currentRow = new vector<BaseEntity*>;
	Container container (width, height, xOffset, yOffset);

	squarify(data, currentRow, container);

}


void Treemap::squarify(vector<BaseEntity*> *data, vector<BaseEntity*> *currentRow, Container container)
{

	for (vector<BaseEntity*>::iterator b = currentRow->begin() ; b != currentRow->end(); ++b)
	{
		cout << (*b)->getNormalizedScore() << endl;
	}


	if (data->size() == 0)
	{
		;//
	}

	if (improvesRatio (currentRow, ((*data)[0])->getNormalizedScore(), container.shortestEdge))
	{
		cout << ((*data)[0])->getNormalizedScore() << " added." << endl;
		currentRow->push_back((*data)[0]);
		data->erase(data->begin());
		for (vector<BaseEntity*>::iterator b = data->begin() ; b != data->end(); ++b)
		{
			cout << (*b)->getNormalizedScore() << endl;
		}
		squarify(data, currentRow, container);
	}
	else
	{
		cout << ((*data)[0])->getNormalizedScore() << " wasnt added." << endl;

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

	float currentRatio = fmax(pow(length, 2) * maxCurrent / pow(sumCurrent, 2), pow(sumCurrent, 2) / (pow(length, 2) * minCurrent));
	float newRatio = fmax(pow(length, 2) * maxNew / pow(sumNew, 2), pow(sumNew, 2) / (pow(length, 2) * minNew));

	cout << currentRatio << " >= " << newRatio << endl;
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
	int sum;
	for (vector<BaseEntity*>::iterator b = data->begin() ; b != data->end(); ++b)
		sum += (*b)->getScore();
	return sum;
}

float Treemap::sumNormalizedScores(vector<BaseEntity*> *data)
{
	int sum;
	for (vector<BaseEntity*>::iterator b = data->begin() ; b != data->end(); ++b)
		sum += (*b)->getNormalizedScore();
	return sum;
}
