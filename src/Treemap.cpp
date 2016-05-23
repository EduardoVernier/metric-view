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
	//squarify(data, [], new Container(xoffset, yoffset, width, height), []);
	
}


void Treemap::squarify(vector<BaseEntity*> *data, vector<BaseEntity*> *currentRow, Container container, vector<BaseEntity*> *stack)
{

}

void Treemap::normalize(vector<BaseEntity*> *data, int area)
{
	int sum = sumScores(data);

	for (vector<BaseEntity*>::iterator b = data->begin() ; b != data->end(); ++b)
	{
		float newScore = (*b)->getScore() * ((float)area/sum);
		(*b)->setNormalizedScore(newScore);
		cout << (*b)->getNormalizedScore() << endl;
	}
}


int Treemap::sumScores(vector<BaseEntity*> *data)
{
	int sum;
	for (vector<BaseEntity*>::iterator b = data->begin() ; b != data->end(); ++b)
		sum += (*b)->getScore();
	return sum;
}