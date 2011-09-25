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

	static float switchTimer;
	static float facSwitchTimer;

	float opacity;

public:
	
	dxScreen ();
	~dxScreen ();

	static bool isValidId (int id);
	static bool SwitchTo (int id);

	static void CreateScreens ();
	static void UpdateScreens (float dt);
	static void RenderScreens ();

	void SetFacSwitchTimer (float fac);
	void SetOpacity (float v);
	float GetOpacity ();

	virtual void Create () = 0;
	virtual void Update (float dt) = 0;
	virtual void Render () = 0;
};

#endif
