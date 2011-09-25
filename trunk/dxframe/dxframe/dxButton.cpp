#include "stdafx.h"
#include "dxButton.h"
#include "dxInput.h"

extern dxInput input;

dxButton::dxButton (const TCHAR *FileName, const char *DefaultObjName, const char *SelectedObjName, const char *PressedObjName) {

	lstState = eDefault;
	curState = eDefault;

	objs [eDefault]  = new dxObj (FileName, DefaultObjName);
	objs [eSelected] = new dxObj (FileName, SelectedObjName);
	objs [ePressed]  = new dxObj (FileName, PressedObjName);

	opacity = 1.f;
}

dxButton::~dxButton () {


}

void dxButton::Move (float x, float y, float z) {

	forup (eNumOfStates) {

		if (objs [i]) {

			objs [i]->Move (x, y, z);
		}
	}
}

void dxButton::Scale (float x, float y, float z) {

	forup (eNumOfStates) {

		if (objs [i]) {

			objs [i]->Scale (x, y, z);
		}
	}
}

bool dxButton::IsDown () {

	if (curState == ePressed) {
		return true;
	} else return false;
}

bool dxButton::IsUp () {

	if (curState == eDefault || curState == eSelected) {
		return true;
	} else return false;
}

bool dxButton::IsToggledDown () {
	
	if (curState == ePressed && (lstState == eDefault || lstState == eSelected)) {
		return true;
	} else return false;
}

bool dxButton::IsToggledUp () {
	
	if ((curState == eDefault || curState == eSelected) && lstState == ePressed) {
		return true;
	} else return false;
}

void dxButton::SetOpacity (float v) {

	opacity = v;

	forup (eNumOfStates) {

		if (objs [i]) {

			objs [i]->SetOpacity (opacity);
		}
	}
}

float dxButton::GetOpacity () {

	return opacity;
}

void dxButton::Update (float dt) {

	lstState = curState;

	if (objs [eDefault]) {
		
		if (objs [eDefault]->IsPick (input.GetCursorPosition ()->x, input.GetCursorPosition ()->y)) {
			
			if (input.IsLeftMouseKeyToggledDown ()) {

				curState = ePressed;
			} else if (input.IsLeftMouseKeyToggledUp ()) {

				curState = eDefault;
			} else if (input.IsLeftMouseKeyDown ()) {

				if (curState == eDefault) {

					curState = eSelected;
				}
			} else {

				curState = eSelected;
			}

		} else if (curState == ePressed) {

			lstState = curState = eDefault;
		} else curState = eDefault;
	}
}

void dxButton::Render () {

	switch (curState) {

		case eDefault:
			
			if (objs [eDefault]) objs [eDefault]->Render ();
			break;

		case ePressed:

			if (objs [ePressed]) objs [ePressed]->Render ();
			break;

		case eSelected:

			if (objs [eSelected]) objs [eSelected]->Render ();
			break;
	}

	/*if (objs [ePressed]) objs [ePressed]->Render ();
	if (objs [eDefault]) objs [eDefault]->Render ();
	if (objs [eSelected]) objs [eSelected]->Render ();*/
}