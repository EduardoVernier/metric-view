#include "../include/EntityTree.h"

// Places a new entity in it's adequate package
void EntityTree::addEntity(Entity ent)
{
	if (packageVector.empty())
	{
		Package newPackage(ent.prefix);
		newPackage.addEntity(ent);
		packageVector.push_back(newPackage);
	}
	else
	{
		int found = 0;
		for (vector<Package>::iterator pckg = packageVector.begin() ; pckg != packageVector.end(); ++pckg)
		{
			if(pckg->prefix == ent.prefix)
			{
				found = 1;
				pckg->addEntity(ent);
				break;
			}
		}

		if (found == 0)
		{
			Package newPackage(ent.prefix);
			newPackage.addEntity(ent);
			packageVector.push_back(newPackage);
		}
	}
}

// Takes the list of packages and organizes it in tree
void EntityTree::buildHierarchy()
{
	for (vector<Package>::reverse_iterator i = packageVector.rbegin(); i!= packageVector.rend(); ++i)
	{
		for (vector<Package>::reverse_iterator j = i+1; j!= packageVector.rend(); ++j)
		{
			if (i->prefix.find(j->prefix) != string::npos)
			{
				j->addChild(*i);
				packageVector.erase((i+1).base());
				break;
			}
		}
	}
	setHierarchicalLevel(&packageVector[0], 0);
	sortPackages(&packageVector[0]);
	generateSortedEntitiesVector(&packageVector[0]);
}

// Determines every entity tree level
void EntityTree::setHierarchicalLevel(Package *p, int level)
{
	p->setLevel(level);
	if (level + 1 > depth)
		depth = level + 1;

	for (unsigned i = 0; i < p->entityVector.size(); ++i)
	{
		p->entityVector[i].setLevel(level+1);
	}

	for (unsigned i = 0; i < p->childrenVector.size(); ++i)
	{
		setHierarchicalLevel(&p->childrenVector[i], level+1);
	}
}

// Sorts individual packages (parcial order)
void EntityTree::sortPackages(Package *p)
{
	sort (p->childrenVector.begin(), p->childrenVector.end(), compPackages());

	for(unsigned i = 0; i < p->childrenVector.size(); ++i)
	{
		sortPackages(&p->childrenVector[i]);
	}
}

// Creates a sorted vector of BaseEntity references
void EntityTree::generateSortedEntitiesVector(Package *p)
{
	unsigned i = 0, j = 0;
	while (1)
	{
		// If both are comparable
		if (i < p->childrenVector.size() && j < p->entityVector.size())
		{
			if (p->childrenVector[i].sum >= p->entityVector[j].value)
			{
				// Package is bigger
				sortedEntities.push_back((BaseEntity*) &p->childrenVector[i]);
				generateSortedEntitiesVector(&p->childrenVector[i]);
				i++;
			}
			else
			{
				// Entity is bigger
				sortedEntities.push_back((BaseEntity*) &p->entityVector[j]);
				j++;
			}
		}
		else if (i < p->childrenVector.size() && j == p->entityVector.size())
		{
			// Only packages left
			for (; i < p->childrenVector.size(); ++i)
			{
				sortedEntities.push_back((BaseEntity*) &p->childrenVector[i]);
				generateSortedEntitiesVector(&p->childrenVector[i]);
			}
		}
		else if (i == p->childrenVector.size() && j < p->entityVector.size())
		{
			// Only entities left
			for (; j < p->entityVector.size(); ++j)
			{
				sortedEntities.push_back((BaseEntity*) &p->entityVector[j]);
			}
		}

		if (i == p->childrenVector.size() && j == p->entityVector.size())
			break;
	}
}

void EntityTree::printTree()
{
	// Test printing
	for (vector<BaseEntity*>::iterator b = sortedEntities.begin() ; b != sortedEntities.end(); ++b)
	{
		if ((*b)->getName() == "") continue; // Ignore root
		for (int j = 0; j < (*b)->getLevel(); ++j)
			cout << " ";
		cout  << (*b)->getLevel() << " - " << (*b)->getScore() << " " << (*b)->getName() << endl;

		for (int j = 0; j < (*b)->getLevel(); ++j)
			cout << " ";


		for (int i = 0; i < 4; ++i)
			cout << (*b)->getCoord(i) << " ";
		cout << endl;
	}
}
