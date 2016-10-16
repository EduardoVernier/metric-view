#include "../include/interaction.h"
#include "../include/GL/glui.h"

GLUI *glui;

void initializeUI()
{
	glui = GLUI_Master.create_glui_subwindow(controller.mainWindow, GLUI_SUBWINDOW_LEFT);
	glui->set_main_gfx_window(controller.mainWindow);

	// Metric listboxes
	glui->add_statictext("Projection and Hierarchical");
	glui->add_statictext("View Color Metric");
	GLUI_Listbox *colorLB = glui->add_listbox("", &controller.colorMetricIndex, COLORMETRIC_LB);

	// Colormap listbox
	glui->add_statictext("Colormap");
	GLUI_Listbox *colormapLB = glui->add_listbox("", &controller.colormapIndex, COLORMAP_LB, controlCB);
	colormapLB->add_item(0, "Sequential Colormap");
	colormapLB->add_item(1, "Qualitative Colormap");
	colormapLB->add_item(2, "Divergent Colormap");

	glui->add_statictext("Hierarchical View");
	GLUI_Listbox *hierarchicalLB = glui->add_listbox("", &controller.hierarchicalView, HIERARCHICAL_LB, controlCB);
	hierarchicalLB->add_item(TREEMAP, "Treemap");
	hierarchicalLB->add_item(SUNBURST, "Sunburst");

	glui->add_separator();


	glui->add_statictext("Radius Metric");
	GLUI_Listbox *radiusLB = glui->add_listbox("", &controller.radiusMetricIndex, RADIUSMETRIC_LB);
	glui->add_separator();


	glui->add_statictext("Evolution View Metric");
	GLUI_Listbox *streamLB = glui->add_listbox("", &controller.evolutionMetricIndex, STREAMMETRIC_LB);
	for (unsigned i = 0; i < entityData->metricVector.size(); ++i)
	{
		colorLB->add_item(i, entityData->metricVector[i].c_str());
		radiusLB->add_item(i, entityData->metricVector[i].c_str());
		streamLB->add_item(i, entityData->metricVector[i].c_str());
	}

	glui->add_statictext("Evolution View");
	GLUI_Listbox *evolutionLB = glui->add_listbox("", &controller.evolutionView, HIERARCHICAL_LB, controlCB);
	evolutionLB->add_item(HIDE, "None");
	evolutionLB->add_item(STREAMGRAPH, "Streamgraph");
	evolutionLB->add_item(SPECTROGRAPH, "Spectrograph");

	glui->add_statictext("Evolution View Colormap");
	GLUI_Listbox *sColormapLB = glui->add_listbox("", &controller.evolutionColormapIndex, SCOLORMAP_LB, controlCB);
	sColormapLB->add_item(0, "Sequential Colormap");
	sColormapLB->add_item(2, "Divergent Colormap");
	glui->add_separator();


	// Animation speed listbox
	glui->add_statictext("Animation Speed");
	GLUI_Listbox *accelerationLB = glui->add_listbox("", &controller.accelerationRatioIndex, ACCELERATION_LB, controlCB);
	accelerationLB->add_item(0,"10x");
	accelerationLB->add_item(1,"5x");
	accelerationLB->add_item(2,"2x");
	accelerationLB->add_item(3,"1x");
	accelerationLB->add_item(4,"0.75x");
	accelerationLB->add_item(5,"0.5x");
	accelerationLB->add_item(6,"0.1x");
	accelerationLB->add_item(7,"0.05x");

	glui->add_checkbox("Display delta pie slice", &controller.deltaPie);
	glui->add_checkbox("Dynamic Treemap", &controller.dynamicTreemap);
	glui->add_checkbox("Enable halos", &controller.halo);

	glui->sync_live();
	toggleControlWindowVisibility();
}

// Callback handling
void controlCB(int control)
{
	switch (control)
	{
		case ACCELERATION_LB:
			switch (controller.accelerationRatioIndex)
			{
				case 0:
					controller.accelerationRatio = 10;
					break;
				case 1:
					controller.accelerationRatio = 5;
					break;
				case 2:
					controller.accelerationRatio = 2;
					break;
				case 3:
					controller.accelerationRatio = 1;
					break;
				case 4:
					controller.accelerationRatio = 0.75;
					break;
				case 5:
					controller.accelerationRatio = 0.5;
					break;
				case 6:
					controller.accelerationRatio = 0.1;
					break;
				case 7:
					controller.accelerationRatio = 0.05;
					break;

			}
			break;
	}
}

