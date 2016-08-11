#include "../include/MetricRank.h"

MetricRank::MetricRank(EntityTree *_et)
{
	et = _et;
	computeGlobalVariance();
}


void MetricRank::computeGlobalVariance()
{
	unsigned nDim = et->metricVector.size();
	vector<double> globalMean(nDim, 0.0);

	// Compute metric mean
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
