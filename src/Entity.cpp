#include "../include/Entity.h"

Entity::Entity (string csvLine, unsigned nRevisions)
{
  // Collect all data
  stringstream ss(csvLine);
  string item;
  while (getline(ss, item, ';'))
  {
      data.push_back(item);
  }
  string name = data[0];
  size_t separator = name.rfind(".");
  id = name.substr(separator+1, name.length());
  if (separator == string::npos)
    prefix = "";
  else
    prefix = name.substr(0,separator);

  // CountLine metric as value
  istringstream buffer(data[22]);
  buffer >> value;

  // Create projection positions vector
  projectionPoints.resize(nRevisions, {0,0});
}

void Entity::addPointAtIndex(Point p, unsigned index)
{
  projectionPoints[index] = p;
}
