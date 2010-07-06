#include "WinMain.h"

HWND hWnd;
BOOL bActive;
LPCWSTR APPNAME = L"dxframe";
LPCWSTR APPTITLE = L"dxframe";

LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_ACTIVATE:
			bActive = LOWORD (wParam);
			break;
		case WM_DESTROY:
			PostQuitMessage (0);
			break;
		case WM_SETCURSOR:
			break;
	} return DefWindowProc (hWnd, message, wParam, lParam);
}

bool WindowInit (HINSTANCE hThisInst, int nCmdShow) {
	WNDCLASS wcl;

	wcl.hInstance = hThisInst;
	wcl.lpszClassName = APPNAME;
	wcl.lpfnWndProc = WindowProc;
	wcl.style = 0;
	wcl.hIcon = LoadIcon (hThisInst, IDC_ICON);
	wcl.hCursor = LoadCursor (hThisInst, IDC_ARROW);
	wcl.lpszMenuName = NULL;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);

	RegisterClass (&wcl);

	hWnd = CreateWindowEx (WS_EX_TOPMOST, APPNAME, APPTITLE, WS_OVERLAPPEDWINDOW, 0, 0, 640, 480, NULL, NULL, hThisInst, NULL);

	if (!hWnd) return false;
	return true;
}

bool AppInit (HINSTANCE hThisInst, int nCmdShow) {
	srand (unsigned int (time (0)));

	if (!WindowInit (hThisInst, nCmdShow)) return false;
	ShowWindow (hWnd, nCmdShow);
	UpdateWindow (hWnd);

	return true;
}

void Render () {

}

int APIENTRY WinMain (HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;

	if (!AppInit (hThisInst, nCmdShow)) return false;

	while (true) {
		if (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!GetMessage (&msg, NULL, 0, 0)) break;
			TranslateMessage (&msg);
			DispatchMessage (&msg);
		} else if (bActive) Render ();
	} return 0;
}