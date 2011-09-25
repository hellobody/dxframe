#ifndef DXSCREEN_H
#define DXSCREEN_H

#include "dxObj.h"
#include "dxButton.h"

class DXFRAME_API dxScreen {

protected:

	static dxObj *darkScreen;

	static std::vector <dxScreen *> vScreens;

	static int currentScreen;
	static int nextScreen;

public:
	
	dxScreen ();
	~dxScreen ();

	static bool isValidId (int id);
	static bool SwitchTo (int id);

	static void UpdateScreens (float dt);
	static void RenderScreens ();

	virtual void Update (float dt) = 0;
	virtual void Render () = 0;
};

#endif
