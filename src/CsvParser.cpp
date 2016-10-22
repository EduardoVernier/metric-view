#include "../include/CsvParser.h"

CsvParser::CsvParser(EntityData *ed, string dataDirName, string projectionDirName) {

    entityData = ed;

    // Parse metric data
    struct dirent *dDirent;
    DIR *dataDir;
    string lastRev;

    // Check if it is a valid directory
    dataDir = opendir(dataDirName.c_str());
    if (dataDir == NULL) {
        cout << "Cannot open directory" << dataDirName << endl;
        return;
    }

    // Find last revision file
    unsigned nRevisions = 0;
    lastRev = "";
    while ((dDirent = readdir(dataDir)) != NULL) {
        string filename(dDirent->d_name);
        if (filename.find(".data") != string::npos) {
            if (filename.length() >= lastRev.length()) {
                lastRev = filename;
                if (dDirent->d_name > lastRev) {
                    lastRev = filename;
                }
            }
            nRevisions++;
        }
    }
    entityData->nRevisions = nRevisions;

    closedir(dataDir);

    // Build empty entities
    parseLastMetricFile(dataDirName + "/" + lastRev, nRevisions);

    entityData->buildHierarchy();

    // Fill entities with metric data
    dataDir = opendir(dataDirName.c_str());
    nRevisions = 0;
    while ((dDirent = readdir(dataDir)) != NULL) {
        string filename(dDirent->d_name);
        if (filename.find(".data") != string::npos && filename != lastRev) {
            parseMetricFile(dataDirName, filename);
        }
    }
    closedir(dataDir);

    // Parse projection data
    DIR *projDir;
    projDir = opendir(projectionDirName.c_str());
    if (projDir == NULL) {
        cout << "Cannot open directory " << projectionDirName << endl;
        return;
    }

    while ((dDirent = readdir(projDir)) != NULL) {
        string filename(dDirent->d_name);
        if (filename.find(".2d") != string::npos) {
            size_t f = filename.find_first_of('.');
            size_t l = filename.find_last_of('.');
            unsigned index;
            istringstream(filename.substr(f + 1, l - f - 1)) >> index;
            parseProjectionFile(projectionDirName + filename, index);
        }
    }

    entityData->normalizeData();
}

// Use newest metric file to construct entities
void CsvParser::parseLastMetricFile(string filename, unsigned nRevisions) {

    int nEntities, nAttributes;
    ifstream file(filename.c_str());
    string line;

    if (!getline(file, line) && line != "DY/n") // 'DY' - flush
    {
        cout << filename << " - path not compatible" << endl;
        return;
    }

    getline(file, line); // number of classes in file
    istringstream(line) >> nEntities;
    getline(file, line); // number of attributes
    istringstream(line) >> nAttributes;
    entityData->nDimensions = (unsigned) nAttributes; // Save to entityData
    getline(file, line); // name of attributes - flush

    stringstream ss(line);
    for (int i = 0; i < nAttributes; ++i) {
        string metricName;
        getline(ss, metricName, ';');
        entityData->metricVector.push_back(metricName);
    }

    // Add tree root
    entityData->addEntity(
            Entity(";0;0;0;0;0;0;0;0.0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0.0;0;0;0;0;0;0",
                   nAttributes, nRevisions));

    // Add all elements (classes) from last metric file to the entityData
    while (getline(file, line)) {
        entityData->addEntity(Entity(line, nAttributes, nRevisions));
    }
}

// Fill entities with time dependent metrics
void CsvParser::parseMetricFile(string address, string filename) {

    unsigned rev;
    size_t f = filename.find_first_of('.');
    size_t l = filename.find_last_of('.');

    rev = stoi(filename.substr(f + 1, l - f - 1));

    ifstream file((address + '/' + filename).c_str());
    string line;

    if (!getline(file, line) && line != "DY/n") // 'DY' - flush
    {
        cout << filename << " - path not compatible" << endl;
        return;
    }

    getline(file, line); // number of classes in file
    getline(file, line); // number of attributes
    int nAttributes;
    istringstream(line) >> nAttributes;
    getline(file, line); // name of attributes - flush

    // Add all elements (classes) from a given revision to the entityData
    while (getline(file, line)) {
        // Collect name
        stringstream ss(line);
        string item, id, prefix;
        getline(ss, item, ';');
        string name = item;
        size_t separator = name.rfind(".");
        id = name.substr(separator + 1, name.length());
        if (separator == string::npos)
            prefix = "";
        else
            prefix = name.substr(0, separator);

        Entity *ent = entityData->getEntityByName(prefix, id);
        ent->addRevisionData(line, rev, nAttributes);
    }
}

// Extract projection data from .2d files
void CsvParser::parseProjectionFile(string filename, unsigned index) {

    int nEntities, nAttributes;
    ifstream file(filename.c_str());
    string line;

    if (!getline(file, line) && line != "DY/n") // 'DY' - flush
    {
        cout << filename << " - path not compatible" << endl;
        return;
    }

    getline(file, line); // number of classes in file
    istringstream(line) >> nEntities;
    getline(file, line); // number of attributes
    istringstream(line) >> nAttributes;
    getline(file, line); // name of attributes - flush

    while (getline(file, line)) {
        stringstream ss(line);
        string name, sx, sy;
        double x, y;
        getline(ss, name, ';');
        getline(ss, sx, ';');
        istringstream(sx) >> x;
        getline(ss, sy, ';');
        istringstream(sy) >> y;
        // cout << name << " " << x << " " << y << endl;
        entityData->addProjection(name, x, y, index);
    }
}
