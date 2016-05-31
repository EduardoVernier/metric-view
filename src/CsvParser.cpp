#include "../include/CsvParser.h"
#include "../include/EntityTree.h"
#include "../include/Package.h" // Remove after testing

CsvParser::CsvParser(EntityTree *et, string dataDirName, string projectionDirName)
{
	entityTree = et;

	// Parse metric data
	struct dirent *dDirent;
	DIR *dataDir;
	string lastRev;

	// Check if it is a valid directory
	dataDir = opendir(dataDirName.c_str());
	if (dataDir == NULL)
	{
		cout << "Cannot open directory" << dataDirName << endl;
		return;
	}

	// Find last revision file
	unsigned nRevisions = 0;
	lastRev = "";
	while ((dDirent = readdir(dataDir)) != NULL)
	{
		string filename (dDirent->d_name);
		if (filename.find(".data") != string::npos)
		{
			if (filename.length() >= lastRev.length())
			{
				lastRev = filename;
				if (dDirent->d_name > lastRev)
				{
					lastRev = filename;
				}
			}
			nRevisions++;
		}
	}
	closedir (dataDir);
	parseMetricFile(dataDirName + "/" + lastRev, nRevisions);
	entityTree->buildHierarchy();

	// Parse projection data
	DIR *projDir;
	projDir = opendir(projectionDirName.c_str());
	if (projDir == NULL)
	{
		cout << "Cannot open directory " << projectionDirName << endl;
		return;
	}

	while ((dDirent = readdir(projDir)) != NULL)
	{
		string filename (dDirent->d_name);
		if (filename.find(".2d") != string::npos)
		{
			size_t f = filename.find_first_of('.');
			size_t l = filename.find_last_of('.');
			unsigned index;
			istringstream(filename.substr(f+1, l-f-1)) >> index;
			parseProjectionFile(projectionDirName + filename, index);
		}
	}
}

void CsvParser::parseMetricFile(string filename, unsigned nRevisions)
{
	int nEntities, nAttributes;
	ifstream file(filename.c_str());
	string line;

	if (!getline(file,line) && line != "DY/n") // 'DY' - flush
	{
		cout << filename << " - path not compatible" << endl;
		return;
	}

	getline(file,line); // number of classes in file
	istringstream(line) >> nEntities;
	getline(file,line); // number of attributes
	istringstream(line) >> nAttributes;
	getline(file,line); // name of attributes - flush

	// Add tree root
	entityTree->addEntity(Entity(";0;0;0;0;0;0;0;0.0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0.0;0;0;0;0;0;0", nRevisions));

	// Add all elements (classes) to the entityTree
	while(getline(file,line))
  {
  	entityTree->addEntity(Entity(line, nRevisions));
  }
}

void CsvParser::parseProjectionFile(string filename, unsigned index)
{
	int nEntities, nAttributes;
	ifstream file(filename.c_str());
	string line;

	if (!getline(file,line) && line != "DY/n") // 'DY' - flush
	{
		cout << filename << " - path not compatible" << endl;
		return;
	}

	getline(file,line); // number of classes in file
	istringstream(line) >> nEntities;
	getline(file,line); // number of attributes
	istringstream(line) >> nAttributes;
	getline(file,line); // name of attributes - flush

	while(getline(file,line))
	{
		stringstream ss(line);
		string name, sx, sy;
		double x, y;
		getline(ss, name, ';');
		getline(ss, sx, ';');
		istringstream(sx) >> x;
		getline(ss, sy, ';');
		istringstream(sy) >> y;
		// cout << name << " " << x << " " << y << endl;
		entityTree->addProjection(name, x, y, index);
	}

}
