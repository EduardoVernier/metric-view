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
	cout << sum << " "<<   prefix << endl;

	for (unsigned i = 0; i < childrenVector.size(); ++i)
	{
		for (int j = 0; j <= level; j++)
			cout << "  ";

		childrenVector[i].printPackage(level+1);
		for (unsigned k = 0; k < entityVector.size(); ++k)
		{
			for (int l = 0; l <= level+1; l++)
				cout << "  ";

			cout << entityVector[k].value << ' ' <<  entityVector[k].id << endl;
		}
	}
}
