#include "../include/TreeManager.h"

void TreeManager::addEntity(Entity ent)
{
	if (packageVector.empty())
	{
		Package newPackage(ent.prefix);
		newPackage.addEntity(ent);
		packageVector.push_back(newPackage);
	}
	else
	{
		int found = 0;
		for (vector<Package>::iterator pckg = packageVector.begin() ; pckg != packageVector.end(); ++pckg)
		{
			if(pckg->prefix == ent.prefix)
			{
				found = 1;
				pckg->addEntity(ent);
				break;
			} 
		}

		if (found == 0)
		{
			Package newPackage(ent.prefix);
			newPackage.addEntity(ent);
			packageVector.push_back(newPackage);
			//sort (packageVector.begin(), packageVector.end(), compPackages());
		}
	}
}

void TreeManager::buildHierarchy()
{
	
	for (std::vector<Package>::reverse_iterator i = packageVector.rbegin(); i!= packageVector.rend(); ++i)
	{
		for (std::vector<Package>::reverse_iterator j = i+1; j!= packageVector.rend(); ++j)
		{
			
			if (i->prefix.find(j->prefix) != string::npos)
			{
				j->addChild(*i);
				packageVector.erase((i+1).base());
				break;
			}
			
		}
	}

	packageVector[0].printPackage(0);
}
