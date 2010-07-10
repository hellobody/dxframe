#include "WinMain.h"

HWND hWnd;
BOOL bActive;
LPCWSTR APPNAME = L"dxframe";
LPCWSTR APPTITLE = L"dxframe";

//interfaces
LPDIRECT3D8 p_d3d = NULL;
LPDIRECT3DDEVICE8 p_d3d_Device = NULL;
LPDIRECT3DVERTEXBUFFER8 p_VertexBuffer = NULL;

CUSTOMVERTEX g_Vertices[] = {
	{  60.0f,  60.0f, 0.5f, 1.0f, 0xffff0000, },
	{ 200.0f,  60.0f, 0.5f, 1.0f, 0xff00ff00, },
	{  60.0f, 200.0f, 0.5f, 1.0f, 0xff0000ff, },

	{  80.0f, 220.0f, 0.5f, 1.0f, 0xff0000ff, },
	{ 220.0f,  80.0f, 0.5f, 1.0f, 0xff00ff00, },
	{ 220.0f, 220.0f, 0.5f, 1.0f, 0xffffff00, },
};

ifstream pFile;

LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_ACTIVATE:
			bActive = LOWORD (wParam);
			break;
		case WM_DESTROY:
			RELEASE (p_VertexBuffer);
			RELEASE (p_d3d_Device);
			RELEASE (p_d3d);
			PostQuitMessage (0);
			break;
		case WM_SETCURSOR:
			SetCursor (NULL);
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

	p_d3d = Direct3DCreate8 (D3D_SDK_VERSION);

	D3DDISPLAYMODE d3ddm;
	p_d3d->GetAdapterDisplayMode (D3DADAPTER_DEFAULT, &d3ddm);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory (&d3dpp, sizeof (d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = d3ddm.Format;

	p_d3d->CreateDevice (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &p_d3d_Device);


	pFile.open ("box.dxf", ios_base::in | ios_base::binary);

	dxObj obj;

	p_d3d_Device->CreateVertexBuffer (6*sizeof (CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &p_VertexBuffer);

	VOID * pVertices;
	p_VertexBuffer->Lock (0, sizeof (g_Vertices), (BYTE**)&pVertices, 0);
	memcpy (pVertices, g_Vertices, sizeof (g_Vertices));
	p_VertexBuffer->Unlock ();

	return true;
}

void Render () {
	
	p_d3d_Device->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (0, 0, 0), 1.f, 0);
	p_d3d_Device->BeginScene ();

	p_d3d_Device->SetVertexShader (D3DFVF_CUSTOMVERTEX);
	p_d3d_Device->SetStreamSource (0, p_VertexBuffer, sizeof (CUSTOMVERTEX));
	p_d3d_Device->DrawPrimitive (D3DPT_TRIANGLELIST, 0, 2);

	p_d3d_Device->EndScene ();
	p_d3d_Device->Present (NULL, NULL, NULL, NULL);
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