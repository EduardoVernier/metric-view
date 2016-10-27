#include "../include/ProjectionCanvas.h"

ProjectionCanvas::ProjectionCanvas() {}

ProjectionCanvas &ProjectionCanvas::getInstance() {

    static ProjectionCanvas instance{};
    return instance;
}

void ProjectionCanvas::init(Point tl, Point br, EntityData *ed) {

    setSize(tl, br);
    entityData = ed;
    double shortEdge = min(currentHeight, currentWidth);
    entityData->normalizeProjection(shortEdge - 50);
};

// Mark entities as selected from projection pane interaction
void ProjectionCanvas::getEntitiesByPositionOnProjection(int *drag, unsigned Rt, unsigned click, bool ctrlDown) {

    Entity *closest = NULL;
    double smallerDist = FLT_MAX;

    if (click && !ctrlDown)
        entityData->selected.clear();

    entityData->hovered = NULL;

    xRatio = currentWidth / initialWidth;
    yRatio = currentHeight / initialHeight;
    minRatio = min(xRatio, yRatio);

    for (auto entity : entityData->entities) {
        double bx = entity->normalizedProjectionPoints[Rt].x * minRatio;
        double by = entity->normalizedProjectionPoints[Rt].y * minRatio;
        if ((drag[0] == drag[2] && drag[1] == drag[3])) { // Case point click (or hover of click = 0)
            double distX = drag[0] - bx;
            double distY = drag[1] - by;
            double dist = sqrt(pow(distX, 2) + pow(distY, 2));
            if (dist < 10 && dist < smallerDist) // If click is close enough
            {
                if (click) {
                    smallerDist = dist;
                    closest = entity;
                } else {
                    smallerDist = dist;
                    entityData->hovered = entity;
                }
            }
        } else if (bx > drag[0] && bx < drag[2] && by > drag[1] && by < drag[3]) { // If inside selection box
            if ((std::find(entityData->selected.begin(), entityData->selected.end(), entity)) !=
                entityData->selected.end()) {
                entityData->selected.erase(std::find(entityData->selected.begin(), entityData->selected.end(), entity));
            } else {
                entityData->selected.push_back(entity);
            }
        }
    }
    if (click == 1 && closest != NULL) {
        if ((std::find(entityData->selected.begin(), entityData->selected.end(), closest)) !=
            entityData->selected.end()) {
            entityData->selected.erase(std::find(entityData->selected.begin(), entityData->selected.end(), closest));
        } else {
            entityData->selected.push_back(closest);
        }
    }
}

