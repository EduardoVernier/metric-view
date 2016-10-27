#include "../include/Entity.h"

Entity::Entity(string csvLine, unsigned nAttributes, unsigned nRevisions) {

    // Collect all data
    stringstream ss(csvLine);

    // Collect name
    string item;
    getline(ss, item, ';');
    string name = item;
    size_t separator = name.rfind(".");
    id = name.substr(separator + 1, name.length());
    if (separator == string::npos)
        prefix = "";
    else
        prefix = name.substr(0, separator);

    // Initialize vectors
    data.resize(nRevisions);
    normalizedData.resize(nRevisions);

    for (unsigned i = 0; i < nRevisions; ++i) {
        data[i].resize(nAttributes, 0.0f);
        normalizedData[i].resize(nAttributes, 0.0f);
    }

    unsigned i = 0;
    while (getline(ss, item, ';')) {
        if (i >= nAttributes) break;
        istringstream buffer(item);
        buffer >> value;
        data[nRevisions - 1][i] = (value);
        ++i;
    }

    // CountLine metric as value
    value = data.at(nRevisions - 1)[21];

    // Create projection positions vector
    projectionPoints.resize(nRevisions, {0, 0});
    normalizedProjectionPoints.resize(nRevisions, {0, 0});
}

void Entity::addRevisionData(string dataLine, unsigned rev, int nAttributes) {

    stringstream ss(dataLine);
    string item;
    getline(ss, item, ';'); // flush name

    int i = 0;
    while (getline(ss, item, ';')) {
        if (i >= nAttributes) break;
        istringstream buffer(item);
        buffer >> value;
        data[rev][i] = (value);
        ++i;
    }
}

void Entity::addPointAtIndex(Point p, unsigned index) {

    projectionPoints[index] = p;
}

double Entity::getScore() {

    double max = 0;
    for (unsigned i = 0; i < data.size(); ++i) {
        if (data[i][21] > max)
            max = data[i][21];
    }
    return max;
}

// Interpolates points according to animation step
Point Entity::getPosition(unsigned int Rt, double animationStep) {
    Point p;
    if (animationStep == 1 || animationStep == -1) // No movement
    {
        p.x = this->normalizedProjectionPoints[Rt].x;
        p.y = this->normalizedProjectionPoints[Rt].y;
    } else if (animationStep > 0 && Rt > 0 && Rt < normalizedProjectionPoints.size()) // Moving forwards
    {
        p.x = (1 - animationStep) * this->normalizedProjectionPoints[Rt - 1].x
              + animationStep * this->normalizedProjectionPoints[Rt].x;
        p.y = (1 - animationStep) * this->normalizedProjectionPoints[Rt - 1].y
              + animationStep * this->normalizedProjectionPoints[Rt].y;
    } else if (animationStep < 0 && Rt < normalizedProjectionPoints.size()) // Moving backwards
    {
        animationStep *= -1;
        p.x = (1 - animationStep) * this->normalizedProjectionPoints[Rt + 1].x
              + animationStep * this->normalizedProjectionPoints[Rt].x;
        p.y = (1 - animationStep) * this->normalizedProjectionPoints[Rt + 1].y
              + animationStep * this->normalizedProjectionPoints[Rt].y;
    }
    return p;
}
















