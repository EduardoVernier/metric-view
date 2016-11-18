#include "../include/EntityData.h"

// Places a new entity in it's adequate package
void EntityData::addEntity(Entity ent) {

    if (packageVector.empty()) {
        Package newPackage(ent.getPrefix());
        newPackage.addEntity(ent);
        packageVector.push_back(newPackage);
    } else {
        int found = 0;
        for (vector<Package>::iterator package = packageVector.begin(); package != packageVector.end(); ++package) {
            if (package->getName() == ent.getPrefix()) {
                found = 1;
                package->addEntity(ent);
                break;
            }
        }

        if (found == 0) {
            Package newPackage(ent.getPrefix());
            newPackage.addEntity(ent);
            packageVector.push_back(newPackage);
        }
    }
}

// Takes the list of packages and organizes it in tree
void EntityData::buildHierarchy() {

    for (vector<Package>::reverse_iterator i = packageVector.rbegin(); i != packageVector.rend(); ++i) {
        for (vector<Package>::reverse_iterator j = i + 1; j != packageVector.rend(); ++j) {
            if (i->getName().find(j->getName()) != string::npos) {
                j->sum += i->sum;
                j->addChild(*i);
                packageVector.erase((i + 1).base());
                break;
            }
        }
    }
    setHierarchicalLevel(&packageVector[0], 0);
    setFirstLevelId(&packageVector[0], 0);
    sortPackages(&packageVector[0]);
    generateSortedEntitiesVector(&packageVector[0]);
    generateEntityVector();
    //printTree();
}

// Determines every entity tree level
void EntityData::setHierarchicalLevel(Package *p, int level) {

    p->setLevel(level);
    if (level + 1 > depth)
        depth = level + 1;

    for (unsigned i = 0; i < p->entityVector.size(); ++i) {
        p->entityVector[i].setLevel(level + 1);
    }

    for (unsigned i = 0; i < p->childrenVector.size(); ++i) {
        setHierarchicalLevel(&p->childrenVector[i], level + 1);
    }
}

// Determines every entity tree level
void EntityData::setFirstLevelId(Package *p, int level) {

    if (p->isPackage() == 1 && p->getLevel() <= 2)
        p->setFirstLevelId(++firstLevelGlobalCounter);
    else
        p->setFirstLevelId(firstLevelGlobalCounter);

    for (unsigned i = 0; i < p->entityVector.size(); ++i) {
        p->entityVector[i].setFirstLevelId(firstLevelGlobalCounter);
    }

    for (unsigned i = 0; i < p->childrenVector.size(); ++i) {
        setFirstLevelId(&p->childrenVector[i], firstLevelGlobalCounter);
    }
}

// Sorts individual packages (partial order)
void EntityData::sortPackages(Package *p) {

    sort(p->childrenVector.begin(), p->childrenVector.end(),
         [](const Package &p1, const Package &p2) { return (p1.sum > p2.sum); });

    for (unsigned i = 0; i < p->childrenVector.size(); ++i) {
        sortPackages(&p->childrenVector[i]);
    }
}

// Creates a sorted vector of BaseEntity references
void EntityData::generateSortedEntitiesVector(Package *p) {

    unsigned i = 0, j = 0;
    while (1) {
        // If both are comparable
        if (i < p->childrenVector.size() && j < p->entityVector.size()) {
            if (p->childrenVector[i].sum >= p->entityVector[j].value) {
                // Package is bigger
                sortedBaseEntities.push_back((BaseEntity *) &p->childrenVector[i]);
                generateSortedEntitiesVector(&p->childrenVector[i]);
                i++;
            } else {
                // Entity is bigger
                sortedBaseEntities.push_back((BaseEntity *) &p->entityVector[j]);
                j++;
            }
        } else if (i < p->childrenVector.size() && j == p->entityVector.size()) {
            // Only packages left
            for (; i < p->childrenVector.size(); ++i) {
                sortedBaseEntities.push_back((BaseEntity *) &p->childrenVector[i]);
                generateSortedEntitiesVector(&p->childrenVector[i]);
            }
        } else if (i == p->childrenVector.size() && j < p->entityVector.size()) {
            // Only entities left
            for (; j < p->entityVector.size(); ++j) {
                sortedBaseEntities.push_back((BaseEntity *) &p->entityVector[j]);
            }
        }

        if (i == p->childrenVector.size() && j == p->entityVector.size())
            break;
    }
}

void EntityData::generateEntityVector() {

    for (auto baseEntity : sortedBaseEntities) {
        if (baseEntity->isEntity() && baseEntity->getName() != "")
            entities.push_back((Entity *) baseEntity);
    }
}

void EntityData::printTree() {
    // Test printing
    for (vector<BaseEntity *>::iterator baseEntity = sortedBaseEntities.begin();
         baseEntity != sortedBaseEntities.end(); ++baseEntity) {
        if ((*baseEntity)->getName() == "") continue; // Ignore root
        for (int j = 0; j < (*baseEntity)->getLevel(); ++j)
            cout << " ";

        cout << (*baseEntity)->getLevel() << " - " << (*baseEntity)->getScore() << " " << (*baseEntity)->getName()
             << " ";
        if ((*baseEntity)->isPackage())
            cout << ((Package *) (*baseEntity))->numberOfEntities << " " << ((Package *) (*baseEntity))->sum;
        cout << endl;
    }
}

