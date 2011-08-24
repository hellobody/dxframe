#include "dxConsole.h"
#include "dxInput.h"

extern dxInput input;

extern void EnableCameraMove ();
extern void DisableCameraMove ();
extern void SwitchToOrthographicView ();
extern void SwitchToPerspectiveView ();
extern void ResetCameraPosition ();

dxConsole::dxConsole () {

}

dxConsole::~dxConsole () {
	
}

void dxConsole::Update () {
	
	if (input.IsKeyToggledDown (DIK_GRAVE)) {

		if (AllocConsole ()) {
			
			HANDLE hIConsole = GetStdHandle (STD_INPUT_HANDLE);

			SMALL_RECT conRect;

			conRect.Left = 10;
			conRect.Right = 100;
			conRect.Top = 10;
			conRect.Bottom = 50;

			SetConsoleWindowInfo (hIConsole, FALSE, &conRect);

			DWORD numOfCharsToRead;
			
			TCHAR tStr0 [MAX_PATH] = _T("");
			TCHAR tStr1 [MAX_PATH] = _T("");

			ReadConsole (hIConsole, tStr0, MAX_PATH * sizeof (TCHAR), &numOfCharsToRead, NULL);

			forup ((int) numOfCharsToRead - 2) {
				tStr1 [i] = tStr0 [i];
			}

			ProcCommand (tStr1);
		}

		FreeConsole ();
	}
}

void dxConsole::ProcCommand (TCHAR *Command) {
	
	if (_tcscmp (Command, _T("camon")) == 0) {
		EnableCameraMove ();
	} else if (_tcscmp (Command, _T("camoff")) == 0) {
		DisableCameraMove ();
	} else if (_tcscmp (Command, _T("rescam")) == 0) {
		ResetCameraPosition ();
	} else if (_tcscmp (Command, _T("2d")) == 0) {
		SwitchToOrthographicView ();
	} else if (_tcscmp (Command, _T("3d")) == 0) {
		SwitchToPerspectiveView ();
	}
}

void dxConsole::Close () {

	FreeConsole ();
}