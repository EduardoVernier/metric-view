#include "../include/EntityTree.h"

// Places a new entity in it's adequate package
void EntityTree::addEntity(Entity ent)
{
	if (packageVector.empty())
	{
		Package newPackage(ent.getPrefix());
		newPackage.addEntity(ent);
		packageVector.push_back(newPackage);
	}
	else
	{
		int found = 0;
		for (vector<Package>::iterator pckg = packageVector.begin() ; pckg != packageVector.end(); ++pckg)
		{
			if(pckg->getName() == ent.getPrefix())
			{
				found = 1;
				pckg->addEntity(ent);
				break;
			}
		}

		if (found == 0)
		{
			Package newPackage(ent.getPrefix());
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
			if (i->getName().find(j->getName()) != string::npos)
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
	setMinMax();
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

// Set min and max for use on colormapping
void EntityTree::setMinMax()
{
	treeMin = FLT_MAX;
	treeMax = FLT_MIN;
	for (vector<BaseEntity*>::iterator b = sortedEntities.begin() ; b != sortedEntities.end(); ++b)
	{
		if ((*b)->isPackage() == 0)
		{
			treeMin = ((*b)->getScore() < treeMin) ? (*b)->getScore() : treeMin;
			treeMax = ((*b)->getScore() > treeMax) ? (*b)->getScore() : treeMax;
		}
	}
}

// Return vector of pointers of the selected entities
void EntityTree::getEntitiesByPositionOnTreemap(int *drag, unsigned click)
{
	if (click)
		selected.clear();

	for (vector<BaseEntity*>::iterator b = sortedEntities.begin(); b != sortedEntities.end(); ++b)
	{
		if ((*b)->isPackage() == 0)
		{
			if ((*b)->getCoord(3) < drag[1])
				continue; // b too high
			else if ((*b)->getCoord(1) > drag[3])
				continue; // svg too low
			else if ((*b)->getCoord(2) < drag[0])
				continue;  // svg too far left
			else if ((*b)->getCoord(0) > drag[2])
				continue;  // svg too far right
			else
			{
				if (click)
					selected.push_back((Entity*)*b);
				else
					hovered = (Entity*)*b;
			}
		}
	}
}

//
void EntityTree::getEntitiesByPositionOnProjection(int *drag, unsigned Rt, unsigned click)
{
	Entity *closest = NULL;
	hovered = NULL;
	double smallerDist = FLT_MAX;
	if (click)
		selected.clear();

	for (vector<BaseEntity*>::iterator b = sortedEntities.begin(); b != sortedEntities.end(); ++b)
	{
		if ((*b)->isPackage() == 0)
		{
			double bx = ((Entity*)(*b))->normalizedProjectionPoints[Rt].x;
			double by = ((Entity*)(*b))->normalizedProjectionPoints[Rt].y;
			if ((drag[0] == drag[2] && drag[1] == drag[3])) // Case point click (or hover of click = 0)
			{
				double distX = drag[0] - bx;
				double distY = drag[1] - by;
				double dist = sqrt(pow(distX,2) + pow(distY,2));
				if (dist < 10 && dist < smallerDist) // If click is close enough
				{
					if (click)
					{
						smallerDist = dist;
						closest = (Entity*)(*b);
					}
					else
					{
						smallerDist = dist;
						hovered = (Entity*)(*b);
					}
				}
			}
			else if (bx > drag[0] && bx < drag[2] && by > drag[1] && by < drag[3]) // If inside selection box
			{
				selected.push_back((Entity*)*b);
			}
		}
	}
	if(click == 1 && closest != NULL)
	{
		selected.push_back(closest);
	}
}


// Add projection point to vector of points
void EntityTree::addProjection(string name, double x, double y, unsigned index)
{
	// Keep mins and maxs updated for normalizing the projection
	if(x < minX) minX = x;
	if(x > maxX) maxX = x;
	if(y < minY) minY = y;
	if(y > maxY) maxY = y;

	for (vector<BaseEntity*>::iterator b = sortedEntities.begin() ; b != sortedEntities.end(); ++b)
	{
		if((*b)->isPackage() == 0 && name == ((Entity*)(*b))->getPrefix()+'.'+(*b)->getName())
		{
			((Entity*)(*b))->addPointAtIndex({x, y}, index);
		}
	}
}

void EntityTree::normalizeProjection(int shortEdge)
{
	for (vector<BaseEntity*>::iterator b = sortedEntities.begin() ; b != sortedEntities.end(); ++b)
	{
		if((*b)->isPackage() == 0)
		{
			for(unsigned i = 0; i < ((Entity*)(*b))->projectionPoints.size() ; ++i)
			{
				double normX = (((Entity*)(*b))->projectionPoints[i].x - minX)*((double)shortEdge)/(maxX - minX);
				double normY = (((Entity*)(*b))->projectionPoints[i].y - minY)*((double)shortEdge)/(maxY - minY);
				((Entity*)(*b))->normalizedProjectionPoints[i] = {normX,normY};
			}
		}
	}
}