// Add projection point to vector of points
void EntityData::addProjection(string name, double x, double y, unsigned index) {
    // Keep mins and maxs updated for normalizing the projection
    if (x < minX) minX = x;
    if (x > maxX) maxX = x;
    if (y < minY) minY = y;
    if (y > maxY) maxY = y;

    for (auto baseEntity : sortedBaseEntities) {
        if (baseEntity->isEntity() && name == ((Entity *) baseEntity)->getPrefix() + '.' + baseEntity->getName()) {
            ((Entity *) baseEntity)->addPointAtIndex({x, y}, index);
        }
    }
}

// Normalize projection points to fit on canvas nicely
void EntityData::normalizeProjection(double shortEdge) {

    for (auto entity : entities) {
        for (unsigned i = 0; i < (entity)->projectionPoints.size(); ++i) {
            double normX = ((entity)->projectionPoints[i].x - minX) * (shortEdge) / (maxX - minX) + 20;
            double normY = ((entity)->projectionPoints[i].y - minY) * (shortEdge) / (maxY - minY) + 20;
            (entity)->normalizedProjectionPoints[i] = {normX, normY};
        }

        if (entity->getName() == "")
            entity->setAsPackage(); // Fix root element
    }
}

void EntityData::normalizeData() {

    maxMetricValue.assign(nDimensions, DBL_MIN);
    minMetricValue.assign(nDimensions, DBL_MAX);

    // Collect min and max values for each metric
    for (unsigned metric = 0; metric < nDimensions; ++metric) {
        for (Entity *entity : entities) {
            for (unsigned revision = 0; revision < nRevisions; ++revision) {
                double value = entity->data[revision][metric];
                if (value > maxMetricValue[metric]) maxMetricValue[metric] = value;
                if (value < minMetricValue[metric]) minMetricValue[metric] = value;
            }
        }
    }

    // Normalized data
    for (unsigned metric = 0; metric < nDimensions; ++metric) {
        for (Entity *entity : entities) {
            for (unsigned revision = 0; revision < nRevisions; ++revision) {
                double value = entity->data[revision][metric];
                entity->normalizedData[revision][metric] =
                        (value - minMetricValue[metric]) / (maxMetricValue[metric] - minMetricValue[metric]);
            }
        }
    }
}

// Given a name and prefix, return corresponding entity pointer
Entity *EntityData::getEntityByName(string prefix, string id) {

    for (auto entity : entities) {
        if (entity->getName() == id && entity->getPrefix() == prefix) {
            return entity;
        }
    }
    return NULL;
}

void EntityData::rankFastestChangingEntities(unsigned Rt, int direction) {

    unsigned K = 10; // Number of shadowed elements
    vector<pair<double, Entity *>> rank;

    // Compute distances on nD and rank
    for (auto e : entities) {
        e->showHalo = false;
        double v = nDEuclidianDistance(e->normalizedData[Rt - direction], e->normalizedData[Rt]);
        rank.push_back(make_pair(v, e));
    }
    sort(rank.begin(), rank.end());

    // Turn on drawShadow flag on K fastest changing elements
    for (unsigned i = 0; i < K; ++i) {
        (rank[rank.size() - 1 - i].second)->showHalo = true;
    }
}


double EntityData::nDEuclidianDistance(vector<double> &a, vector<double> &b) {

    double sumOfSquaredDiff = 0.0;

    if (a.size() == b.size()) {
        unsigned nDimensions = (unsigned) a.size();
        for (unsigned i = 0; i < nDimensions; ++i) {
            sumOfSquaredDiff += pow(a[i] - b[i], 2);
        }
        return sqrt(sumOfSquaredDiff);
    } else {
        return 0;
    }

}

void EntityData::updateSelectedEntities() {

    vector<std::pair<double, Entity *>> sortedSelectedEntities;
    sortedSelectedEntities.clear();

    for (Entity *entity : selected) {
        double metricMean = 0.0;
        for (unsigned i = 0; i < nRevisions; ++i) {
            metricMean += entity->data[i][controller.evolutionMetricIndex];
        }

        pair<double, Entity *> selectedPair = std::make_pair(metricMean / nRevisions, entity);
        sortedSelectedEntities.push_back(selectedPair);
    }

    sort(sortedSelectedEntities.begin(), sortedSelectedEntities.end(),
         [](const std::pair<double, Entity *> &a, const std::pair<double, Entity *> &b) { return a.first < b.first; });

    selected.clear();
    for (pair<double, Entity *> &entityPair : sortedSelectedEntities) {
        selected.push_back(entityPair.second);
    }
}

void EntityData::computeMostSimilarPairs(unsigned Rt) {

    multimap<double, pair<Entity *, Entity *>> multimap;

    unsigned nEntities = (unsigned) entities.size();
    for (unsigned i = 0; i < nEntities; ++i) {
        for (unsigned j = i + 1; j < nEntities; ++j) {
            double dist = nDEuclidianDistance(entities[i]->normalizedData[Rt], entities[j]->normalizedData[Rt]);
            multimap.insert(pair<double, pair<Entity *, Entity *>>(dist, make_pair(entities[i], entities[j])));
        }
    }

    // Fill a sorted vector with most similar entities on first indexes
    similarityRank.clear();
    for (auto it = multimap.begin(); it != multimap.end(); ++it) {
        similarityRank.push_back(make_pair((*it).second.first, (*it).second.second));
    }
}