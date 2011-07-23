#include "dxMainFrame.h"

dxMainFrame::dxMainFrame () {

}

dxMainFrame::~dxMainFrame () {
	
}

void dxMainFrame::Create () {
	
	//todo: create dxObjes here

	DEL (obj2);
	obj2 = new dxObj;
	obj2->CreateFromFile (_T("test.dxf"), "Plane01");

	DEL (obj);
	obj = new dxObj;
	obj->CreateFromFile (_T("test.dxf"), "Plane02");
}

void dxMainFrame::Update (float dt) {
	
	static float c = 0;
	c += dt;

	if (obj) {
		obj->RotateZ (0.001f);
	}
}

void dxMainFrame::Destroy () {
	
}