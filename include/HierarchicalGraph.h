#pragma once

#include <vector>
#include "Point.h"
#include "EntityData.h"
#include "Node.h"
#include "Canvas.h"
#include "Colormap.h"

using namespace std;

class HierarchicalGraph : public Canvas {
private:
    HierarchicalGraph();

    EntityData *entityData;
    vector<Node> nodes;
    vector<pair<Node*, Node*>> adjacencyList;
    unsigned nNonLeafs;
    unsigned nNodes;
    unsigned nEdges;

public:
    static HierarchicalGraph &getInstance();
    void init(Point tl, Point br, EntityData *ed);
    void drawCanvas(unsigned Rt, double animationStep);
    void buildGraph();
    void updatePositions(unsigned int i, double d);

    void drawEdge(Point pointA, Point pointB, Color color);

    void drawNonLeafNode(Point point, Color color);

    Point normalize(Point vector);
};