// Draw scaled Canvas
void ProjectionCanvas::drawCanvas(unsigned Rt, double animationStep) {

    if (controller.metricLegend) {
        // Clear space for colorbar
        setSize(top_left, {bottom_right.x - 40, bottom_right.y});
    }

    glColor3d(1.0f, 1.0f, 1.0f);
    glRectd(top_left.x, top_left.y, bottom_right.x, bottom_right.y);

    // Scale initial aspect ratio by new
    glPushMatrix();
    glTranslated(xOff, yOff, 0.0);

    xRatio = currentWidth / initialWidth;
    yRatio = currentHeight / initialHeight;
    double minRatio = min(xRatio, yRatio);
    glScaled(minRatio, minRatio, 1.0);

    unsigned rMetric = (unsigned) controller.radiusMetricIndex;

    // Draw halos
    if (controller.halo) {
        for (auto entity : entityData->entities) {
            if (entity->showHalo) {
                Point p = entity->getPosition(Rt, animationStep);
                double radius = entity->normalizedData[Rt][rMetric];
                drawHalo(p.x, p.y, radius, animationStep);
            }
        }
    }

    // Draw selected entities
    for (auto selected : entityData->selected) {
        Point p = selected->getPosition(Rt, animationStep);
        double value = selected->data[Rt][rMetric];
        double radius = selected->normalizedData[Rt][rMetric];
        double delta = (Rt > 1) ? (value - selected->data[Rt - 1][rMetric]) / value : 0;
        drawEntity(p.x, p.y, radius, delta, colorSelection, 1);
    }

    // Draw hovered entities
    if (entityData->hovered != NULL) {
        if (entityData->hovered->isEntity()) {
            Entity *hovered = (Entity *) entityData->hovered;
            Point p = hovered->getPosition(Rt, animationStep);
            double value = hovered->data[Rt][rMetric];
            double radius = hovered->normalizedData[Rt][rMetric];
            double delta = (Rt > 1) ? (value - hovered->data[Rt - 1][rMetric]) / value : 0;
            drawEntity(p.x, p.y, radius, delta, colorHover, 1);
        } else if (entityData->hovered->isPackage()) {
            // Recursive lambda function to find all entities belonging to a package
            Package *hovered = (Package *) entityData->hovered;
            function<void(Package *)> f;
            f = [&](Package *p) {
                if (p == NULL)
                    return;
                else {
                    for (auto c : p->childrenVector)
                        f(&c);

                    for (auto e : p->entityVector) {
                        Point p = e.getPosition(Rt, animationStep);
                        double value = e.data[Rt][rMetric];
                        double radius = e.normalizedData[Rt][rMetric];
                        double delta = (Rt > 1) ? (value - e.data[Rt - 1][rMetric]) / value : 0;
                        drawEntity(p.x, p.y, radius, delta, colorHover, 1);
                    }
                }
            };
            f(hovered);
        }
    }

    // Draw all entities
    for (auto b : entityData->entities) {
        Point p = b->getPosition(Rt, animationStep);
        Color c = getColor(controller.colormapIndex, b, Rt);
        double radius = b->normalizedData[Rt][rMetric];
        double delta = (Rt > 1) ? (b->data[Rt][rMetric] - b->data[Rt - 1][rMetric]) / b->data[Rt][rMetric] : 0;
        drawEntity(p.x, p.y, radius, delta, c, 0);
    }

    glPopMatrix();

    if (controller.metricLegend) {
        displayRadiusLegend();
        displayColorbar();
    }
}

// Draw shadow
void ProjectionCanvas::drawHalo(double x, double y, double radius, double animationStep) {

    int triangleAmount = 360; //# of triangles/degrees used to draw circle

    radius = (radius * 20 + 7) * 2; // Ugly as frick

    // Create the opacity curve like a triangle
    animationStep = (animationStep < 0) ? animationStep * (-1) : animationStep;
    double opacity = (animationStep < 0.5) ? 2 * animationStep : 1 - (animationStep - 0.5) * 2;

    double radians = 2.0f * PI;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_TRIANGLE_FAN);
    glColor4d(0, 0, 0, opacity);
    glVertex3d(x, y, 0);
    glColor4d(0, 0, 0, 0);
    for (int i = 0; i <= triangleAmount; i++) {
        glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
                   y + (radius * sin(i * radians / triangleAmount)), 0);
    }
    glEnd();
}


// Draw circles
void ProjectionCanvas::drawEntity(double x, double y, double radius, double delta, Color color, int action) {

    if (controller.deltaPie == 0 || fabs(delta) < 0.01) // 1% tolerance
        drawSolidEntity(x, y, radius, color, action);
    else
        drawPieEntity(x, y, radius, delta, color, action);
}

void ProjectionCanvas::drawSolidEntity(double x, double y, double radius, Color color, int action) {

    radius = radius * 20 + 3 + action * 4; // Scaling
    if (color == colorHover)
        radius += 2;

    // Use 1px black border
    drawCircle(x, y, 0, radius, Color {0, 0, 0});
    drawCircle(x, y, 0, radius - 1, color);
}

