#include "WinMain.h"

HWND hWnd;

BOOL bActive;

LPCWSTR APPNAME = L"dxframe";
LPCWSTR APPTITLE = L"dxframe";

LPDIRECTINPUT8 din = NULL;    // the pointer to our DirectInput interface
LPDIRECTINPUTDEVICE8 dinkeybd = NULL;    // the pointer to the keyboard device
LPDIRECTINPUTDEVICE8 dinmouse = NULL;    // the pointer to the mouse device

DIMOUSESTATE mousestate;    // the storage for the mouse-information
BYTE keystate [256];    // the storage for the key-information

LPDIRECT3D8 p_d3d = NULL;						//direct 3d main interface
LPDIRECT3DDEVICE8 p_d3d_Device = NULL;			//direct 3d device

D3DDISPLAYMODE d3ddm;							//display mode parameters
D3DPRESENT_PARAMETERS d3dpp;					//present parameters

D3DXMATRIX matWorld;
D3DXMATRIX matView;
D3DXMATRIX matProj;

//materials
D3DMATERIAL8 mtrl1;
D3DMATERIAL8 mtrl2;

LPDIRECT3DTEXTURE8 tex1 = NULL;	//the pointer to texture

//light
D3DLIGHT8 light;

ifstream fin;							//file input
dxObj *obj;								//my object

//camera
cCamera camera (D3DXVECTOR3 (0, 0, -250));
//

//timer
float lt = 0; //last clock value
float ct;	//current clock value
float dt; //tick size in seconds
//

//fps
int fps = 0;
//

HFONT hFont = NULL;
LPD3DXFONT Font = NULL;

dxMainFrame MainFrame;

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

	din->CreateDevice (GUID_SysMouse,
		&dinmouse,
		NULL);

	// set the data format to keyboard format
	dinkeybd->SetDataFormat (&c_dfDIKeyboard);
	dinmouse->SetDataFormat (&c_dfDIMouse);

	// set the control you will have over the keyboard
	dinkeybd->SetCooperativeLevel (hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	dinmouse->SetCooperativeLevel (hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
}

// this is the function that closes DirectInput
void cleanDInput () {
	dinkeybd->Unacquire ();    // make sure the keyboard is unacquired
	dinmouse->Unacquire ();    // make sure the mouse in unacquired
	din->Release ();    // close DirectInput before exiting
}

// this is the function that gets the latest input data
void updateInput () {
	// get access if we don't have it already
	dinkeybd->Acquire ();
	dinmouse->Acquire ();

	// get the input data
	dinkeybd->GetDeviceState (256, (LPVOID) keystate);
	dinmouse->GetDeviceState (sizeof (DIMOUSESTATE), (LPVOID) &mousestate);
}

void Destroy ();

LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {

		case WM_ACTIVATE:
			bActive = LOWORD (wParam);
			break;

		case WM_DESTROY:
			Destroy ();
			PostQuitMessage (0);
			break;

		case WM_SETCURSOR:
			//SetCursor (1);
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
	wcl.hCursor = LoadCursor (hThisInst, MAKEINTRESOURCE (IDC_POINTER));
	wcl.lpszMenuName = NULL;
	wcl.cbClsExtra = 0;
	wcl.cbWndExtra = 0;
	wcl.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);

	RegisterClass (&wcl);

	int sx = GetSystemMetrics (SM_CXSCREEN);
	int sy = GetSystemMetrics (SM_CYSCREEN);

	hWnd = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW, APPNAME, APPTITLE, WS_THICKFRAME | WS_SYSMENU, sx/2-WIDTH/2, sy/2-HEIGHT/2, WIDTH, HEIGHT, NULL, NULL, hThisInst, NULL);

	if (!hWnd) return false;
	return true;
}

void getFileNameFromFullPath (const char *fullPath, char *fileName) {

	char s0 [nameSize];
	char s1 [nameSize] = "";
	strcpy_s (s0, nameSize, fullPath);
	strcpy_s (s0, nameSize, _strrev (s0));

	int i = 0;
	while (s0 [i] != '\\') {
		s1 [i] = s0 [i];
		i++;
	}
	strcpy_s (fileName, nameSize, _strrev (s1));
}

