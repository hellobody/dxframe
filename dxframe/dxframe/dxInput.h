#ifndef DXINPUT_H
#define DXINPUT_H

#include "defines.h"

class DXFRAME_API dxInput {

protected:

	LPDIRECTINPUT8 din;				// the pointer to our DirectInput interface
	LPDIRECTINPUTDEVICE8 dinkeybd;    // the pointer to the keyboard device
	LPDIRECTINPUTDEVICE8 dinmouse;    // the pointer to the mouse device

	DIMOUSESTATE lstMousestate;    // the storage for the last mouse-information
	DIMOUSESTATE mousestate;    // the storage for the mouse-information
	BYTE lstKeystate [256];    // the storage for the last key-information
	BYTE keystate [256];    // the storage for the key-information

	LPPOINT CursorPosition;

	HWND hWnd;

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

	LPPOINT GetCursorPosition ();
};

#endif
