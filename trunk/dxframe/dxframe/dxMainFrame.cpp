#include "dxMainFrame.h"

extern dxInput input;

dxMainFrame::dxMainFrame () {
	
	background = NULL;
	b_default = NULL;
	b_selected = NULL;
}

dxMainFrame::~dxMainFrame () {
	
}

void dxMainFrame::Create () {
	
	//todo: create dxObjes here

	background = new dxObj;
	b_default = new dxObj;
	b_selected = new dxObj;

	background->CreateFromFile (_T("main_menu.dxf"), "background");
	b_default->CreateFromFile (_T("main_menu.dxf"), "button_play_default");
	b_selected->CreateFromFile (_T("main_menu.dxf"), "button_play_selected");
}

void dxMainFrame::Update (float dt) {
	

	//obj->RotateZ (dt * .25f);

	/*if (obj0->IsPick (input.GetCursorPosition ()->x, input.GetCursorPosition ()->y))
	{
		
	}*/
}

void dxMainFrame::Destroy () {
	
}