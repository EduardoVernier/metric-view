#include "../include/Entity.h"

Entity::Entity (string csvLine, unsigned nRevisions)
{
  // Collect all data
  stringstream ss(csvLine);

  string item;
  getline(ss, item, ';');
  string name = item;
  size_t separator = name.rfind(".");
  id = name.substr(separator+1, name.length());
  if (separator == string::npos)
    prefix = "";
  else
    prefix = name.substr(0,separator);

  while (getline(ss, item, ';'))
  {
    istringstream buffer(item);
    buffer >> value;
    data.push_back(value);
  }

  // CountLine metric as value
  value = data[21];

  // Create projection positions vector
  projectionPoints.resize(nRevisions, {0,0});
  normalizedProjectionPoints.resize(nRevisions, {0,0});
}

void Entity::addPointAtIndex(Point p, unsigned index)
{
  projectionPoints[index] = p;
}
