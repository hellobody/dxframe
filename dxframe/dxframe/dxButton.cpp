#include "dxButton.h"
#include "dxInput.h"

extern dxInput input;

dxButton::dxButton (const TCHAR *FileName, const char *DefaultObjName, const char *SelectedObjName, const char *PressedObjName) {

	state = eDefault;

	objs [eDefault]  = new dxObj (FileName, DefaultObjName);
	objs [eSelected] = new dxObj (FileName, SelectedObjName);
	objs [ePressed]  = new dxObj (FileName, PressedObjName);
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

void dxButton::Update (float dt) {

	if (objs [eDefault]) {
		
		if (objs [eDefault]->IsPick (input.GetCursorPosition ()->x, input.GetCursorPosition ()->y)) {
			
			if (input.IsLeftMouseKeyDown ()) {
				
				state = ePressed;
			} else {
				
				state = eSelected;
			}
		} else {

			state = eDefault;
		}
	}
}

void dxButton::Render () {

	switch (state) {

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