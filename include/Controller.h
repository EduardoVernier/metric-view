#pragma once

#include <queue>

enum hierView { TREEMAP, SUNBURST };

class Controller
{
public:
	int winWidth = 1920, winHeight = 1056;
	int mainWindow;
	int hierarchicalView = TREEMAP;

	int radiusMetricIndex = 21;
	int colorMetricIndex = 21;
	int streamMetricIndex = 21;
	int colormapIndex = 0;
	int sColormapIndex = 2;
	int accelerationRatioIndex = 1;

	int deltaPie = 0;
	bool ctrlDown = false;
	int streamgraphFlag = 0;
	int dynamicTreemap = 0;
	int halo = 0;

	int streamgraphHeight = 250;

	int animationDirection = 0;
	double animationStep = 1.0;
	double accelerationRatio = 1;

	queue<short> windowQueue;
};