// Draw smooth circles
void ProjectionCanvas::drawCircle(double x, double y, double z, double radius, Color color) {

    double triangleAmount = 360; //# of triangles/degrees used to draw circle
    double radians = 2.0f * PI;
    double smoothStep = 1;

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= triangleAmount; ++i) {
        glColor3d(color.R, color.G, color.B);
        glVertex3d(x + ((radius - smoothStep) * cos(i * radians / triangleAmount)),
                   y + ((radius - smoothStep) * sin(i * radians / triangleAmount)), z);

        glColor4d(color.R, color.G, color.B, 0.0);
        glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
                   y + (radius * sin(i * radians / triangleAmount)), z);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3d(color.R, color.G, color.B);
    glVertex3d(x, y, 0);
    for (int i = 0; i <= triangleAmount; ++i) {
        glVertex3d(x + ((radius - smoothStep) * cos(i * radians / triangleAmount)),
                   y + ((radius - smoothStep) * sin(i * radians / triangleAmount)), z);
    }
    glEnd();
}

void ProjectionCanvas::drawPieEntity(double x, double y, double radius, double delta, Color c, int action) {

    int triangleAmount = 360; //# of triangles/degrees used to draw circle

    radius = radius * 20 + 3 + action * 4; // Scaling
    if (c == colorHover)
        radius += 2;

    double radians = 2.0f * PI;
    if (controller.deltaPie)
        radians *= (1 - fabs(delta)); // Take only a fraction of the 360 degrees

    // Old indian trick to rotate a circle around it's center
    glPushMatrix();
    glColor3d(c.R, c.G, c.B);
    double rotation = 0;
    glTranslated(x, y, 0); // Translate vector to the object's position

    // Define removed slice accordingly to delta value
    if (delta > 0)
        rotation = -90 + delta * 180;
    else
        rotation = 90 - delta * 180;

    glRotated(rotation, 0, 0, 1); // Use rotation matrix (clock-wise)

    glTranslated(-x, -y, 0); // Translate to normal origin

    // Draw circle with missing slice
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(x, y, 0);
    for (int i = 0; i <= triangleAmount; i++) // Draw x percent of circle
    {
        glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
                   y + (radius * sin(i * radians / triangleAmount)), 0);
    }
    glEnd();

    // Draw stroke
    glBegin(GL_LINE_STRIP);
    glColor3d(0, 0, 0);
    glVertex3d(x, y, 0);
    for (int i = 0; i <= triangleAmount; i++) // Draw x percent of circle
    {
        glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
                   y + (radius * sin(i * radians / triangleAmount)), 0);
    }
    glVertex3d(x, y, 0);
    glEnd();

    glPopMatrix();

    // Draw missing slice
    if (delta > 0)
        radius *= 1.2;
    else
        radius = 0;

    radians = 2 * PI - radians;

    glPushMatrix();
    glColor3d(c.R, c.G, c.B);
    glTranslated(x, y, 0); // Translate vector to the object's position

    // Define removed slice accordingly to delta value
    if (delta > 0)
        rotation = -90 - delta * 180;
    else
        rotation = 90 + delta * 180;

    glRotated(rotation, 0, 0, 1); // Use rotation matrix (clock-wise)

    glTranslated(-x, -y, 0); // Translate to normal origin

    // Draw missing slice
    glBegin(GL_TRIANGLE_FAN);
    glVertex3d(x, y, 0);
    for (int i = 0; i <= triangleAmount; i++) // Draw x percent of circle
    {
        glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
                   y + (radius * sin(i * radians / triangleAmount)), 0);
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3d(0, 0, 0);
    glTranslated(x, y, 0); // Translate vector to the object's position

    // Define removed slice accordingly to delta value
    if (delta > 0)
        rotation = -90 - delta * 180;
    else
        rotation = 90 + delta * 180;

    glRotated(rotation, 0, 0, 1); // Use rotation matrix (clock-wise)

    glTranslated(-x, -y, 0); // Translate to normal origin

    // Draw missing slice
    glBegin(GL_LINE_STRIP);
    glLineWidth(2.0);
    glVertex3d(x, y, 0);
    for (int i = 0; i <= triangleAmount; i++) // Draw x percent of circle
    {
        glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
                   y + (radius * sin(i * radians / triangleAmount)), 0);
    }
    glVertex3d(x, y, 0);
    glEnd();
    glPopMatrix();
}

