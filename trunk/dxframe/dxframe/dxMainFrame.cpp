#include "dxMainFrame.h"

dxMainFrame::dxMainFrame () {
	
	obj0 = NULL;
	obj1 = NULL;
}

dxMainFrame::~dxMainFrame () {
	
}

void dxMainFrame::Create () {
	
	//todo: create dxObjes here

	obj0 = new dxObj;
	obj0->CreateFromFile (_T("test.dxf"), "Plane01");

	obj1 = new dxObj;
	obj1->CreateFromFile (_T("test.dxf"), "Plane02");
}

void dxMainFrame::Update (float dt) {
	
	static float c = 0;
	c += dt;

	//obj1->Move (dt, 10, 10);
}

void dxMainFrame::Destroy () {
	
}