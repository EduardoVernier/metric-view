#include "../include/Colormap.h"
#include "../include/interaction.h"

Color getColor(int colormap, BaseEntity *b, unsigned Rt) {

    int cMetric = controller.colorMetricIndex;
    double value;

    if (b->isEntity()) {
        value = ((Entity *) b)->normalizedData[Rt][cMetric];
    } else {
        Package *p = (Package *) b;
        value = 0;
        for (Entity entity : p->entityVector)
            value += entity.normalizedData[Rt][cMetric];
        value /= p->entityVector.size();
    }

    Color c(1, 1, 1);
    switch (colormap) {
        case (int) COLORMAP::sequential:
            c = sequentialColormap(value);
            break;
        case (int) COLORMAP::qualitative:
            c = qualitativeColormap(b->getFirstLevelId());
            break;
        case (int) COLORMAP::divergent:
            if (Rt > 0) {
                double pValue = ((Entity *) b)->normalizedData[Rt - 1][cMetric];
                c = divergentColormap(value - pValue);
            }
            break;
        default:
            break;
    }
    return c;
}

/* Blue to red colormap */
Color divergent[5] = {
        Color(0.019, 0.443, 0.690),
        Color(0.572, 0.733, 0.870),
        Color(0.968, 0.968, 0.968),
        Color(0.956, 0.647, 0.509),
        Color(0.792, 0, 0.125)};


/* Beige to red colormap */
Color sequential[5] = {
        Color(0.996, 0.941, 0.850),
        Color(0.992, 0.8, 0.541),
        Color(0.988, 0.552, 0.349),
        Color(0.890, 0.290, 0.2),
        Color(0.701, 0, 0)};

Color sequentialColormap(double v) {
    double d = v;
    while (d > 0.25) d -= 0.25;
    d *= 4;

    int f = int(floor(v * 4));
    int c = int(ceil(v * 4));
    return (sequential[f] * (1 - d)) + (sequential[c] * (d));
}

Color divergentColormap(double v) {
    double diff = (v) * 2 + 0.5; // 5 is the "amplification" level
    if (diff > 1)
        diff = 1;
    else if (diff < 0)
        diff = 0;

    double d = diff;
    while (d > 0.25) d -= 0.25;
    d *= 4;

    int f = int(floor(diff * 4));
    int c = int(ceil(diff * 4));
    return (divergent[f] * (1 - d)) + (divergent[c] * (d));
}

Color qualitativeColormap(int v) {
    int cm[24][3] =
            {{166, 206, 227},
             {31,  120, 180},
             {178, 223, 138},
             {51,  160, 44},
             {251, 154, 153},
             {227, 26,  28},
             {253, 191, 111},
             {255, 127, 0},
             {202, 178, 214},
             {106, 61,  154},
             {255, 255, 153},
             {177, 89,  40},
             {141, 211, 199},
             {255, 255, 179},
             {190, 186, 218},
             {251, 128, 114},
             {128, 177, 211},
             {253, 180, 98},
             {179, 222, 105},
             {252, 205, 229},
             {217, 217, 217},
             {188, 128, 189},
             {204, 235, 197},
             {255, 237, 111}};

    return Color(cm[v % 24][0] / 255.0, cm[v % 24][1] / 255.0, cm[v % 24][2] / 255.0);
}
