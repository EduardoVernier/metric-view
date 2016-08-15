#include "../include/MetricRank.h"

MetricRank::MetricRank(EntityTree *_et)
{
	et = _et;
	computeGlobalMetricStd();

	explainingMetric.resize(et->entities.size());
	for(unsigned i = 0; i < et->entities.size(); ++i)
		explainingMetric[i].resize(et->nRevisions, 255);

	for(unsigned t = 0; t < et->nRevisions; ++t)
		computeLocalExplainingMetric(t, NEIGHBORHOOD_RADIUS);
}

void MetricRank::computeGlobalMetricStd()
{
	unsigned nDim = et->metricVector.size();
	vector<double> globalMean;
	globalMean.resize(nDim, 0);
	globalStd.resize(nDim, 0);
	computeGlobalMetricMean(globalMean);

	for(unsigned m = 0; m < nDim; ++m)
	{
		for(unsigned t = 0; t < et->nRevisions; ++t)
		{
			for(vector<Entity*>::iterator b = et->entities.begin(); b != et->entities.end(); ++b)
			{
				globalStd[m] += pow((*b)->data[t][m] - globalMean[m], 2);
			}
		}
		globalStd[m] /= (double)(et->nRevisions*et->entities.size());
		globalStd[m] = sqrt(globalStd[m]);
	}
}

void MetricRank::computeGlobalMetricMean(vector<double> &globalMean)
{
	unsigned nDim = et->metricVector.size();
	for(unsigned m = 0; m < nDim; ++m)
	{
		for(unsigned t = 0; t < et->nRevisions; ++t)
		{
			for(vector<Entity*>::iterator b = et->entities.begin(); b != et->entities.end(); ++b)
			{
				globalMean[m] += (*b)->data[t][m];
			}
		}
		globalMean[m] /= (double)(et->nRevisions*et->entities.size());
	}
}

void MetricRank::computeLocalExplainingMetric(unsigned Rt, double d)
{
	// Look for valid neighborhood
	for(vector<Entity*>::iterator pivot = et->entities.begin(); pivot != et->entities.end(); ++pivot)
	{
		vector<Entity*> localNeighborhood;
		localNeighborhood.push_back(*pivot);
		for(vector<Entity*>::iterator neighbor = et->entities.begin(); neighbor != et->entities.end(); ++neighbor)
		{
			if(pivot == neighbor)
				continue;
			if (projectionDistance(*pivot, *neighbor, Rt) < d)
			{
				localNeighborhood.push_back(*neighbor);
			}
		}
		int i = 10; ++i;
	}
}


double MetricRank::projectionDistance(Entity* a, Entity* b, unsigned rev)
{
	return sqrt(pow(a->normalizedProjectionPoints[rev].x - b->normalizedProjectionPoints[rev].x, 2)
							+ pow(a->normalizedProjectionPoints[rev].y - b->normalizedProjectionPoints[rev].y, 2));
}
