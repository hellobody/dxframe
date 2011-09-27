#include "stdafx.h"
#include "dxScreen.h"

dxObj *dxScreen::darkScreen = NULL;

std::vector <dxScreen *> dxScreen::vScreens;

int dxScreen::currentScreen = UNDEFINED;
int dxScreen::nextScreen = UNDEFINED;

float dxScreen::switchTimer = 0;
float dxScreen::facSwitchTimer = 1.f;

dxScreen::dxScreen () {

	vScreens.push_back (this);

	currentScreen = 0;
	nextScreen = UNDEFINED;

	opacity = 1.f;
}

dxScreen::~dxScreen () {	//при первом вызове деструктора, уничтожаться все объекты dxScreen

	while (vScreens.size ()) {

		DEL (vScreens [0]);
		vScreens.erase (vScreens.begin ());
	}
}

void dxScreen::SetFacSwitchTimer (float fac) {

	if (fac > 0) {

		facSwitchTimer = fac;
	}
}

bool dxScreen::isValidId (int id) {

	if (id >= 0 && id < (int) vScreens.size ()) {

		return true;
	} return false;
}

bool dxScreen::SwitchTo (int id) {

	if (isValidId (id) && nextScreen == UNDEFINED && id != currentScreen) {

		switchTimer = .001f;
		nextScreen = id;
		return true;
	} return false;
}

void dxScreen::CreateScreens () {

	forup ((int) vScreens.size ()) {

		if (vScreens [i]) {

			vScreens [i]->Create ();
		}
	}
}

void dxScreen::UpdateScreens (float dt) {

	if (switchTimer > 0)
	{
		switchTimer += dt * facSwitchTimer;

		if (switchTimer >= 1) {

			switchTimer = 0;

			currentScreen = nextScreen;
			nextScreen = UNDEFINED;
		}
	}

	if (isValidId (currentScreen)) {

		if (vScreens [currentScreen]) {

			vScreens [currentScreen]->Update (dt);
		}
	}

	if (isValidId (nextScreen)) {

		if (vScreens [nextScreen]) {

			vScreens [nextScreen]->Update (dt);
		}
	}
}

void dxScreen::RenderScreens () {

	if (switchTimer < .5f) {

		if (isValidId (currentScreen)) {

			if (vScreens [currentScreen]) {

				vScreens [currentScreen]->SetOpacity (1.f - switchTimer * 2);
				vScreens [currentScreen]->Render ();
			}
		}
	} else {

		if (isValidId (nextScreen)) {

			if (vScreens [nextScreen]) {

				vScreens [nextScreen]->SetOpacity (switchTimer * 2 - 1.f);
				vScreens [nextScreen]->Render ();
			}
		}
	}
}

void dxScreen::SetOpacity (float v) {

	opacity = v;
}

float dxScreen::GetOpacity () {

	return opacity;
}