bool AppInit (HINSTANCE hThisInst, int nCmdShow) {

	dxLogger Logger;
	Logger.getSystemInfo ();
	Logger.logSystemInfo ();

	Logger.trace (_T("test"));

	if (!WindowInit (hThisInst, nCmdShow)) return false; //init window

	if ((p_d3d = Direct3DCreate8 (D3D_SDK_VERSION)) == NULL) {	//creating main interface
		trace (_T("Direct3D instance did not created."));
		return false; 
	}

	ZeroMemory (&d3dpp, sizeof (d3dpp));		//clear struct

	p_d3d->GetAdapterDisplayMode (D3DADAPTER_DEFAULT, &d3ddm); //get info about current display mode (resolution and parameters) 

#pragma region [test]
	//////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////
	//get total count of available display modes
	int adapterModeCount = p_d3d->GetAdapterModeCount (D3DADAPTER_DEFAULT);

	D3DDISPLAYMODE td3ddm;
	vector <D3DDISPLAYMODE> vVideoModes; 
	forup (adapterModeCount) {

		p_d3d->EnumAdapterModes (D3DADAPTER_DEFAULT, i, &td3ddm);
		vVideoModes.push_back (td3ddm);
	}

	forup (adapterModeCount) {
		
		if (vVideoModes [i].Width == WIDTH &&
			vVideoModes [i].Height == HEIGHT) {
			
			//d3ddm = vVideoModes [i];
			break;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion
	d3dpp.Windowed = true;						//windowed mode
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;	//set method of window update
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

	int errorType;
	if (FAILED (errorType = p_d3d->CreateDevice (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &p_d3d_Device))) {

		switch (errorType) {
			case D3DERR_INVALIDCALL:
				break;
			case D3DERR_NOTAVAILABLE:
				break;
			case D3DERR_OUTOFVIDEOMEMORY:
				break;
		}
		
		trace (_T("Direct3D device did not created."));
		return false;
	}

	dxObj::objs.clear ();
	dxObj::using_d3d_Device = p_d3d_Device;

	D3DXMatrixRotationY (&matWorld, 0.0f);

	//if need ortographic camera
	//D3DXMatrixOrthoLH (&matProj, WIDTH, HEIGHT, -2000, 2000);	//turn on orthographic camera
	//else
	D3DXMatrixPerspectiveFovLH (&matProj, D3DX_PI/2, 4.f/3.f, 1.f, 10000.f); //last two edges of drawing, do not set near val < 1.f
	
	//second param - angle of view, third - aspect ratio

	p_d3d_Device->SetTransform (D3DTS_WORLD, &matWorld);
	p_d3d_Device->SetTransform (D3DTS_PROJECTION, &matProj);
	p_d3d_Device->SetRenderState (D3DRS_CULLMODE, D3DCULL_CCW);

	//init light
	D3DXVECTOR3 vecDir;
	ZeroMemory (&light, sizeof (D3DLIGHT8));
	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;

	vecDir = D3DXVECTOR3 (0.0f, -1.0f, 0.0f);
	D3DXVec3Normalize ((D3DXVECTOR3*) &light.Direction, &vecDir);

	light.Range = 10000.0f;

	p_d3d_Device->SetLight (0, &light);
	p_d3d_Device->LightEnable (0, true);
	
	p_d3d_Device->SetRenderState (D3DRS_LIGHTING, true);
	p_d3d_Device->SetRenderState (D3DRS_AMBIENT, 0);
	//
	
	//for Z-buffer
	p_d3d_Device->SetRenderState (D3DRS_ZENABLE, D3DZB_TRUE);
	//

	// Create a D3DX font object
	hFont = CreateFont (20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PROOF_QUALITY, 0, _T("Comic Sans MS"));
	if (S_OK != D3DXCreateFont (p_d3d_Device, hFont, &Font)) {
		trace (_T ("Font did not created."));
	}

	MainFrame.Create ();

	initDInput (hThisInst, hWnd);
	
	ShowWindow (hWnd, nCmdShow);
	UpdateWindow (hWnd);

	return true;
}

void Update () {

	updateInput ();

	float ct = (float) clock () / CLOCKS_PER_SEC;
	dt = ct - lt;
	lt = ct;

	static int cntFrame = 0;
	static float oneSec = 0;
	cntFrame ++;
	oneSec += dt;

	if (oneSec >= 1.f) {

		fps = cntFrame;
		cntFrame = 0;
		oneSec = 0;
	}

	MainFrame.Update (dt);

	for (objMap::iterator it = dxObj::objs.begin (); it != dxObj::objs.end (); it++) {
		it->second->Transform ();
	}

	///////////////////

	bool dik_space_pressed = false;

	static float speed = 100;

	if (1) {	//off camera move

		if (keystate [DIK_RETURN] & 0x80) speed = 30000;
		if (keystate [DIK_SPACE] & 0x80) {
			if (!dik_space_pressed && speed < 3000000) {
				speed *= 2;
				dik_space_pressed = true;
			}
		} else dik_space_pressed = false;

		if (keystate [DIK_W] & 0x80) camera.walk (dt * speed);
		if (keystate [DIK_S] & 0x80) camera.walk (-dt * speed);
		if (keystate [DIK_A] & 0x80) camera.strafe (-dt * speed);
		if (keystate [DIK_D] & 0x80) camera.strafe (dt * speed);

		camera.yaw (mousestate.lX * .01f);
		camera.pitch (mousestate.lY * .01f);

		matView = camera.getViewMatrix ();
		p_d3d_Device->SetTransform (D3DTS_VIEW, &matView);
	}
}

void Render () {

	p_d3d_Device->Clear (0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB (0, 0, 0), 1.0f, 0);
	p_d3d_Device->BeginScene ();

	static bool q = true;
	for (objMap::iterator it = dxObj::objs.begin (); it != dxObj::objs.end (); it++) {
		it->second->Render ();
	}

	// Create a colour for the text - in this case blue
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255,0,0,255);    

	// Create a rectangle to indicate where on the screen it should be drawn
	RECT rct;
	rct.left = 10;
	rct.right = 780;
	rct.top = 10;
	rct.bottom = rct.top + 20;

	if (Font) {
		TCHAR tS [MAX_PATH];
		_stprintf_s (tS, _T("fps: %i"), fps);
		Font->DrawText (tS, -1, &rct, 0, fontColor);
	}

	p_d3d_Device->EndScene ();
	p_d3d_Device->Present (NULL, NULL, NULL, NULL);
}

void Destroy ()	{

	RELEASE (Font);

	MainFrame.Destroy ();

	RELEASE (p_d3d_Device);
	RELEASE (p_d3d);
	RELEASE (tex1);
	
	cleanDInput ();

	for (objMap::iterator it = dxObj::objs.begin (); it != dxObj::objs.end (); it++)
	{
		DEL (it->second);
	}
}

int APIENTRY WinMain (HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {

	MSG msg;

	if (!AppInit (hThisInst, nCmdShow)) return false;

	while (true) {

		if (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE)) {

			if (!GetMessage (&msg, NULL, 0, 0)) break;
			TranslateMessage (&msg);
			DispatchMessage (&msg);

		} else if (bActive) {

			Update ();
			Render ();
		}
	} return 0;
}