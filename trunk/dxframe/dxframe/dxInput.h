#ifndef DXINPUT_H
#define DXINPUT_H

#include "defines.h"

class dxInput {

public:

	dxInput ();
	~dxInput ();

	void Initialize (HINSTANCE hInstance, HWND hWnd);
	void Update ();
	void Clean ();

	bool IsKeyDown (BYTE KeyCode);
	bool IsKeyUp (BYTE KeyCode);
	bool IsKeyToggledDown (BYTE KeyCode);
	bool IsKeyToggledUp (BYTE KeyCode);

	bool IsLeftMouseKeyDown ();
	bool IsRightMouseKeyDown ();
	bool IsLeftMouseKeyUp ();
	bool IsRightMouseKeyUp ();
	bool IsLeftMouseKeyToggledDown ();
	bool IsRightMouseKeyToggledDown ();
	bool IsLeftMouseKeyToggledUp ();
	bool IsRightMouseKeyToggledUp ();

	long GetMouseDeltaX ();
	long GetMouseDeltaY ();
	long GetMouseDeltaZ ();
};

#endif
