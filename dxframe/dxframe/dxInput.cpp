#include "stdafx.h"
#include "dxInput.h"
#include "dxLogger.h"

#define FIND_HEAP_CORRUPTION {char *tmp = new char [1]; delete tmp;};

extern dxLogger logger;

extern D3DDISPLAYMODE d3ddm;

extern bool fullScreen;

static HRESULT hr;

dxInput::dxInput () {

	din = NULL;	
	dinkeybd = NULL;    
	dinmouse = NULL;

	CursorPosition = new POINT;
}

dxInput::~dxInput () {
	
	Clean ();
}

void dxInput::Initialize (HINSTANCE hInstance, HWND hWnd) {
	
	this->hWnd = hWnd;
	
	// create the DirectInput interface
	hr = DirectInput8Create (hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **) &din, NULL);
	
	hr = din->CreateDevice (GUID_SysKeyboard, &dinkeybd, NULL);
	hr = dinkeybd->SetDataFormat (&c_dfDIKeyboard);
	hr = dinkeybd->SetCooperativeLevel (hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	//hr = dinkeybd->Acquire ();

	hr = din->CreateDevice (GUID_SysMouse, &dinmouse, NULL);
	hr = dinmouse->SetDataFormat (&c_dfDIMouse);
	hr = dinmouse->SetCooperativeLevel (hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	//hr = dinmouse->Acquire ();

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

	//store last key states
	for (int i=0; i<256; i++) {
		lstKeystate [i] = keystate [i];
	}

	//store last mouse states
	lstMousestate = mousestate;

	////get the input data

	trace (_T("0"));

	hr = dinkeybd->GetDeviceState (256, (LPVOID) keystate);

	trace (_T("1"));

	if (FAILED (hr)) {

		int a;

		switch (hr) {
			case DIERR_INPUTLOST:
				a = 0;
				break;
			case DIERR_INVALIDPARAM: 
				a = 0;
				break;
			case DIERR_NOTACQUIRED: 
				// If input is lost then acquire and keep trying until we get it back 
				trace (_T("2"));

				/*do {
					hr = dinkeybd->Unacquire ();
					trace (_T("2.1"));
					hr = dinkeybd->Acquire ();
				} while (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED);*/

				trace (_T("3"));
				break;
			case DIERR_NOTINITIALIZED: 
				a = 0;
				break;
			case E_PENDING: 
				a = 0;
				break;
		}

		

		// Could be we failed for some other reason
		if (FAILED (hr)) {
			return;
		}
		// Now read the state again
		dinkeybd->GetDeviceState (256, (LPVOID) keystate);

		trace (_T("4"));
	}

	hr = dinmouse->GetDeviceState (sizeof (DIMOUSESTATE), (LPVOID) &mousestate);

	trace (_T("5"));

	if (FAILED (hr)) {
		// If input is lost then acquire and keep trying until we get it back 
		hr = dinmouse->Acquire ();

		trace (_T("6"));

		/*while (hr == DIERR_INPUTLOST) {          
			hr = dinmouse->Acquire ();
		}*/

		trace (_T("7"));

		// Could be we failed for some other reason
		if (FAILED (hr)) {
			return;
		}
		// Now read the state again
		dinmouse->GetDeviceState (sizeof (DIMOUSESTATE), (LPVOID) &mousestate);

		trace (_T("8"));
	}

	trace (_T("exit"));

	//dinkeybd->GetDeviceState (256, (LPVOID) keystate);
	//dinmouse->GetDeviceState (sizeof (DIMOUSESTATE), (LPVOID) &mousestate);
	////
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

	if (!fullScreen) {
		ScreenToClient (hWnd, CursorPosition);
	}

	CursorPosition->x = CursorPosition->x - d3ddm.Width / 2;
	CursorPosition->y = CursorPosition->y - d3ddm.Height / 2;
	CursorPosition->y *= -1;

	return CursorPosition;
}