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

private:
	void computeGlobalVariance();

	EntityTree *et;
	vector<double> globalVariance;
};

#endif

