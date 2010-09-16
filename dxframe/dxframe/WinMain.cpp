#include "WinMain.h"
#include "TCHAR.h"

HWND hWnd;
BOOL bActive;
LPCWSTR APPNAME = L"dxframe";
LPCWSTR APPTITLE = L"dxframe";

LPDIRECTINPUT8 din;    // the pointer to our DirectInput interface
LPDIRECTINPUTDEVICE8 dinkeybd;    // the pointer to the keyboard device
LPDIRECTINPUTDEVICE8 dinmouse;    // the pointer to the mouse device

DIMOUSESTATE mousestate;    // the storage for the mouse-information
BYTE keystate [256];    // the storage for the key-information

LPDIRECT3D8 p_d3d = NULL;						//direct 3d main interface
LPDIRECT3DDEVICE8 p_d3d_Device = NULL;			//direct 3d device
LPDIRECT3DINDEXBUFFER8 p_IndexBuffer = NULL;	//indexes buffer
LPDIRECT3DVERTEXBUFFER8 p_VertexBuffer = NULL;	//vertex buffer

D3DDISPLAYMODE d3ddm;							//display mode parameters
D3DPRESENT_PARAMETERS d3dpp;					//present parameters

D3DXMATRIX matWorld;
D3DXMATRIX matView;
D3DXMATRIX matProj;

//materials
D3DMATERIAL8 mtrl1;
D3DMATERIAL8 mtrl2;

//light
D3DLIGHT8 light;

ifstream fin;							//file input
objMap objs;						
dxObj *obj;								//my object

