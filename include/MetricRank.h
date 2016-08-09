#ifndef METRICRANK_H
#define METRICRANK_H

#include <vector>
#include <algorithm>

#include "Entity.h"
#include "EntityTree.h"
#include "Colormap.h"

using namespace std;

class MetricRank
{
public:
	MetricRank(EntityTree *_et);
	void computeClusters(unsigned Rt, double d);
	vector<vector<Entity*> > clusters;
private:
	EntityTree *et;
};

#endif

