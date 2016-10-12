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
			if (pckg->getName() == ent.getPrefix())
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
				j->sum += i->sum;
				j->addChild(*i);
				packageVector.erase((i+1).base());
				break;
			}
		}
	}
	setHierarchicalLevel(&packageVector[0], 0);
	setFirstLevelId(&packageVector[0], 0);
	sortPackages(&packageVector[0]);
	generateSortedEntitiesVector(&packageVector[0]);
	setMinMax();
	generateEntityVector();
	//printTree();
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

// Determines every entity tree level
void EntityTree::setFirstLevelId(Package *p, int id)
{
	if (p->isPackage() == 1 && p->getLevel() <= 2)
		p->firstLevelId = ++firstLevelGlobalCounter;
	else
		p->firstLevelId = firstLevelGlobalCounter;

	for (unsigned i = 0; i < p->entityVector.size(); ++i)
	{
		p->entityVector[i].firstLevelId = firstLevelGlobalCounter;
	}

	for (unsigned i = 0; i < p->childrenVector.size(); ++i)
	{
		setFirstLevelId(&p->childrenVector[i], firstLevelGlobalCounter);
	}
}

// Sorts individual packages (parcial order)
void EntityTree::sortPackages(Package *p)
{
	sort (p->childrenVector.begin(), p->childrenVector.end(),
				[](const Package& p1, const Package& p2){ return (p1.sum > p2.sum); });

	for (unsigned i = 0; i < p->childrenVector.size(); ++i)
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
				sortedBaseEntities.push_back((BaseEntity*) &p->childrenVector[i]);
				generateSortedEntitiesVector(&p->childrenVector[i]);
				i++;
			}
			else
			{
				// Entity is bigger
				sortedBaseEntities.push_back((BaseEntity*) &p->entityVector[j]);
				j++;
			}
		}
		else if (i < p->childrenVector.size() && j == p->entityVector.size())
		{
			// Only packages left
			for (; i < p->childrenVector.size(); ++i)
			{
				sortedBaseEntities.push_back((BaseEntity*) &p->childrenVector[i]);
				generateSortedEntitiesVector(&p->childrenVector[i]);
			}
		}
		else if (i == p->childrenVector.size() && j < p->entityVector.size())
		{
			// Only entities left
			for (; j < p->entityVector.size(); ++j)
			{
				sortedBaseEntities.push_back((BaseEntity*) &p->entityVector[j]);
			}
		}

		if (i == p->childrenVector.size() && j == p->entityVector.size())
			break;
	}
}

void EntityTree::generateEntityVector()
{
	for (auto b : sortedBaseEntities)
	{
		if (b->isEntity() && b->getName() != "")
			entities.push_back((Entity*)b);
	}
}

void EntityTree::printTree()
{
	// Test printing
	for (vector<BaseEntity*>::iterator b = sortedBaseEntities.begin() ; b != sortedBaseEntities.end(); ++b)
	{
		if ((*b)->getName() == "") continue; // Ignore root
		for (int j = 0; j < (*b)->getLevel(); ++j)
			cout << " ";

		cout << (*b)->getLevel() << " - " << (*b)->getScore() << " " << (*b)->getName() << " ";
		if ((*b)->isPackage())
			cout << ((Package*)(*b))->numberOfEntities << " " << ((Package*)(*b))->sum;
		cout << endl;
	}
}

// Set min and max for use on colormapping
void EntityTree::setMinMax()
{
	treeMin = FLT_MAX;
	treeMax = FLT_MIN;
	for (auto b : sortedBaseEntities)
	{
		if (b->isEntity())
		{
			treeMin = (b->getScore() < treeMin) ? b->getScore() : treeMin;
			treeMax = (b->getScore() > treeMax) ? b->getScore() : treeMax;
		}
	}
}

// Add projection point to vector of points
void EntityTree::addProjection(string name, double x, double y, unsigned index)
{

	// Keep mins and maxs updated for normalizing the projection
	if (x < minX) minX = x;
	if (x > maxX) maxX = x;
	if (y < minY) minY = y;
	if (y > maxY) maxY = y;

	for (auto b : sortedBaseEntities)
	{
		if (b->isEntity() && name == ((Entity*)b)->getPrefix()+'.'+b->getName())
		{
			((Entity*)b)->addPointAtIndex({x, y}, index);
		}
	}
}

