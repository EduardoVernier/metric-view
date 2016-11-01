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

    nNonLeafs = (unsigned) entityData->firstLevelGlobalCounter;
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
            nodes[i].position = (nodes[i].position + Point {10, 10}) / childrenCount[i];
            nodes[0].position = nodes[0].position + nodes[i].position;
        }
    }

    // Compute root position
    nodes[0].position = nodes[0].position / (nNonLeafs - 1); // Avoid putting on top of entity

    // Add entities to nodes
    for (unsigned i = 0; i < entityData->entities.size(); ++i) {
        Node node = Node {false};
        node.position = entityData->entities[i]->normalizedProjectionPoints[0];
        node.entity = entityData->entities[i];
        nodes.push_back(node);
    }

    // Connect root to non leafs
    for (unsigned i = 1; i < nNonLeafs; ++i) {
        adjacencyList.push_back(make_pair(&nodes[0], &nodes[i]));
    }

    // Connect leafs to non leafs
    for (unsigned entityIndex = 0; entityIndex < entityData->entities.size(); ++entityIndex) {

        unsigned entityMatrixIndex = entityIndex + nNonLeafs; // How entity indexing works
        unsigned nonLeafParent = (unsigned int) (entityData->entities[entityIndex]->getFirstLevelId() - 1);

        adjacencyList.push_back(make_pair(&nodes[entityMatrixIndex], &nodes[nonLeafParent]));
    }

    nNodes = (unsigned int) nodes.size();
    nEdges = (unsigned int) adjacencyList.size();
}

void HierarchicalGraph::drawCanvas(unsigned Rt, double animationStep) {

    updatePositions(Rt, animationStep);

    glPushMatrix();
    glTranslated(xOff, yOff, 0.0);

    xRatio = currentWidth / initialWidth;
    yRatio = currentHeight / initialHeight;
    double minRatio = min(xRatio, yRatio);
    glScaled(minRatio, minRatio, 1.0);


    for (unsigned i = 0; i < nEdges; ++i) {
        pair<Node *, Node *> edge = adjacencyList[i];
        Node *nodeA = edge.first;
        Node *nodeB = edge.second;

        if (entityData->selected.size() != 0) {
            bool skipFlag = true;
            for (Entity *entity : entityData->selected) {
                if (entity == nodeA->entity || entity == nodeB->entity) {
                    skipFlag = false;
                }
            }

            if (skipFlag) {
                continue;
            }
        }

        if (nodeA->entity == NULL && nodeB->entity == NULL) {
            drawEdge(nodeA->position, nodeB->position, Color{.5, .5, .5});
            drawNonLeafNode(nodeA->position, Color{.5, .5, .5});
            drawNonLeafNode(nodeB->position, Color{.5, .5, .5});
        } else if (nodeA->entity == NULL) {
            Color color = getColor((int) COLORMAP::qualitative, nodeB->entity, 0);
            drawEdge(nodeA->position, nodeB->position, color);
            drawNonLeafNode(nodeA->position, color);
        } else if (nodeB->entity == NULL) {
            Color color = getColor((int) COLORMAP::qualitative, nodeA->entity, 0);
            drawEdge(nodeA->position, nodeB->position, color);
            drawNonLeafNode(nodeB->position, color);
        }
    }

    glPopMatrix();
}

void HierarchicalGraph::drawEdge(Point pointA, Point pointB, Color color) {

    glColor3d(color.R, color.G, color.B);
    // Draw edge
    glBegin(GL_LINES);
    glVertex2d(pointA.x, pointA.y);
    glVertex2d(pointB.x, pointB.y);
    glEnd();
}

void HierarchicalGraph::drawNonLeafNode(Point point, Color color) {

    double d = 5;
    glColor3d(color.R, color.G, color.B);
    // Draw edge
    glBegin(GL_POLYGON);
    glVertex2d(point.x, point.y - d);
    glVertex2d(point.x + d, point.y);
    glVertex2d(point.x, point.y + d);
    glVertex2d(point.x - d, point.y);
    glEnd();
}

void HierarchicalGraph::updatePositions(unsigned Rt, double animationStep) {

    double REPULSION = 5.0;
    double ATTRACTION = 0.01;

    // Update entity nodes position
    for (unsigned i = 0; i < nEdges; ++i) {
        pair<Node*, Node*> edge = adjacencyList[i];
        Node *nodeA = edge.first;
        Node *nodeB = edge.second;

        if (nodeA->entity != NULL) {
            nodeA->position = nodeA->entity->getPosition(Rt, animationStep);
        }
        if (nodeB->entity != NULL) {
            nodeB->position = nodeB->entity->getPosition(Rt, animationStep);
        }
    }

    // Compute repulsion force on each nonLeaf
    for (unsigned i = 0; i < nNonLeafs + 1; ++i) {
        nodes[i].netForce.x = 0;
        nodes[i].netForce.y = 0;
        for (unsigned j = 0; j < nNodes; ++j) {
            if (i != j) {
                double dist = nodes[i].position.euclidianDistance(nodes[j].position);
                nodes[i].netForce += (nodes[i].position - nodes[j].position)
                                     * (REPULSION / (dist*dist));
            }
        }
    }

    // Compute attraction
    for (unsigned i = 0; i < nEdges; ++i) {
        pair<Node*, Node *> edge = adjacencyList[i];
        Node *nodeA = edge.first;
        Node *nodeB = edge.second;

        if (nodeA->movable) {
            nodeA->netForce += (nodeB->position - nodeA->position) * ATTRACTION;
        }

        if (nodeB->movable) {
            nodeB->netForce += (nodeA->position - nodeB->position) * ATTRACTION;
        }
    }

    for (unsigned i = 0; i < nNonLeafs + 1; ++i) {
        nodes[i].velocity = nodes[i].velocity * 0.3 + nodes[i].netForce * 0.7;
        nodes[i].position += (nodes[i].velocity);
    }
}


