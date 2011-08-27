#include "dxIniFileInterface.h"
#include "dxConsole.h"
#include "dxInput.h"

extern dxIniFileInterface iniFileInterface;
extern dxInput input;

extern void EnableCameraMove ();
extern void DisableCameraMove ();
extern void SwitchToOrthographicView ();
extern void SwitchToPerspectiveView ();
extern void ResetCameraPosition ();

static DWORD numOfCharsToReadOrWrited;

dxConsole::dxConsole () {

	hIConsole = NULL;
	hOConsole = NULL;

	if (AllocConsole ()) {
			
		hIConsole = GetStdHandle (STD_INPUT_HANDLE);
		hOConsole = GetStdHandle (STD_OUTPUT_HANDLE);

		SetConsoleTitle (_T("Command Console"));

		SMALL_RECT conRect;

		conRect.Left = 10;
		conRect.Right = 100;
		conRect.Top = 10;
		conRect.Bottom = 50;

		SetConsoleWindowInfo (hIConsole, TRUE, &conRect);
	}
}

dxConsole::~dxConsole () {

	FreeConsole ();
}

bool dxConsole::Update () {
		
	TCHAR tStr0 [MAX_PATH] = _T("");
	TCHAR tStr1 [MAX_PATH] = _T("");

	ReadConsole (hIConsole, tStr0, MAX_PATH * sizeof (TCHAR), &numOfCharsToReadOrWrited, NULL);

	forup ((int) numOfCharsToReadOrWrited - 2) {
		tStr1 [i] = tStr0 [i];
	}

	return ProcCommand (tStr1);
}

bool dxConsole::ProcCommand (TCHAR *Command) {
	
	if (_tcscmp (Command, _T("camon")) == 0) {

		EnableCameraMove ();

		buf = _T("Free look camera mode is on.\n");
		WriteConsole (hOConsole, buf, (int)_tcslen (buf), &numOfCharsToReadOrWrited, NULL);

	} else if (_tcscmp (Command, _T("camoff")) == 0) {

		DisableCameraMove ();

		buf = _T("Free look camera mode is off.\n");
		WriteConsole (hOConsole, buf, (int)_tcslen (buf), &numOfCharsToReadOrWrited, NULL);

	} else if (_tcscmp (Command, _T("rescam")) == 0) {

		ResetCameraPosition ();

	} else if (_tcscmp (Command, _T("2d")) == 0) {

		iniFileInterface.Set (_T("View Mode"), _T("2d"));
		SwitchToOrthographicView ();

	} else if (_tcscmp (Command, _T("3d")) == 0) {

		iniFileInterface.Set (_T("View Mode"), _T("3d"));
		SwitchToPerspectiveView ();

	} else if (_tcscmp (Command, _T("close")) == 0 || 
				_tcscmp (Command, _T("quit")) == 0 ||
				_tcscmp (Command, _T("exit")) == 0) {

		return false;
	}

	return true;
}