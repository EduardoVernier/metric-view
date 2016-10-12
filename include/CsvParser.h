#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <memory>

#include "Entity.h"
#include "EntityData.h"

using namespace std;

class CsvParser
{
public:
	CsvParser(EntityData *ed, string dataDir, string projectionDirName);
	EntityData* getEntityData() { return entityData; };

private:
	void parseLastMetricFile(string filename, unsigned nRevisions);
	void parseMetricFile(string address, string filename);
	void parseProjectionFile(string filename, unsigned index);

	EntityData *entityData;
};