void initDInput (HINSTANCE hInstance, HWND hWnd) {
	// create the DirectInput interface
	DirectInput8Create (hInstance,    // the handle to the application
		DIRECTINPUT_VERSION,    // the compatible version
		IID_IDirectInput8,    // the DirectInput interface version
		(void**)&din,    // the pointer to the interface
		NULL);    // COM stuff, so we'll set it to NULL

	// create the keyboard device
	din->CreateDevice (GUID_SysKeyboard,    // the default keyboard ID being used
		&dinkeybd,    // the pointer to the device interface
		NULL);    // COM stuff, so we'll set it to NULL

	din->CreateDevice(GUID_SysMouse,
		&dinmouse,
		NULL);

	// set the data format to keyboard format
	dinkeybd->SetDataFormat (&c_dfDIKeyboard);
	dinmouse->SetDataFormat (&c_dfDIMouse);

	// set the control you will have over the keyboard
	dinkeybd->SetCooperativeLevel (hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dinmouse->SetCooperativeLevel (hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
}

// this is the function that closes DirectInput
void cleanDInput ()
{
	dinkeybd->Unacquire ();    // make sure the keyboard is unacquired
	dinmouse->Unacquire ();    // make sure the mouse in unacquired
	din->Release ();    // close DirectInput before exiting
}

// this is the function that gets the latest input data
void updateInput ()
{
	// get access if we don't have it already
	dinkeybd->Acquire ();
	dinmouse->Acquire ();

	// get the input data
	dinkeybd->GetDeviceState (256, (LPVOID) keystate);
	dinmouse->GetDeviceState (sizeof (DIMOUSESTATE), (LPVOID) &mousestate);
}

LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {

		case WM_ACTIVATE:
			bActive = LOWORD (wParam);
			break;

		case WM_DESTROY:
			RELEASE (p_VertexBuffer);
			RELEASE (p_d3d_Device);
			RELEASE (p_d3d);
			
			cleanDInput ();

			for (objMap::iterator it = objs.begin (); it != objs.end (); it++)
			{
				DEL (it->second);
			}

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

	hWnd = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW, APPNAME, APPTITLE, WS_THICKFRAME | WS_SYSMENU, 0, 0, WIDTH, HEIGHT, NULL, NULL, hThisInst, NULL);

	if (!hWnd) return false;
	return true;
}

bool AppInit (HINSTANCE hThisInst, int nCmdShow) {

	if (!WindowInit (hThisInst, nCmdShow)) return false; //init window

	p_d3d = Direct3DCreate8 (D3D_SDK_VERSION); //creating main interface
	p_d3d->GetAdapterDisplayMode (D3DADAPTER_DEFAULT, &d3ddm); //get info about current display mode (resolution and parameters) 

	ZeroMemory (&d3dpp, sizeof (d3dpp));		//clear struct
	d3dpp.Windowed = true;						//windowed mode
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//set method of window update
	d3dpp.BackBufferFormat = d3ddm.Format;		//set format of surface of second buffer

	//for Z-buffer
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
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

		do {

			obj = new dxObj;

			fin.read ((char *) &obj->Name, nameSize);
			fin.read ((char *) &obj->numVerts, 4);
			fin.read ((char *) &obj->numFaces, 4);

			obj->pVertsWithNormals = new float [obj->numVerts * 3 * 2]; //because verts with normals

			forup (obj->numVerts * 3 * 2) {
				fin.read ((char *) &obj->pVertsWithNormals[i], 4);
			}

			obj->pFaces = new int [obj->numFaces * 3]; 

			forup (obj->numFaces * 3) {
				fin.read ((char *) &obj->pFaces[i], 4);
			}

			//Read vertexes
			CUSTOMVERTEX *g_Vertices;
			g_Vertices = new CUSTOMVERTEX [obj->numVerts];
			forup (obj->numVerts) {
				g_Vertices[i] = CUSTOMVERTEX (obj->pVertsWithNormals[i*6], obj->pVertsWithNormals[i*6+1], obj->pVertsWithNormals[i*6+2], obj->pVertsWithNormals[i*6+3], obj->pVertsWithNormals[i*6+4], obj->pVertsWithNormals[i*6+5]);
			}

			//Create vertex buffer
			p_d3d_Device->CreateVertexBuffer (obj->numVerts * sizeof (CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &p_VertexBuffer);

			VOID * pVertices;
			p_VertexBuffer->Lock (0, obj->numVerts * sizeof (CUSTOMVERTEX), (BYTE**)&pVertices, 0);
			memcpy (pVertices, g_Vertices, obj->numVerts * sizeof (CUSTOMVERTEX));
			p_VertexBuffer->Unlock ();

			DELA (g_Vertices);

			//Create index buffer
			p_d3d_Device->CreateIndexBuffer (obj->numFaces * 12, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &p_IndexBuffer);

			VOID* pVerticesI;
			p_IndexBuffer->Lock (0, obj->numFaces * 12, (BYTE**)&pVerticesI, 0);
			memcpy (pVerticesI, obj->pFaces, obj->numFaces * 12);
			p_IndexBuffer->Unlock();

			obj->Create (p_d3d_Device, obj->numVerts, obj->numFaces);

			objs.insert (objPair (obj->Name, obj)); //push new model to map 

			char endByte;
			fin.read ((char *) &endByte, 1);

			if (!fin.eof ()) {
				fin.seekg (int (fin.tellg ()) - 1);
			}

		} while (!fin.eof ());
		
		fin.close ();
	}

	D3DXMatrixRotationY (&matWorld, 0.0f);
	D3DXMatrixLookAtLH (&matView, &D3DXVECTOR3 (0.0f, 0.0f, -500.0f), &D3DXVECTOR3 (0.f, 0.f, 0.f), &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));
	D3DXMatrixPerspectiveFovLH (&matProj, D3DX_PI/2, 4.f/3.f, 1.f, 10000.f); //last two edges of drawing, do not set near val < 1.f
	//second param - angle of view, third - aspect ratio

	p_d3d_Device->SetTransform (D3DTS_VIEW, &matView);
	p_d3d_Device->SetTransform (D3DTS_WORLD, &matWorld);
	p_d3d_Device->SetTransform (D3DTS_PROJECTION, &matProj);
	p_d3d_Device->SetRenderState (D3DRS_CULLMODE, D3DCULL_CW);

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

	//init light
	D3DXVECTOR3 vecDir;
	ZeroMemory (&light, sizeof(D3DLIGHT8));
	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;

	vecDir = D3DXVECTOR3 (0.0f, 0.0f, 1.0f);
	D3DXVec3Normalize ((D3DXVECTOR3*)&light.Direction, &vecDir);

	light.Range = 10000.0f;

	p_d3d_Device->SetLight (0, &light);
	p_d3d_Device->LightEnable (0, true);

	p_d3d_Device->SetRenderState (D3DRS_LIGHTING, true);
	p_d3d_Device->SetRenderState (D3DRS_AMBIENT, 0);
	//
	
	//for Z-buffer
	p_d3d_Device->SetRenderState (D3DRS_ZENABLE, D3DZB_TRUE);
	//

	initDInput (hThisInst, hWnd);
	
	ShowWindow (hWnd, nCmdShow);
	UpdateWindow (hWnd);

	return true;
}
void Render ()
{
	//move to Update
	updateInput ();

	static D3DXVECTOR3 pEye = D3DXVECTOR3 (0,0,-400);
	static D3DXVECTOR3 pAt = D3DXVECTOR3 (0,0,0);
	static D3DXVECTOR3 pUp = D3DXVECTOR3 (0,1,0);

	

	static float ang0 = 0;
	static float ang1 = 0;

	ang0 += mousestate.lX * .005f;
	ang1 -= mousestate.lY * .005f;

	if (ang1 > D3DX_PI/2) ang1 = D3DX_PI/2;
	else if (ang1 < -D3DX_PI/2) ang1 = -D3DX_PI/2;

	D3DXVECTOR3 dir = D3DXVECTOR3 (sin (ang0), sin (ang1), cos (ang0));

	pAt = pEye + dir;

	D3DXMatrixLookAtLH (&matView, &pEye, &pAt, &pUp);
	p_d3d_Device->SetTransform (D3DTS_VIEW, &matView);
	
	if (keystate [DIK_W] & 0x80) {
		pEye += dir * 3.f;
	}
	if (keystate [DIK_S] & 0x80) {
		pEye -= dir * 3.f;
	}
	if (keystate [DIK_A] & 0x80) {
		pEye -= dir2 * 3.f;
	}
	if (keystate [DIK_D] & 0x80) {
		pEye += dir2 * 3.f;
	}
	
	/*if (keystate [DIK_LEFT] & 0x80) {
		pEye.x -= 3.f;
	}
	if (keystate [DIK_RIGHT] & 0x80) {
		pEye.x += 3.f;
	}
	if (keystate [DIK_UP] & 0x80) {
		pEye.z += 3.f;
	}
	if (keystate [DIK_DOWN] & 0x80) {
		pEye.z -= 3.f;
	}*/

	for (objMap::iterator it = objs.begin (); it != objs.end (); it++) {

		//it->second->RotateY (D3DX_PI/500);
		//it->second->Move (mousestate.lX * .5f, -mousestate.lY * .5f, mousestate.lZ * .5f);
		it->second->Transform ();
	}
	//

	p_d3d_Device->Clear (0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB (255, 255, 255), 1.0f, 0);
	p_d3d_Device->BeginScene ();

	p_d3d_Device->SetMaterial (&mtrl1);

	for (objMap::iterator it = objs.begin (); it != objs.end (); it++) {
		it->second->Render ();
	}
	
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