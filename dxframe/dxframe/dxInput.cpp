#include "dxInput.h"
#include "dxLogger.h"

LPDIRECTINPUT8 din = NULL;    // the pointer to our DirectInput interface
LPDIRECTINPUTDEVICE8 dinkeybd = NULL;    // the pointer to the keyboard device
LPDIRECTINPUTDEVICE8 dinmouse = NULL;    // the pointer to the mouse device

DIMOUSESTATE lstMousestate;    // the storage for the last mouse-information
DIMOUSESTATE mousestate;    // the storage for the mouse-information
BYTE lstKeystate [256];    // the storage for the last key-information
BYTE keystate [256];    // the storage for the key-information

extern dxLogger logger;

extern HWND hWnd;

dxInput::dxInput () {

	CursorPosition = new POINT;
}

dxInput::~dxInput () {

	Clean ();
}

void dxInput::Initialize (HINSTANCE hInstance, HWND hWnd) {
	// create the DirectInput interface
	DirectInput8Create (hInstance,    // the handle to the application
		DIRECTINPUT_VERSION,    // the compatible version
		IID_IDirectInput8,    // the DirectInput interface version
		(void**)&din,    // the pointer to the interface
		NULL);    // COM stuff, so we'll set it to NULL

	if (din == NULL) {
		logger.trace (_T("Error creation Direct Input interface."));
		return;
	}

	// create the keyboard device
	din->CreateDevice (GUID_SysKeyboard,    // the default keyboard ID being used
		&dinkeybd,    // the pointer to the device interface
		NULL);    // COM stuff, so we'll set it to NULL

	din->CreateDevice (GUID_SysMouse,
		&dinmouse,
		NULL);

	if (dinkeybd == NULL || dinmouse == NULL) {
		logger.trace (_T("Error creation Keyboard or Mouse devices pointers."));
		return;
	}

	// set the data format to keyboard format
	dinkeybd->SetDataFormat (&c_dfDIKeyboard);
	dinmouse->SetDataFormat (&c_dfDIMouse);

	// set the control you will have over the keyboard
	dinkeybd->SetCooperativeLevel (hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dinmouse->SetCooperativeLevel (hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	ZeroMemory (&lstMousestate, sizeof (lstMousestate));

	for (int i=0; i<256; i++) {
		lstKeystate [i] = 0x00;
	}
}

// this is the function that gets the latest input data
void dxInput::Update () {

	if (dinkeybd == NULL || dinmouse == NULL) {
		return;
	}

	// get access if we don't have it already
	dinkeybd->Acquire ();
	dinmouse->Acquire ();

	//store last key states
	for (int i=0; i<256; i++) {
		lstKeystate [i] = keystate [i];
	}

	//store last mouse states
	lstMousestate = mousestate;

	// get the input data
	dinkeybd->GetDeviceState (256, (LPVOID) keystate);
	dinmouse->GetDeviceState (sizeof (DIMOUSESTATE), (LPVOID) &mousestate);
}

// this is the function that closes DirectInput
void dxInput::Clean () {

	DEL (CursorPosition);

	if (din == NULL || dinkeybd == NULL || dinmouse == NULL) {
		return;
	}

	dinkeybd->Unacquire ();    // make sure the keyboard is unacquired
	dinmouse->Unacquire ();    // make sure the mouse in unacquired
	din->Release ();    // close DirectInput before exiting

	dinkeybd = NULL;
	dinmouse = NULL;
	din = NULL;
}

bool dxInput::IsKeyDown (BYTE KeyCode) {
	if (keystate [KeyCode] & 0x80) {
		return true;
	} return false;
}

bool dxInput::IsKeyUp (BYTE KeyCode) {
	if (keystate [KeyCode] & 0x80) {
		return false;
	} return true;
}

bool dxInput::IsKeyToggledDown (BYTE KeyCode) {
	if (IsKeyDown (KeyCode) && !(lstKeystate [KeyCode] & 0x80)) {
		return true;
	} return false;
}

bool dxInput::IsKeyToggledUp (BYTE KeyCode) {
	if (IsKeyUp (KeyCode) && (lstKeystate [KeyCode] & 0x80)) {
		return true;
	} return false;
}

bool dxInput::IsLeftMouseKeyDown () {
	if (mousestate.rgbButtons [0] & 0x80) {
		return true;
	} return false;
}

bool dxInput::IsRightMouseKeyDown () {
	if (mousestate.rgbButtons [1] & 0x80) {
		return true;
	} return false;
}

bool dxInput::IsLeftMouseKeyUp () {
	if (mousestate.rgbButtons [0] & 0x80) {
		return false;
	} return true;
}

bool dxInput::IsRightMouseKeyUp () {
	if (mousestate.rgbButtons [1] & 0x80) {
		return false;
	} return true;
}

bool dxInput::IsLeftMouseKeyToggledDown () {
	if (IsLeftMouseKeyDown () && !(lstMousestate.rgbButtons [0] & 0x80)) {
		return true;
	} return false;
}

bool dxInput::IsRightMouseKeyToggledDown () {
	if (IsRightMouseKeyDown () && !(lstMousestate.rgbButtons [1] & 0x80)) {
		return true;
	} return false;
}

bool dxInput::IsLeftMouseKeyToggledUp () {
	if (IsLeftMouseKeyUp () && (lstMousestate.rgbButtons [0] & 0x80)) {
		return true;
	} return false;
}

bool dxInput::IsRightMouseKeyToggledUp () {
	if (IsRightMouseKeyUp () && (lstMousestate.rgbButtons [1] & 0x80)) {
		return true;
	} return false;
}

long dxInput::GetMouseDeltaX () {
	return mousestate.lX;
}

long dxInput::GetMouseDeltaY () {
	return mousestate.lY;
}

long dxInput::GetMouseDeltaZ () {
	return mousestate.lZ;
}

LPPOINT dxInput::GetCursorPosition () {

	if (GetCursorPos (CursorPosition) == NULL) {
		
		trace (_T("GetCursorPos some error."));
	}

	ScreenToClient (hWnd, CursorPosition);

	CursorPosition->x = CursorPosition->x - WIDTH/2;
	CursorPosition->y = CursorPosition->y - HEIGHT/2;
	CursorPosition->y *= -1;

	return CursorPosition;
}