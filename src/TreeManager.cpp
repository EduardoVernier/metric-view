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

	sortPackages(&packageVector[0]);
	//packageVector[0].printPackage(0);
	generateOutputVector(&packageVector[0]);


	for (vector<BaseEntity*>::iterator b = output.begin() ; b != output.end(); ++b)
	{
		cout << (*b)->getScore() << " " << (*b)->getName() << endl;
	}


}

void TreeManager::sortPackages(Package *p)
{
	sort (p->childrenVector.begin(), p->childrenVector.end(), compPackages());

	for(unsigned i = 0; i < p->childrenVector.size(); ++i)
	{
		sortPackages(&p->childrenVector[i]);
	}
}


void TreeManager::generateOutputVector(Package *p)
{

	int i = 0, j = 0;
	while (1)
	{
		// If both are comparable
		if (i < p->childrenVector.size() && j < p->entityVector.size())
		{
			if (p->childrenVector[i].sum >= p->entityVector[j].value)
			{
				output.push_back((BaseEntity*) &p->childrenVector[i]);
				generateOutputVector(&p->childrenVector[i]);
				i++;
			}
			else
			{
				output.push_back((BaseEntity*) &p->entityVector[j]);
				j++;
			}
		}
		else if (i < p->childrenVector.size() && j == p->entityVector.size())
		{
			// Only packages left
			for (; i < p->childrenVector.size(); ++i)
			{
				output.push_back((BaseEntity*) &p->childrenVector[i]);
				generateOutputVector(&p->childrenVector[i]);
			}
		}
		else if (i == p->childrenVector.size() && j < p->entityVector.size())
		{
			// Only entities left
			for (; j < p->entityVector.size(); ++j)
			{
				output.push_back((BaseEntity*) &p->entityVector[j]);
			}			
		}

		if (i == p->childrenVector.size() && j == p->entityVector.size())
		{
			break;
		}
	}
}


