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

void Package::sortEntities()
{
	unsigned i = 0, j = 0;
	while (1)
	{
		// If both are comparable
		if (i < childrenVector.size() && j < entityVector.size())
		{
			if (childrenVector[i].sum >= entityVector[j].value)
			{
				// Package is bigger
				sortedEntities.push_back((BaseEntity*) &childrenVector[i]);
				i++;
			}
			else
			{
				// Entity is bigger
				sortedEntities.push_back((BaseEntity*) &entityVector[j]);
				j++;
			}
		}
		else if (i < childrenVector.size() && j == entityVector.size())
		{
			// Only packages left
			for (; i < childrenVector.size(); ++i)
			{
				sortedEntities.push_back((BaseEntity*) &childrenVector[i]);
			}
		}
		else if (i == childrenVector.size() && j < entityVector.size())
		{
			// Only entities left
			for (; j < entityVector.size(); ++j)
			{
				sortedEntities.push_back((BaseEntity*) &entityVector[j]);
			}
		}

		if (i == childrenVector.size() && j == entityVector.size())
			break;
	}
}	
