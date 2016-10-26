#include "../include/HierarchicalGraph.h"

HierarchicalGraph::HierarchicalGraph() {}

HierarchicalGraph &HierarchicalGraph::getInstance() {

    static HierarchicalGraph instance{};
    return instance;
}

void HierarchicalGraph::init(Point tl, Point br, EntityData *ed) {

    setSize(tl, br);
    entityData = ed;
    buildGraph();
}

void HierarchicalGraph::buildGraph() {

    unsigned nNonLeafs = (unsigned) entityData->firstLevelGlobalCounter;
    nodes.assign(nNonLeafs, Node{true});
    vector<unsigned> childrenCount(nNonLeafs, 0);

    // Compute non leaf nodes position -- except for the root
    for (Entity *entity : entityData->entities) {
        int id = entity->getFirstLevelId() - 1;
        nodes[id].position = nodes[id].position + entity->normalizedProjectionPoints[0];
        childrenCount[id]++;
    }

    for (unsigned i = 1; i < nNonLeafs; ++i) {
        if (childrenCount[i] != 0) {
            nodes[i].position = nodes[i].position / childrenCount[i];
            nodes[0].position = nodes[0].position + nodes[i].position;
        }
    }

    // Compute root position
    nodes[0].position = nodes[0].position / (nNonLeafs - 1);

    // Initialize adjacency matrix
    nNodes = (unsigned int) (nNonLeafs + entityData->entities.size());

    adjacencyMatrix.resize(nNodes);
    for (unsigned i = 0; i < nNodes; ++i) {
        adjacencyMatrix[i].assign(nNodes, false);
    }

    // Connect root to non leafs
    for (unsigned i = 1; i < nNonLeafs; ++i) {
        adjacencyMatrix[0][i] = true;
        adjacencyMatrix[i][0] = true;
    }

    // Connect leafs to non leafs
    for (unsigned entityIndex = 0; entityIndex < entityData->entities.size(); ++entityIndex ) {

        unsigned entityMatrixIndex = entityIndex + nNonLeafs;
        unsigned nonLeafParent = entityData->entities[entityIndex]->getFirstLevelId() - 1;

        adjacencyMatrix[entityMatrixIndex][nonLeafParent] = true;
        adjacencyMatrix[nonLeafParent][entityMatrixIndex] = true;
    }
}

void HierarchicalGraph::drawCanvas(unsigned Rt, double animationStep) {

}

