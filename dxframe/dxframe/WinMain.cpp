#include "WinMain.h"
#include "TCHAR.h"

HWND hWnd;
BOOL bActive;
LPCWSTR APPNAME = L"dxframe";
LPCWSTR APPTITLE = L"dxframe";

LPDIRECT3D8 p_d3d = NULL;				//direct 3d main interface
LPDIRECT3DDEVICE8 p_d3d_Device = NULL;	//direct 3d device
LPDIRECT3DVERTEXBUFFER8 p_VertexBuffer = NULL; //vertex buffer
LPDIRECT3DINDEXBUFFER8 p_IndexBuffer  = NULL; //indexes buffer

D3DDISPLAYMODE d3ddm;					//display mode parameters
D3DPRESENT_PARAMETERS d3dpp;			//present parameters

D3DXMATRIX matWorld;
D3DXMATRIX matView;
D3DXMATRIX matProj;

//materials
D3DMATERIAL8 mtrl1;
D3DMATERIAL8 mtrl2;

//light
D3DLIGHT8 light;

ifstream fin;							//file input
dxObj obj;								//my object

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

	hWnd = CreateWindowEx (WS_EX_TOPMOST, APPNAME, APPTITLE, WS_OVERLAPPEDWINDOW, 0, 0, WIDTH, HEIGHT, NULL, NULL, hThisInst, NULL);

	if (!hWnd) return false;
	return true;
}

