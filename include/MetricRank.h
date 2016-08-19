#ifndef METRICRANK_H
#define METRICRANK_H

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
	void computeLocalGroupMetric();
	void computeLocalMetricMean(vector<double> &localMean);

	vector<vector<unsigned char>> explainingMetric;

private:
	void computeGlobalMetricStd();
	void computeGlobalMetricMean(vector<double> &globalMean);

	EntityTree *et;
	vector<double> globalStd;
};

#endif
