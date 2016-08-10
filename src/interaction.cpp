#include "../include/interaction.h"

unsigned ctrlDown = 0;
GLUI *glui;
int radiusMetricIndex=21;
int colorMetricIndex=21;
int streamMetricIndex=21;
int colormapIndex=0;
int deltaPie = 0;
int streamgraphFlag = 0;
int dynamicTreemap = 0;

void initializeUI()
{
	glui = GLUI_Master.create_glui( "GLUI" );
	// Metric listboxes
	glui->add_statictext("Color Metric");
	GLUI_Listbox *colorLB = glui->add_listbox("", &colorMetricIndex, COLORMETRIC_LB, controlCB);
	glui->add_statictext("Radius Metric");
	GLUI_Listbox *radiusLB = glui->add_listbox("", &radiusMetricIndex, RADIUSMETRIC_LB, controlCB);
	glui->add_statictext("Streamgraph Metric");
	GLUI_Listbox *streamLB = glui->add_listbox("", &streamMetricIndex, STREAMMETRIC_LB, controlCB);
	glui->add_statictext("Colormap");
	for (unsigned i = 0; i < entityTree->metricVector.size(); ++i)
	{
		colorLB->add_item(i, entityTree->metricVector[i].c_str());
		radiusLB->add_item(i, entityTree->metricVector[i].c_str());
		streamLB->add_item(i, entityTree->metricVector[i].c_str());
	}

	// Colormap listbox
	GLUI_Listbox *colormapLB = glui->add_listbox("", &colormapIndex, COLORMAP_LB, controlCB);
	colormapLB->add_item(0, "Sequential Colormap");
	colormapLB->add_item(1, "Qualitative Colormap (Hierarchy)");
	colormapLB->add_item(2, "Divergent Colormap");
	glui->add_checkbox("Display delta pie slice", &deltaPie);
	glui->add_checkbox("Stream Graph", &streamgraphFlag);
	glui->add_checkbox("Dynamic Treemap", &dynamicTreemap);

	entityTree->setColorMetric(colorMetricIndex);
	entityTree->setRadiusMetric(radiusMetricIndex);
	entityTree->setStreamMetric(streamMetricIndex);

}

// Callback handling
void controlCB(int control)
{
	switch (control)
	{
		case COLORMETRIC_LB:
			entityTree->setColorMetric(colorMetricIndex);
		 	break;
		case RADIUSMETRIC_LB:
			entityTree->setRadiusMetric(radiusMetricIndex);
			break;
		case STREAMMETRIC_LB:
			entityTree->setStreamMetric(streamMetricIndex);
			break;
	}
}

void mouseClick(int button, int state, int x, int y)
{
	int drag[4] = {0,0,0,0};
	switch (mouse->click(button, state, x, y, drag))
	{
		case 1:
			pCanvas->getEntitiesByPositionOnProjection(drag, Rt, 1, ctrlDown);
			break;
		case 2:
			tCanvas->getEntitiesByPositionOnTreemap(drag, 1, ctrlDown);
			break;
	}
	mouse->updateMouse(x, y);
}

void mousePassive (int x, int y)
{
	mouse->updateMouse(x, y);
	if (mouse->canvas == 3) // Hovering streamgraph
	{
		int drag[4] = {mouse->x,mouse->y,mouse->x,mouse->y};
		sCanvas->getEntitiesOnStreamgraph(drag, 0, ctrlDown);
		hover = entityTree->hovered;
	}
	else if (mouse->canvas == 2) // Hovering treemap
	{
		int drag[4] = {mouse->x,mouse->y,mouse->x,mouse->y};
		tCanvas->getEntitiesByPositionOnTreemap(drag, 0, ctrlDown);
		hover = entityTree->hovered;
	}
	else if (mouse->canvas == 1) // Hovering projection
	{
		int drag[4] = {mouse->x,mouse->y,mouse->x,mouse->y};
		pCanvas->getEntitiesByPositionOnProjection(drag, Rt, 0, ctrlDown);
		hover = entityTree->hovered;
	}
	else
	{
		hover = NULL;
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
		case 'q': exit(0); break;
		case 'z': if (Rt > 0) --Rt; cout<<Rt<<endl; break;
		case 'x': if (Rt < entityTree->nRevisions-1) ++Rt; break;
	}
}

void keyboardMod(int key, int x, int y)
{
	if (key == (int)114) // Check for ctrl
		ctrlDown = (ctrlDown == 0)? 1 : 0;
}
