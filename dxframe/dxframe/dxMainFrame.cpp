#include "dxMainFrame.h"

extern dxInput input;

dxMainFrame::dxMainFrame () {
	
	background = NULL;
	b_default = NULL;
	b_selected = NULL;

	bPlay = NULL;
}

dxMainFrame::~dxMainFrame () {
	
}

void dxMainFrame::Create () {
	
	//todo: create dxObjes here

	background = new dxObj;
	

	background->CreateFromFile (_T("main_menu.dxf"), "background");

	bPlay = new dxButton (_T("main_menu.dxf"), "button_play_default", "button_play_selected", "button_play_pressed");

	//b_default->CreateFromFile (_T("main_menu.dxf"), "button_play_default");
	//b_selected->CreateFromFile (_T("main_menu.dxf"), "button_play_selected");
}

void dxMainFrame::Update (float dt) {
	
	if (bPlay) {

		bPlay->Update (dt);
	}

	//obj->RotateZ (dt * .25f);

	if (input.IsRightMouseKeyToggledDown ()) {

		TCHAR tStr [MAX_PATH]; 
		_stprintf_s (tStr, _T("%i, %i"), input.GetCursorPosition ()->x, input.GetCursorPosition ()->y);
		trace (tStr);
	}
}

void dxMainFrame::Render () {
	
	if (background) {

		background->Render ();
	}

	if (bPlay) {

		bPlay->Render ();
	}
}

void dxMainFrame::Destroy () {
	
	DEL (bPlay);
}