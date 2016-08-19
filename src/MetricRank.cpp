#include "../include/MetricRank.h"

MetricRank::MetricRank(EntityTree *_et)
{
	et = _et;
	computeGlobalMetricStd();
}

void MetricRank::computeGlobalMetricStd()
{
	unsigned nDim = et->metricVector.size();
	vector<double> globalMean;
	globalMean.resize(nDim, 0);
	globalStd.resize(nDim, 0);
	computeGlobalMetricMean(globalMean);

	for (unsigned m = 0; m < nDim; ++m)
	{
		for (unsigned t = 0; t < et->nRevisions; ++t)
		{
			for (vector<Entity*>::iterator b = et->entities.begin(); b != et->entities.end(); ++b)
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
	for (unsigned m = 0; m < nDim; ++m)
	{
		for (unsigned t = 0; t < et->nRevisions; ++t)
		{
			for (vector<Entity*>::iterator b = et->entities.begin(); b != et->entities.end(); ++b)
			{
				globalMean[m] += (*b)->data[t][m];
			}
		}
		globalMean[m] /= (double)(et->nRevisions*et->entities.size());
	}
}

void MetricRank::computeLocalGroupMetric()
{
	unsigned nDim = et->metricVector.size();
	vector<double> localMean;
	localMean.resize(nDim, 0);
	//localStd.resize(nDim, 0);
	computeLocalMetricMean(localMean);

}

void MetricRank::computeLocalMetricMean(vector<double> &localMean)
{

}
