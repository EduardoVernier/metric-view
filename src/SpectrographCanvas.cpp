#include "../include/SpectrographCanvas.h"

SpectrographCanvas::SpectrographCanvas() {}

SpectrographCanvas &SpectrographCanvas::getInstance() {

    static SpectrographCanvas instance{};
    return instance;
}

void SpectrographCanvas::init(Point tl, Point br, EntityData *ed) {

    setSize(tl, br);
    entityData = ed;
}

double SpectrographCanvas::getHeight() {

    double desiredHeight = entityData->selected.size() * MAX_LINE_HEIGHT;
    return (desiredHeight < 500) ? desiredHeight : 500; // Set 500 as limit for now
}

void SpectrographCanvas::drawCanvas(unsigned Rt, double animationStep) {

    if (controller.metricLegend) {
        setSize(top_left, {bottom_right.x - 40, bottom_right.y});
    }

    // Translate everything into frame
    glPushMatrix();
    glTranslated(top_left.x, top_left.y, 0);

    double cellHeight = (getHeight() - 10) / entityData->selected.size();
    double cellWidth = currentWidth / getInstance().entityData->nRevisions;
    int sMetric = controller.evolutionMetricIndex;

    // Draw cells
    for (unsigned i = 0; i < entityData->selected.size(); ++i) {
        Entity *entity = entityData->selected[i];
        for (unsigned revision = 0; revision < entityData->nRevisions; ++revision) {
            double value = entity->normalizedData[revision][sMetric];

            Color c{1, 1, 1};
            if (controller.evolutionColormapIndex == (int) COLORMAP::sequential) {
                c = sequentialColormap(value);
            } else if (controller.evolutionColormapIndex == (int) COLORMAP::divergent) {
                if (revision > 0) {
                    double pValue = entity->normalizedData[revision - 1][sMetric];
                    c = divergentColormap(value - pValue);
                } else {
                    c = divergentColormap(0.0);
                }
            }
            glColor3d(c.R, c.G, c.B);

            glRectd(revision * cellWidth, i * cellHeight,
                    (revision + 1) * cellWidth, (i + 1) * cellHeight);

        }
        // Draw hover indication
        if (entityData->hovered != NULL) {
            if (entity->getName() == entityData->hovered->getName()) {
                glColor3d(colorHover.R, colorHover.G, colorHover.B);

                glRectd(-2, i * cellHeight,
                        -9, (i + 1) * cellHeight);
            }
        }
    }

    // Draw separation
    if (getHeight() < 450) {
        for (unsigned i = 1; i < entityData->selected.size(); ++i) {
            glColor4f(1, 1, 1, 1);
            glBegin(GL_LINES);
            glVertex2d(0, i * cellHeight);
            glVertex2d(currentWidth, i * cellHeight);
            glEnd();
        }
    }

    // Draw current Rt "window"
    glColor4f(1, 0, 0, 0.8);
    glBegin(GL_LINE_STRIP);
    double Rtp = (animationStep != 0.0 && animationStep > 0) ? Rt + animationStep : (Rt + 2) + (animationStep);
    glVertex2d((Rtp - 1) * cellWidth, 0);
    glVertex2d((Rtp - 1) * cellWidth, currentHeight);
    glVertex2d((Rtp) * cellWidth, currentHeight);
    glVertex2d((Rtp) * cellWidth, 0);
    glVertex2d((Rtp - 1) * cellWidth, 0);
    glEnd();

    glPopMatrix();

    if (controller.metricLegend) {
        displayColorbar();
    }
}

void SpectrographCanvas::getEntitiesOnSpectrograph(int *drag, unsigned click, bool ctrlDown) {

    double yB = drag[1] - top_left.y + 10;
    double yE = drag[3] - top_left.y + 10;
    double cellHeight = getHeight() / entityData->selected.size();
    unsigned indexBegin = (unsigned) (yB / cellHeight);
    unsigned indexEnd = (unsigned) (yE / cellHeight);


    if (click) {
        if (indexBegin == indexEnd) {
            entityData->selected.clear();
            entityData->selected.push_back(entityData->selected[indexBegin]);
        } else {
            for (unsigned i = indexBegin; i < indexEnd; ++i) {
                std::vector<Entity*> temp(entityData->selected.begin() + indexBegin, entityData->selected.begin() + indexEnd);
                entityData->selected = temp;
            }
        }
    }
    else {
        entityData->hovered = entityData->selected[indexBegin];
    }
}

void SpectrographCanvas::displayColorbar() {

    double colorMaxMetricValue = entityData->maxMetricValue[controller.evolutionMetricIndex];
    double colorMinMetricValue = entityData->minMetricValue[controller.evolutionMetricIndex];

    double stepSize = currentHeight / 256;
    double l = 0;
    for (unsigned i = 0; i < 256; ++i) {
        double value = double(i) / 256;

        Color c{1, 1, 1};
        if (controller.evolutionColormapIndex == (int) COLORMAP::sequential) {
            c = sequentialColormap(value);
        } else if (controller.evolutionColormapIndex == (int) COLORMAP::divergent) {
            c = divergentColormap(value - 0.5);
        }
        glColor3d(c.R, c.G, c.B);

        glRectd(bottom_right.x + 10, bottom_right.y - l, bottom_right.x + 35, bottom_right.y - l - stepSize);
        l += stepSize;
    }

    glColor3d(0, 0, 0);
    glPushMatrix();
    glTranslated(currentWidth + 39, yOff + currentHeight, 0);
    glScalef(0.15f, -0.15f, 0);
    glRotated(90, 0, 0, 1);

    string max = "", min = "";
    if (controller.evolutionColormapIndex == (int) COLORMAP::sequential) {
        stringstream streamMin, streamMax;
        streamMin << fixed << setprecision(2) << colorMinMetricValue;
        min = streamMin.str();
        min.replace(min.find(".00"), 3, "");

        streamMax.str("");
        streamMax << fixed << setprecision(2) << colorMaxMetricValue;
        max = streamMax.str();
        max.replace(max.find(".00"), 3, "");
    } else if (controller.evolutionColormapIndex == (int) COLORMAP::divergent) {
        max = "0.5";
        min = "-0.5";
    }


    glColor3d(0, 0, 0);
    const unsigned char *s = reinterpret_cast<const unsigned char *>(min.c_str());
    glutStrokeString(GLUT_STROKE_ROMAN, s);
    glPopMatrix();

    s = reinterpret_cast<const unsigned char *>(max.c_str());
    glColor3d(1, 1, 1);
    glPushMatrix();
    glTranslated(currentWidth + 39, yOff + max.length() * 16 - 5, 0);
    glScalef(0.15f, -0.15f, 0);
    glRotated(90, 0, 0, 1);

    glutStrokeString(GLUT_STROKE_ROMAN, s);
    glPopMatrix();
}
