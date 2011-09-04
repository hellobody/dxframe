#include "dxMainFrame.h"

extern dxInput input;

dxMainFrame::dxMainFrame () {
	
	
}

dxMainFrame::~dxMainFrame () {
	
}

void dxMainFrame::Create () {
	
	//todo: create dxObjes here

	objs.push_back ();

	/*obj = new dxObj;
	obj->CreateFromFile (_T("test.dxf"), "Plane01");*/
}

void dxMainFrame::Update (float dt) {
	

	//obj->RotateZ (dt * .25f);

	/*if (obj0->IsPick (input.GetCursorPosition ()->x, input.GetCursorPosition ()->y))
	{
		
	}*/
}

void dxMainFrame::Destroy () {
	
}