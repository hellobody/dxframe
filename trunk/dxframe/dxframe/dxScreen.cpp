#include "stdafx.h"
#include "dxScreen.h"

dxObj *dxScreen::darkScreen = NULL;

std::vector <dxScreen *> dxScreen::vScreens;

int dxScreen::currentScreen = UNDEFINED;
int dxScreen::nextScreen = UNDEFINED;

dxScreen::dxScreen () {

	vScreens.push_back (this);

	currentScreen = 0;
	nextScreen = UNDEFINED;
}

dxScreen::~dxScreen () {	//при первом вызове деструктора, уничтожаться все объекты dxScreen

	while (vScreens.size ()) {

		DEL (vScreens [0]);
		vScreens.erase (vScreens.begin ());
	}
}

bool dxScreen::isValidId (int id) {

	if (id >= 0 && id < (int) vScreens.size ()) {

		return true;
	} return false;
}

bool dxScreen::SwitchTo (int id) {

	if (isValidId (id) && nextScreen == UNDEFINED && id != currentScreen) {

		nextScreen = id;
		return true;
	} return false;
}

void dxScreen::UpdateScreens (float dt) {

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

	if (isValidId (currentScreen)) {

		if (vScreens [currentScreen]) {

			vScreens [currentScreen]->Render ();
		}
	}

	if (isValidId (nextScreen)) {

		if (vScreens [nextScreen]) {

			vScreens [nextScreen]->Render ();
		}
	}
}