void ProjectionCanvas::displayRadiusLegend() {

    double radiusMaxMetricValue = entityData->maxMetricValue[controller.radiusMetricIndex];
    double radiusMinMetricValue = entityData->minMetricValue[controller.radiusMetricIndex];

    // Format and print min/max metric values
    stringstream stream;
    stream << fixed << setprecision(2) << radiusMinMetricValue;
    string min = stream.str();
    min.replace(min.find(".00"), 3, "");

    stream.str(""); // Clear stream
    stream << fixed << setprecision(2) << radiusMaxMetricValue;
    string max = stream.str();
    max.replace(max.find(".00"), 3, "");

    string range = min + " - " + max;

    glColor3d(0, 0, 0);
    glRasterPos2d(currentWidth - (range.length() + 1) * 9, 30);
    const unsigned char *s = reinterpret_cast<const unsigned char *>(range.c_str());
    glutBitmapString(GLUT_BITMAP_9_BY_15, s);

    // Draw min max radius circles
    int triangleAmount = 360; //# of triangles/degrees used to draw circle
    double radians = 2.0f * PI;
    double radius = MAX_RADIUS * minRatio;
    double x = currentWidth - radius;
    double y = 60;
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= triangleAmount; ++i) {
        glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
                   y + (radius * sin(i * radians / triangleAmount)), 0);
    }
    glEnd();

    radius = MIN_RADIUS * minRatio;
    x -= 50;
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= triangleAmount; ++i) {
        glVertex3d(x + (radius * cos(i * radians / triangleAmount)),
                   y + (radius * sin(i * radians / triangleAmount)), 0);
    }
    glEnd();

    glColor4d(0, 0, 0, 0.1);
    glRectd(currentWidth + 10, 10, currentWidth - 100, 90);
}

void ProjectionCanvas::displayColorbar() {

    double colorMaxMetricValue = entityData->maxMetricValue[controller.colorMetricIndex];
    double colorMinMetricValue = entityData->minMetricValue[controller.colorMetricIndex];

    double stepSize = currentHeight / 256;
    double l = 0;
    for (unsigned i = 0; i < 256; ++i) {
        double value = double(i) / 256;

        Color c {1, 1, 1};
        if (controller.colormapIndex == (int) COLORMAP::sequential) {
            c = sequentialColormap(value);
        } else if (controller.colormapIndex == (int) COLORMAP::divergent) {
            value -= 0.5;
            c = divergentColormap(value);
        }
        glColor3d(c.R, c.G, c.B);

        glRectd(bottom_right.x + 10, bottom_right.y - l, bottom_right.x + 35, bottom_right.y - l - stepSize);
        l += stepSize;
    }

    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(currentWidth + 39, currentHeight, 0);
    glScalef(0.15f, -0.15f, 0);
    glRotated(90, 0, 0, 1);

    string max = "", min = "";
    if (controller.colormapIndex == (int) COLORMAP::sequential) {
        stringstream streamMin, streamMax;
        streamMin << fixed << setprecision(2) << colorMinMetricValue;
        min = streamMin.str();
        min.replace(min.find(".00"), 3, "");

        streamMax.str("");
        streamMax << fixed << setprecision(2) << colorMaxMetricValue;
        max = streamMax.str();
        max.replace(max.find(".00"), 3, "");
    } else if (controller.colormapIndex == (int) COLORMAP::divergent) {
        max = "0.5";
        min = "-0.5";
    }

    const unsigned char *s = reinterpret_cast<const unsigned char *>(min.c_str());
    glutStrokeString(GLUT_STROKE_ROMAN, s);
    glPopMatrix();

    s = reinterpret_cast<const unsigned char *>(max.c_str());
    glColor3d(1, 1, 1);
    glPushMatrix();
    glTranslated(currentWidth + 39, max.length() * 16 + 2, 0);
    glScalef(0.15f, -0.15f, 0);
    glRotated(90, 0, 0, 1);

    glutStrokeString(GLUT_STROKE_ROMAN, s);
    glPopMatrix();
}