bool AppInit (HINSTANCE hThisInst, int nCmdShow) {

	if (!WindowInit (hThisInst, nCmdShow)) return false; 

	ShowWindow (hWnd, nCmdShow);
	UpdateWindow (hWnd);

	p_d3d = Direct3DCreate8 (D3D_SDK_VERSION); //creating main interface
	p_d3d->GetAdapterDisplayMode (D3DADAPTER_DEFAULT, &d3ddm); //get info about current display mode (resolution and parameters) 

	ZeroMemory (&d3dpp, sizeof (d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//set method of window update
	d3dpp.BackBufferFormat = d3ddm.Format;		//set format of surface of second buffer

	//init materials
	ZeroMemory (&mtrl1, sizeof(D3DMATERIAL8));
	mtrl1.Diffuse.r = mtrl1.Ambient.r = 0.0f;
	mtrl1.Diffuse.g = mtrl1.Ambient.g = 0.0f;
	mtrl1.Diffuse.b = mtrl1.Ambient.b = 1.0f;
	mtrl1.Diffuse.a = mtrl1.Ambient.a = 1.0f;

	ZeroMemory (&mtrl2, sizeof(D3DMATERIAL8));
	mtrl2.Diffuse.r = mtrl1.Ambient.r = 1.0f;
	mtrl2.Diffuse.g = mtrl1.Ambient.g = 1.0f;
	mtrl2.Diffuse.b = mtrl1.Ambient.b = 0.0f;
	mtrl2.Diffuse.a = mtrl1.Ambient.a = 1.0f;
	//

	

	//create and init d3d device
	//it is will belonged to first video adapter D3DADAPTER_DEFAULT
	//working with using hardware acceleration D3DDEVTYPE_HAL
	//the window context will be hWnd
	//will be use software vertex processing D3DCREATE_SOFTWARE_VERTEXPROCESSING
	//will be use above setted present parameters d3dpp
	//p_d3d_Device - will be the pointer to this device
	p_d3d->CreateDevice (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &p_d3d_Device);

	//open file with .dxf models
	fin.open (_T("data\\test.DXF"), ios::in | ios::binary);

	if (!fin.fail ()) {
		fin.read ((char *) &obj, sizeof (dxObj));
		
		obj.pVertsWithNormals = new float [obj.numVerts * 3 * 2]; //because verts with normals
		
		forup (obj.numVerts * 3 * 2) {
			fin.read ((char *) &obj.pVertsWithNormals[i], 4);
		}

		obj.pFaces = new int [obj.numFaces * 3]; 

		forup (obj.numFaces * 3) {
			fin.read ((char *) &obj.pFaces[i], 4);
		}

		fin.close ();
	}

	//Read vertexes
	CUSTOMVERTEX *g_Vertices;
	//obj.numVerts = 3; //temp
	g_Vertices = new CUSTOMVERTEX [obj.numVerts];

	//temp
	/*g_Vertices[0] = CUSTOMVERTEX (0,	0,	0,	0,	1,	0);
	g_Vertices[1] = CUSTOMVERTEX (100,	0,	0,	0,	1,	0);
	g_Vertices[2] = CUSTOMVERTEX (0,	0,	100,0,	1,	0);*/
	//

	forup (obj.numVerts) {
		g_Vertices[i] = CUSTOMVERTEX (obj.pVertsWithNormals[i*6], obj.pVertsWithNormals[i*6+2], obj.pVertsWithNormals[i*6+1], obj.pVertsWithNormals[i*6+3], obj.pVertsWithNormals[i*6+4], obj.pVertsWithNormals[i*6+5]);
	}
	
	//Create vertex buffer
	p_d3d_Device->CreateVertexBuffer (obj.numVerts * sizeof (CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &p_VertexBuffer);

	VOID * pVertices;
	p_VertexBuffer->Lock (0, obj.numVerts * sizeof (CUSTOMVERTEX), (BYTE**)&pVertices, 0);
	memcpy (pVertices, g_Vertices, obj.numVerts * sizeof (CUSTOMVERTEX));
	p_VertexBuffer->Unlock ();

	DELA (g_Vertices);

	//Create index buffer

	//obj.numFaces = 1; //temp

	//temp
	/*obj.pFaces[0] = 0;
	obj.pFaces[1] = 1; 
	obj.pFaces[2] = 2;*/
	//

	p_d3d_Device->CreateIndexBuffer (obj.numFaces * 12, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &p_IndexBuffer);

	VOID* pVerticesI;
	p_IndexBuffer->Lock (0, obj.numFaces * 12, (BYTE**)&pVerticesI, 0);
	memcpy (pVerticesI, obj.pFaces, obj.numFaces * 4);
	p_IndexBuffer->Unlock();

	D3DXMatrixRotationY (&matWorld, 0.0f);
	D3DXMatrixLookAtLH (&matView, &D3DXVECTOR3 (0.0f, 0.0f, -200.0f), &D3DXVECTOR3 (0.f, 0.f, 0.f), &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));
	D3DXMatrixPerspectiveFovLH (&matProj, D3DX_PI/2, 4.f/3.f, 1.f, 10000.f); //last two edges of drawing, do not set near val < 1.f
	//second param - angle of view, third - aspect ratio

	p_d3d_Device->SetTransform (D3DTS_WORLD, &matWorld);
	p_d3d_Device->SetTransform (D3DTS_VIEW, &matView);
	p_d3d_Device->SetTransform (D3DTS_PROJECTION, &matProj);
	p_d3d_Device->SetRenderState (D3DRS_CULLMODE, D3DCULL_NONE);

	//light init
	D3DXVECTOR3 vecDir;
	ZeroMemory (&light, sizeof(D3DLIGHT8));
	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;

	vecDir = D3DXVECTOR3 (0.0f, 0.0f, 1.0f);
	D3DXVec3Normalize ((D3DXVECTOR3*)&light.Direction, &vecDir);

	light.Range = 20000.0f;

	p_d3d_Device->SetLight (0, &light);
	p_d3d_Device->LightEnable (0, true);

	p_d3d_Device->SetRenderState (D3DRS_LIGHTING, true);
	p_d3d_Device->SetRenderState (D3DRS_AMBIENT, 0);
	//

	return true;
}
void Render ()
{
	p_d3d_Device->Clear (0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB (255, 255, 255), 1.0f, 0);
	p_d3d_Device->BeginScene ();

	p_d3d_Device->SetVertexShader (D3DFVF_CUSTOMVERTEX);
	p_d3d_Device->SetStreamSource (0, p_VertexBuffer, sizeof(CUSTOMVERTEX));
	p_d3d_Device->SetIndices (p_IndexBuffer, 0);

	p_d3d_Device->SetMaterial (&mtrl1);

	p_d3d_Device->DrawIndexedPrimitive (D3DPT_TRIANGLELIST, 0, obj.numVerts, 0, obj.numFaces); 

	p_d3d_Device->EndScene ();
	p_d3d_Device->Present (NULL, NULL, NULL, NULL);
};

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