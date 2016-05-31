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
	CsvParser(EntityTree *et, string dataDir, string projectionDirName);
	EntityTree* getEntityTree() { return entityTree; };

private:
	void parseMetricFile(string filename, unsigned nRevisions);
	void parseProjectionFile(string filename, unsigned index);

	EntityTree *entityTree;
};


#endif
