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

	data.resize(nRevisions);

	for (unsigned i = 0; i < nRevisions; ++i)
	{
		data[i].resize(nAttributes, 0.0f);
	}

	unsigned i = 0;
  while (getline(ss, item, ';'))
  {
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

void Entity::addRevisionData (string dataLine, unsigned rev)
{
	stringstream ss(dataLine);
	string item;
	getline(ss, item, ';'); // flush name

	unsigned i = 0;
	while (getline(ss, item, ';'))
	{
		float value;
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
