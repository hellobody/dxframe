#include "WinMain.h"
#include "TCHAR.h"
#include <fstream>

using namespace std;

ifstream fin;

HWND hWnd;
BOOL bActive;
LPCWSTR APPNAME = L"dxframe";
LPCWSTR APPTITLE = L"dxframe";

//interfaces
LPDIRECT3D8 p_d3d = NULL;
LPDIRECT3DDEVICE8 p_d3d_Device = NULL;
LPDIRECT3DVERTEXBUFFER8 p_VertexBuffer = NULL;

dxObj obj;

D3DXMATRIX matWorld;
D3DXMATRIX matView;
D3DXMATRIX matProj;

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

	hWnd = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW, APPNAME, APPTITLE, WS_OVERLAPPEDWINDOW, 0, 0, WIDTH, HEIGHT, NULL, NULL, hThisInst, NULL);

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

	D3DXMatrixRotationY (&matWorld, 0.0f);
	D3DXMatrixLookAtLH (&matView, &D3DXVECTOR3 (0.0f, 0.0f, -500.0f),
		&D3DXVECTOR3 (0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3 (0.0f, 1.0f, 0.0f));
	D3DXMatrixPerspectiveFovLH (&matProj, D3DX_PI/4, 1.0f, 1.0f, 10000.0f);

	fin.open (_T("data\\test.DXF"), ios::in | ios::binary);

	if (!fin.fail ()) {
		fin.read ((char *) &obj, sizeof (dxObj));
		obj.pVerts = new float [obj.numVerts * 3];
	}

	forup (obj.numVerts * 3) {
		fin.read ((char *) &obj.pVerts[i], 4);
	}

	fin.close ();

	CUSTOMVERTEX *g_Vertices;

	g_Vertices = new CUSTOMVERTEX [obj.numVerts];

	forup (obj.numVerts) {
		g_Vertices[i] = CUSTOMVERTEX (obj.pVerts[i*3] + WIDTH/2, obj.pVerts[i*3+2] + HEIGHT/2, obj.pVerts[i*3+1]);
	}
	
	/*g_Vertices[0] = CUSTOMVERTEX (60.0f, 60.0f, 0.5f, 1.0f, 0xffff0000);
	g_Vertices[1] = CUSTOMVERTEX (200.0f, 60.0f, 0.5f, 1.0f, 0xff00ff00);
	g_Vertices[2] = CUSTOMVERTEX (60.0f, 200.0f, 0.5f, 1.0f, 0xff0000ff);

	g_Vertices[3] = CUSTOMVERTEX (80.0f, 220.0f, 0.5f, 1.0f, 0xff0000ff);
	g_Vertices[4] = CUSTOMVERTEX (220.0f, 80.0f, 0.5f, 1.0f, 0xff00ff00);
	g_Vertices[5] = CUSTOMVERTEX (220.0f, 220.0f, 0.5f, 1.0f, 0xffffff00);*/
	
	p_d3d_Device->CreateVertexBuffer (obj.numVerts * sizeof (CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &p_VertexBuffer);

	VOID * pVertices;
	p_VertexBuffer->Lock (0, obj.numVerts * sizeof (CUSTOMVERTEX), (BYTE**)&pVertices, 0);
	memcpy (pVertices, g_Vertices, obj.numVerts * sizeof (CUSTOMVERTEX));
	p_VertexBuffer->Unlock ();

	DELA (g_Vertices);

	return true;
}

void Render () {

	p_d3d_Device->SetTransform (D3DTS_WORLD, &matWorld);
	p_d3d_Device->SetTransform (D3DTS_VIEW, &matView);
	p_d3d_Device->SetTransform (D3DTS_PROJECTION, &matProj);

	p_d3d_Device->SetRenderState (D3DRS_CULLMODE, D3DCULL_NONE);



	
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