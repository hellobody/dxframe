#include "WinMain.h"

#define WIDTH 1024
#define HEIGHT 768

HWND hWnd;

BOOL bActive;
BOOL bAlwaysActive = FALSE;

LPCWSTR APPNAME		= L"dxframe";
LPCWSTR APPTITLE	= L"dxframe";


int NCmdShow = 0;

dxFrame dxframe;
mainFrame mainframe;



LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {

		case WM_ACTIVATE:

			bActive = LOWORD (wParam);
			break;

		case WM_DESTROY:
			dxframe.Destroy ();
			DestroyWindow (hWnd);
			PostQuitMessage (0);
			break;

	} return DefWindowProc (hWnd, message, wParam, lParam);
}

bool WindowInit (HINSTANCE hThisInst, int nCmdShow) {

	WNDCLASS windowClass;

	windowClass.hInstance = hThisInst;
	windowClass.lpszClassName = APPNAME;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.style = 0;
	windowClass.hIcon = LoadIcon (hThisInst, IDC_ICON);
	windowClass.hCursor = LoadCursor (hThisInst, MAKEINTRESOURCE (IDC_POINTER));
	windowClass.lpszMenuName = NULL;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);

	RegisterClass (&windowClass);

	
	int sx = GetSystemMetrics (SM_CXSCREEN);
	int sy = GetSystemMetrics (SM_CYSCREEN);
	
	hWnd = CreateWindowEx (0, APPNAME, APPTITLE, WS_BORDER | WS_POPUP, sx/2-WIDTH/2, sy/2-HEIGHT/2, WIDTH, HEIGHT, NULL, NULL, hThisInst, NULL);
	
	if (!hWnd) return false;
	return true;
}

bool AppInit (HINSTANCE hThisInst, int nCmdShow) {

	if (!WindowInit (hThisInst, nCmdShow)) {
		//trace (_T("Window initialize error."));
		return false;
	}

	dxframe.Create (hThisInst, nCmdShow, hWnd);
	mainframe.Create ();
	
	ShowWindow (hWnd, nCmdShow);

	return true;
}

int APIENTRY WinMain (HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {

	NCmdShow = nCmdShow;

	MSG msg;
	ZeroMemory (&msg, sizeof (msg));

	if (!AppInit (hThisInst, nCmdShow)) return false;

	while (msg.message != WM_QUIT) {

		if (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE)) {

			if (!GetMessage (&msg, NULL, 0, 0)) break;

			TranslateMessage (&msg); 
			DispatchMessage (&msg);

		} else if (bActive || bAlwaysActive) {

			dxframe.Update (hWnd);
			
			if (S_OK == dxframe.RenderBegin ()) {

				mainframe.Render ();

				dxframe.RenderEnd ();
			}
		}

	} return 0;
}