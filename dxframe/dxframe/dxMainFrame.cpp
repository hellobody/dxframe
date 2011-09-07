#include "dxMainFrame.h"

extern dxInput input;

extern void Exit ();

dxMainFrame::dxMainFrame () {
	
	background = NULL;
	b_default = NULL;
	b_selected = NULL;

	bPlay = NULL;
	bOptions = NULL;
	bScores = NULL;
	bExit = NULL;
}

dxMainFrame::~dxMainFrame () {
	
}

void dxMainFrame::Create () {
	
	//todo: create dxObjes here

	background = new dxObj;
	

	background->CreateFromFile (_T("main_menu.dxf"), "background");

	bPlay = new dxButton (_T("main_menu.dxf"), "button_play_default", "button_play_selected", "button_play_pressed");

	bOptions = new dxButton (_T("main_menu.dxf"), "button_play_default", "button_play_selected", "button_play_pressed");
	bScores = new dxButton (_T("main_menu.dxf"), "button_play_default", "button_play_selected", "button_play_pressed");
	bExit = new dxButton (_T("main_menu.dxf"), "button_play_default", "button_play_selected", "button_play_pressed");

	bOptions->Move (0, -100, 0);
	bScores->Move (0, -200, 0);

	bExit->Move (0, -400, 0);
	bExit->Scale (.5f, .5f, 1);
}

void dxMainFrame::Update (float dt) {
	
	if (bPlay) {

		bPlay->Update (dt);
	}
	if (bOptions) {

		bOptions->Update (dt);
	}
	if (bScores) {

		bScores->Update (dt);
	}

	if (bExit) {

		bExit->Update (dt);

		if (bExit->IsToggledUp ()) {

			trace (_T("IsToggledUp"));
			Exit ();
			return;
		}
		if (bExit->IsToggledDown ()) {

			trace (_T("IsToggledDown"));
		}
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
	if (bOptions) {

		bOptions->Render ();
	}
	if (bScores) {

		bScores->Render ();
	}
	if (bExit) {

		bExit->Render ();
	}

}

void dxMainFrame::Destroy () {
	
	DEL (bPlay);
	DEL (bOptions);
	DEL (bScores);
	DEL (bExit);
}