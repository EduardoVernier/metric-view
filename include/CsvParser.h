#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>

#include "Entity.h"
#include "EntityTree.h"

using namespace std;

class CsvParser
{
public:
	CsvParser(string dataDir);
	EntityTree getEntityTree() { return *entityTree; };

private:
	void parseMetricFile(string filename);
	EntityTree *entityTree = new EntityTree();
};


#endif
