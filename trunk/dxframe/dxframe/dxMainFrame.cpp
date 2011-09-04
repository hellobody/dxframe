#include "dxMainFrame.h"

extern dxInput input;

dxMainFrame::dxMainFrame () {
	
	obj = NULL;
	obj0 = NULL;
	obj1 = NULL;
}

dxMainFrame::~dxMainFrame () {
	
}

void dxMainFrame::Create () {
	
	//todo: create dxObjes here

	obj = new dxObj;
	obj->CreateFromFile (_T("test.dxf"), "Box01");

	/*obj0 = new dxObj;
	obj0->CreateFromFile (_T("test.dxf"), "Box01");*/
}

void dxMainFrame::Update (float dt) {
	
	static float c = 0;
	c += dt;

	//obj1->Move (dt, 10, 10);

	/*if (input.IsLeftMouseKeyToggledDown ()) {
		TCHAR tStr [MAX_PATH];
		_stprintf (tStr, _T("%i, %i"), input.GetCursorPosition ()->x, input.GetCursorPosition ()->y);
		trace (tStr);
	}*/

	/*if (obj1->IsPick (input.GetCursorPosition ()->x, input.GetCursorPosition ()->y))
	{
		int a = 0;
	}*/
}

void dxMainFrame::Destroy () {
	
}