#include "../include/MetricRank.h"

MetricRank::MetricRank(EntityTree *_et)
{
	et = _et;
	computeClusters(0, 20);
}

void MetricRank::computeClusters(unsigned Rt, double d)
{
	// Clear cluster vector for new computation
	for (unsigned i = 0; i < clusters.size(); ++i)
	{
		clusters[i].clear();
	}

	vector<Entity*> entities;
	for (vector<BaseEntity*>::iterator b = et->sortedEntities.begin(); b != et->sortedEntities.end(); ++b)
	{
		if((*b)->isPackage() == 0 && (*b)->getName() != "")
		{
			entities.push_back((Entity*)(*b));
		}
	}

}