void mouseClick(int button, int state, int x, int y)
{
	// Terrible way to avoid misclick - unfortunately necessary
	unsigned long queueSize = controller.windowQueue.size();
	unsigned sum = 0;
	while (!controller.windowQueue.empty())
	{
		sum += controller.windowQueue.front();
		controller.windowQueue.pop();
	}
	if (sum > queueSize)
		return;

	vector<Entity*> selected = entityData->selected;
	// Find selected entities on entityData
	int drag[4] = {0,0,0,0};
	switch (mouse->click(button, state, x, y, drag))
	{
		case 1:
			pCanvas->getEntitiesByPositionOnProjection(drag, Rt, 1, controller.ctrlDown);
			break;
		case 2:
			if (controller.hierarchicalView == TREEMAP)
				tCanvas->getEntitiesByPositionOnTreemap(drag, 1, controller.ctrlDown);
			else
				sbCanvas->getEntitiesByPosition(drag, 1, controller.ctrlDown);
			break;
		default:break;
	}

	if (selected != entityData->selected) // Change detected in the selected group
	{
		entityData->updateSelectedEntities();
		mRank->computeLocalGroupMetric(Rt);
	}

	mouse->updateMouse(x, y);
}

void mousePassive (int x, int y)
{
	mouse->updateMouse(x, y);
	if (mouse->canvas == 3) // Hovering streamgraph
	{
		int drag[4] = {mouse->x,mouse->y,mouse->x,mouse->y};
		if (controller.evolutionView == STREAMGRAPH)
		{
			stCanvas->getEntitiesOnStreamgraph(drag, 0, controller.ctrlDown);
		}
		else if (controller.evolutionView == SPECTROGRAPH)
		{
			SpectrographCanvas::getInstance().getEntitiesOnSpectrograph(drag, 0, controller.ctrlDown);
		}
	}
	else if (mouse->canvas == 2) // Hovering treemap
	{
		int drag[4] = {mouse->x,mouse->y,mouse->x,mouse->y};
		if (controller.hierarchicalView == TREEMAP)
			tCanvas->getEntitiesByPositionOnTreemap(drag, 0, controller.ctrlDown);
		else
			sbCanvas->getEntitiesByPosition(drag, 0, controller.ctrlDown);
	}
	else if (mouse->canvas == 1) // Hovering projection
	{
		int drag[4] = {mouse->x,mouse->y,mouse->x,mouse->y};
		pCanvas->getEntitiesByPositionOnProjection(drag, Rt, 0, controller.ctrlDown);
	}
	else
	{
		entityData->hovered = NULL;
	}
}

void mouseUpdate(int x, int y)
{
	mouse->updateMouse(x, y);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'q':
			glutLeaveMainLoop();
			break;
		case 'z':
			if (Rt > 0)
			{
				--Rt;
				controller.animationDirection = -1;
				controller.animationStep = 0.0;
				mRank->computeLocalGroupMetric(Rt);
				entityData->rankFastestChangingEntities(Rt, controller.animationDirection );
			}
			break;
		case 'x':
			if (Rt < entityData->nRevisions-1)
			{
				++Rt;
				controller.animationDirection = 1;
				controller.animationStep = 0.0;
				mRank->computeLocalGroupMetric(Rt);
				entityData->rankFastestChangingEntities(Rt, controller.animationDirection );
			}
			break;
		case 'c':
			toggleControlWindowVisibility();
			break;
		default:break;
	}
}


void toggleControlWindowVisibility() {
	controller.displayControlWindow = !controller.displayControlWindow;
	int vx, vy, vw, vh;
	if (controller.displayControlWindow)
	{
		glui->show();
		GLUI_Master.get_viewport_area(&vx, &vy, &vw, &vh);
		controller.viewportXOffset = vx;
	}
	else
	{
		glui->hide();
		GLUI_Master.get_viewport_area(&vx, &vy, &vw, &vh);
		controller.viewportXOffset = 0;
	}

	reshape(controller.winWidth, controller.winHeight);
}

void keyboardMod(int key, int x, int y)
{
	if (key == (int)114) // Check for ctrl
		controller.ctrlDown = !controller.ctrlDown;
}
