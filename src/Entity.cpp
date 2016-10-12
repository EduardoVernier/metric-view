#include "../include/Entity.h"

Entity::Entity (string csvLine, unsigned nAttributes, unsigned nRevisions)
{
	// Collect all data
	stringstream ss(csvLine);

	// Collect name
	string item;
	getline(ss, item, ';');
	string name = item;
	size_t separator = name.rfind(".");
	id = name.substr(separator+1, name.length());
	if (separator == string::npos)
		prefix = "";
	else
		prefix = name.substr(0,separator);

	// Initialize vectors
	data.resize(nRevisions);
	normalizedData.resize(nRevisions);

	for (unsigned i = 0; i < nRevisions; ++i)
	{
		data[i].resize(nAttributes, 0.0f);
		normalizedData[i].resize(nAttributes, 0.0f);
	}

	unsigned i = 0;
	while (getline(ss, item, ';'))
	{
		if (i >= nAttributes) break;
		istringstream buffer(item);
		buffer >> value;
		data[nRevisions-1][i] = (value);
		++i;
	}

	// CountLine metric as value
	value = data.at(nRevisions-1)[21];

	// Create projection positions vector
	projectionPoints.resize(nRevisions, {0,0});
	normalizedProjectionPoints.resize(nRevisions, {0,0});
}

void Entity::addRevisionData (string dataLine, unsigned rev, int nAttributes)
{
	stringstream ss(dataLine);
	string item;
	getline(ss, item, ';'); // flush name

	int i = 0;
	while (getline(ss, item, ';'))
	{
		if (i >= nAttributes) break;
		istringstream buffer(item);
		buffer >> value;
		data[rev][i] = (value);
		++i;
	}
}

void Entity::addPointAtIndex(Point p, unsigned index)
{
	projectionPoints[index] = p;
}

double Entity::getScore()
{
	double max = 0;
	for (unsigned i = 0; i < data.size(); ++i)
	{
		if (data[i][21] > max)
			max = data[i][21];
	}
	return max;
}
















