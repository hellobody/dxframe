#include "dxMainFrame.h"

dxMainFrame::dxMainFrame () {
	obj = NULL;
	obj2 = NULL;
	testTextField = NULL;
}

dxMainFrame::~dxMainFrame () {
	
}

void dxMainFrame::Create () {
	
	//todo: create dxObjes here
	DEL (obj);
	obj = new dxObj;
	obj->CreateFromFile (_T("test.dxf"), "Plane01");

	DEL (obj2);
	obj2 = new dxObj;
	obj2->CreateFromFile (_T("test.dxf"), "M_03");

	//testTextField = new dxTextField (_T("Hello you!!!"), dxRect (250, 500, 50, 100));
}

void dxMainFrame::Update (float dt) {
	
	static float c = 0;
	c += dt;

	
}

void dxMainFrame::Destroy () {
	
}