#include "mainFrame.h"

extern void Exit ();

mainFrame::mainFrame () {

	skySphere = NULL;
	background = NULL;
	box = NULL;
	sphere = NULL;

	bPlay = NULL;
	bOptions = NULL;
	bScores = NULL;
	bExit = NULL;
}

mainFrame::~mainFrame () {
	
}

void mainFrame::Create () {
	
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

	box = new dxObj (_T("test.dxf"), "Box01");
	sphere = new dxObj (_T("test.dxf"), "GeoSphere01");

	box->Scale (0.33f, 0.33f, 0.33f);
	box->Move (-200, 200, 0);

	sphere->Scale (0.33f, 0.33f, 0.33f);
	sphere->Move (200, 200, 0);

}

void mainFrame::Update (float dt) {
	
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

	//if (input.IsRightMouseKeyToggledDown ()) {

	//	TCHAR tStr [MAX_PATH]; 
	//	_stprintf_s (tStr, _T("%i, %i"), input.GetCursorPosition ()->x, input.GetCursorPosition ()->y);
	//	trace (tStr);
	//}
}

void mainFrame::Render () {

	if (skySphere) {

		skySphere->Render ();
	}
	
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

	if (box) box->Render ();

	if (sphere) sphere->Render ();

}

void mainFrame::Destroy () {
	
	DEL (bPlay);
	DEL (bOptions);
	DEL (bScores);
	DEL (bExit);

}