// Normalize projection points to fit on canvas nicely
void EntityTree::normalizeProjection(double shortEdge)
{
	for (auto b : entities)
	{
		for (unsigned i = 0; i < ((Entity*)b)->projectionPoints.size() ; ++i)
		{
			double normX = (((Entity*)b)->projectionPoints[i].x - minX)*((double)shortEdge)/(maxX - minX) + 20;
			double normY = (((Entity*)b)->projectionPoints[i].y - minY)*((double)shortEdge)/(maxY - minY) + 20;
			((Entity*)b)->normalizedProjectionPoints[i] = {normX, normY};
		}

		if (b->getName() == "")
			b->setAsPackage(); // Fix root element
	}
}

// Given a name and prefix, return corresponding entity pointer
Entity* EntityTree::getEntityByName(string prefix, string id)
{
	for (auto b : entities)
	{
		if (b->getName() == id && b->getPrefix() == prefix)
		{
			return b;
		}
	}
	return NULL;
}

void EntityTree::setColorMetric(int mIndex)
{
	colorMetricIndex = mIndex;
	colorMetricMin = FLT_MAX;
	colorMetricMax = FLT_MIN;
	for (unsigned i = 0; i < sortedBaseEntities.size() ; ++i)
	{
		if (sortedBaseEntities[i]->isEntity())
		{
			for (unsigned time = 0; time < ((Entity*)sortedBaseEntities[i])->data.size() ; ++time)
			{
				float mValue = ((Entity*)sortedBaseEntities[i])->data[time][mIndex];
				colorMetricMax = (mValue > colorMetricMax)? mValue : colorMetricMax;
				colorMetricMin = (mValue < colorMetricMin)? mValue : colorMetricMin;
			}
		}
	}
}

void EntityTree::setRadiusMetric(int mIndex)
{
	radiusMetricIndex = mIndex;
	radiusMetricMin = FLT_MAX;
	radiusMetricMax = FLT_MIN;
	for (unsigned i = 0; i < sortedBaseEntities.size() ; ++i)
	{
		if (sortedBaseEntities[i]->isEntity())
		{
			for (unsigned time = 0; time < ((Entity*)sortedBaseEntities[i])->data.size() ; ++time)
			{
				float mValue = ((Entity*)sortedBaseEntities[i])->data[time][mIndex];
				radiusMetricMax = (mValue > radiusMetricMax)? mValue : radiusMetricMax;
				radiusMetricMin = (mValue < radiusMetricMin)? mValue : radiusMetricMin;
			}
		}
	}
}

void EntityTree::setStreamMetric(int mIndex)
{
	streamMetricIndex = mIndex;
	streamMetricMin = FLT_MAX;
	streamMetricMax = FLT_MIN;
	for (unsigned i = 0; i < sortedBaseEntities.size() ; ++i)
	{
		if (sortedBaseEntities[i]->isEntity())
		{
			for (unsigned time = 0; time < ((Entity*)sortedBaseEntities[i])->data.size() ; ++time)
			{
				float mValue = ((Entity*)sortedBaseEntities[i])->data[time][mIndex];
				streamMetricMax = (mValue > streamMetricMax)? mValue : streamMetricMax;
				streamMetricMin = (mValue < streamMetricMin)? mValue : streamMetricMin;
			}
		}
	}
}

void EntityTree::rankFastestChangingEntities(unsigned Rt, int direction)
{
	unsigned K = 10; // Number of shadowed elements
	vector<pair<double,Entity*>> rank;

	// Compute distances and rank
	for (auto e : entities)
	{
		e->showHalo = false;
		Point pFrom = e->normalizedProjectionPoints[Rt-direction];
		Point pTo = e->normalizedProjectionPoints[Rt];
		double v = sqrt(pow(pTo.x - pFrom.x, 2) + pow(pTo.y - pFrom.y, 2));
		rank.push_back(make_pair(v, e));
	}
	sort(rank.begin(), rank.end());

	// Turn on drawShadow flag on K fastest changing elements
	for(unsigned i = 0; i < K; ++i)
	{
		((Entity*)(rank[rank.size()-1-i].second))->showHalo = true;
	}
}
