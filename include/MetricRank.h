#ifndef METRICRANK_H
#define METRICRANK_H

#define NEIGHBORHOOD_RADIUS 150
#define MIN_NEIGHBORHOOD_SIZE 5

#include <vector>
#include <algorithm>
#include <cmath>

#include "Entity.h"
#include "EntityTree.h"
#include "Colormap.h"

using namespace std;

class MetricRank
{
public:
	MetricRank(EntityTree *_et);
	vector<vector<unsigned char>> explainingMetric;

private:
	void computeGlobalMetricStd();
	void computeGlobalMetricMean(vector<double> &globalMean);
	void computeLocalExplainingMetric(unsigned Rt, double d);
	double projectionDistance(Entity* a, Entity* b, unsigned rev);

	EntityTree *et;
	vector<double> globalStd;
};

#endif
