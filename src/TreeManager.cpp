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
			sort (packageVector.begin(), packageVector.end(), compPackages());

			cout << endl;
			for (unsigned i=0; i<packageVector.size(); i++)
				cout << packageVector[i].sum << ' ' << packageVector[i].prefix << endl;
		}
	}
}
