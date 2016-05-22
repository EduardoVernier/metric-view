#include "../include/CsvParser.h"
#include "../include/EntityTree.h"
#include "../include/Package.h" // Remove after testing

CsvParser::CsvParser(string dataDirName)
{
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
	lastRev = "";
	while ((dDirent = readdir(dataDir)) != NULL)
	{
		string filename (dDirent->d_name);
		if (filename.length() >= lastRev.length())
		{
			lastRev = filename;
			if (dDirent->d_name > lastRev)
			{
				lastRev = filename;
			}
		}
	}
	closedir (dataDir);
	cout << dataDirName + lastRev <<  endl;
	parseMetricFile(dataDirName + lastRev);
}

void CsvParser::parseMetricFile(string filename)
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
	entityTree->addEntity(Entity(";0;0;0;0;0;0;0;0.0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0.0;0;0;0;0;0;0"));

	// Add all elements (classes) to the entityTree
	while(getline(file,line))
    {
        entityTree->addEntity(Entity(line));
    }

    entityTree->buildHierarchy();

}
