#include "../include/Package.h"

void Package::addEntity(Entity ent)
{
	sum+=ent.value;
	entityVector.push_back(ent);
	sort(entityVector.begin(), entityVector.end(), compEntities());
}

void Package::addChild(Package child)
{
	sum += child.sum;
	childrenVector.push_back(child);
}

void Package::printPackage(int level)
{
	cout << prefix << endl;

	for (unsigned i = 0; i < childrenVector.size(); ++i)
	{
		for (unsigned j = 0; j <= level; j++) 
			cout << "  ";
		childrenVector[i].printPackage(level+1);
	}	
}