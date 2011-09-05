#include "WinMain.h"

HWND hWnd;

BOOL bActive;
BOOL bAlwaysActive = FALSE;

LPCWSTR APPNAME = L"dxframe";
LPCWSTR APPTITLE = L"dxframe";

LPDIRECT3D8 p_d3d = NULL;						//direct 3d main interface
LPDIRECT3DDEVICE8 p_d3d_Device = NULL;			//direct 3d device

D3DDISPLAYMODE d3ddm;							//display mode parameters
D3DPRESENT_PARAMETERS d3dpp;					//present parameters

D3DXMATRIX matWorld;
D3DXMATRIX matView;
D3DXMATRIX matProj;

//light
D3DLIGHT8 light;

//timer
float lt = 0; //last clock value
float ct;	//current clock value
float dt; //tick size in seconds
//

int fps = 0;

HFONT hFont = NULL;
LPD3DXFONT Font = NULL;

cCamera camera (D3DXVECTOR3 (0, 0, 250));

dxEnvironmentVars environmentVars;
dxIniFileInterface iniFileInterface;

dxMainFrame MainFrame;

dxInput input;
dxLogger logger;

HANDLE hThreadConsole = NULL;

void Destroy ();

bool enableCameraMove = false;
bool showFPS = true;

void EnableCameraMove () {
	enableCameraMove = true;
}

void DisableCameraMove () {
	enableCameraMove = false;
}

void SwitchToOrthographicView () {
	D3DXMatrixOrthoRH (&matProj, WIDTH, HEIGHT, -2000, 2000);	//turn on orthographic camera
	if (p_d3d_Device) {
		p_d3d_Device->SetTransform (D3DTS_PROJECTION, &matProj);
	}
}

void SwitchToPerspectiveView () {
	D3DXMatrixPerspectiveFovRH (&matProj, D3DX_PI/2, 4.f/3.f, 1.f, 10000.f); //last two edges of drawing, do not set near val < 1.f
	if (p_d3d_Device) {
		p_d3d_Device->SetTransform (D3DTS_PROJECTION, &matProj);
	}
}

void ResetCameraPosition () {

	cCamera newCamera;
	camera = newCamera;

	matView = camera.getViewMatrix ();
	if (p_d3d_Device) {
		p_d3d_Device->SetTransform (D3DTS_VIEW, &matView);
	}
}

DWORD WINAPI ThreadConsoleFunction (LPVOID lpParam) 
{ 
	dxConsole console;

	while (console.Update ()) {}

    return 0;
}

LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {

		case WM_ACTIVATE:

			bActive = LOWORD (wParam);
			break;

		case WM_DESTROY:

			Destroy ();
			PostQuitMessage (0);
			break;

		case WM_CLOSE:

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

	logger.getSystemInfo ();
	logger.logSystemInfo ();

	logger.trace (_T("test"));

	if (!WindowInit (hThisInst, nCmdShow)) return false; //init window

	if ((p_d3d = Direct3DCreate8 (D3D_SDK_VERSION)) == NULL) {	//creating main interface
		trace (_T("Direct3D instance did not created."));
		return false; 
	}

	ZeroMemory (&d3dpp, sizeof (d3dpp));		//clear struct

	p_d3d->GetAdapterDisplayMode (D3DADAPTER_DEFAULT, &d3ddm); //get info about current display mode (resolution and parameters) 

#pragma region [try to work with adapter modes here]
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

	//I get frame rate fluctuations in windowed mode

	//When your device is created the presentation interval is set to D3DPRESENT_INTERVAL_DEFAULT. This setting synchronises your frames to to the window rate so there is only one present allowed per frame. This prevents tearing effects (D3DPRESENT_INTERVAL_IMMEDIATE provides unlimited presents).  However if you are seeing fluctuations in frame rate you might be better to try D3DPRESENT_INTERVAL_ONE, this uses a higher resolution timer than the default setting which improves the quality of the vsync but takes up a bit more processing time. To change this value you need to alter the D3DPRESENT_PARAMETERS PresentationInterval value.

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

	D3DXMatrixRotationZ (&matWorld, 0.0f);

	
	//if need ortographic camera
	D3DXMatrixOrthoRH (&matProj, WIDTH, HEIGHT, -2000, 2000);	//turn on orthographic camera
	//else
	//D3DXMatrixPerspectiveFovRH (&matProj, D3DX_PI/2, 4.f/3.f, 1.f, 10000.f); //last two edges of drawing, do not set near val < 1.f
	
	//second param - angle of view, third - aspect ratio

	p_d3d_Device->SetTransform (D3DTS_WORLD, &matWorld);
	p_d3d_Device->SetTransform (D3DTS_PROJECTION, &matProj);
	p_d3d_Device->SetRenderState (D3DRS_CULLMODE, D3DCULL_NONE);

	matView = camera.getViewMatrix ();
	p_d3d_Device->SetTransform (D3DTS_VIEW, &matView);

	//init light
	D3DXVECTOR3 vecDir;
	ZeroMemory (&light, sizeof (D3DLIGHT8));
	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;

	vecDir = D3DXVECTOR3 (0.0f, 0.0f, 1.0f);
	D3DXVec3Normalize ((D3DXVECTOR3*) &light.Direction, &vecDir);

	light.Range = 10000.0f;

	p_d3d_Device->SetLight (0, &light);
	p_d3d_Device->LightEnable (0, true);
	
	p_d3d_Device->SetRenderState (D3DRS_LIGHTING, true);
	//p_d3d_Device->SetRenderState (D3DRS_AMBIENT, 0);
	//
	
	p_d3d_Device->SetRenderState (D3DRS_ZENABLE, D3DZB_TRUE);

	//try use alpha blend
	p_d3d_Device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    p_d3d_Device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    p_d3d_Device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	//

	input.Initialize (hThisInst, hWnd);

	// Create a D3DX font object
	hFont = CreateFont (20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PROOF_QUALITY, 0, _T("Comic Sans MS"));
	if (S_OK != D3DXCreateFont (p_d3d_Device, hFont, &Font)) {
		trace (_T ("Font did not created."));
	}
	
	MainFrame.Create ();
	
	ShowWindow (hWnd, nCmdShow);
	UpdateWindow (hWnd);

	return true;
}

void Update () {

	input.Update ();

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

	static float speed = 800;

	if (enableCameraMove) {

		if (input.IsKeyDown (DIK_W)) camera.walk (-dt * speed);
		if (input.IsKeyDown (DIK_S)) camera.walk (dt * speed);
		if (input.IsKeyDown (DIK_A)) camera.strafe (-dt * speed);
		if (input.IsKeyDown (DIK_D)) camera.strafe (dt * speed);

		camera.yaw (input.GetMouseDeltaX () * -.01f);
		camera.pitch (input.GetMouseDeltaY () * -.01f);

		matView = camera.getViewMatrix ();
		p_d3d_Device->SetTransform (D3DTS_VIEW, &matView);
	}

	if (input.IsKeyToggledDown (DIK_ESCAPE)) {
		SendMessage (hWnd, WM_DESTROY, 0, 0);
	}

	if (input.IsKeyToggledDown (DIK_GRAVE)) {

		if (hThreadConsole) {

			TerminateThread (hThreadConsole, 0);
			hThreadConsole = NULL;
		} else {

			DWORD ThreadID;
			hThreadConsole = CreateThread (NULL, 0, ThreadConsoleFunction, 0, 0, &ThreadID);		
		}
	}
}

void Render () {

	if (p_d3d_Device == NULL) {
		return;
	}

	p_d3d_Device->Clear (0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB (0, 0, 0), 1.0f, 0);
	p_d3d_Device->BeginScene ();

	static bool q = true;
	for (objMap::iterator it = dxObj::objs.begin (); it != dxObj::objs.end (); it++) {
		it->second->Render ();
	}

	forup ((int) dxTextField::textFields.size ()) {
		if (dxTextField::textFields [i]) {
			dxTextField::textFields [i]->Draw ();
		}
	} 

	if (showFPS) {
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
	}

	p_d3d_Device->EndScene ();
	p_d3d_Device->Present (NULL, NULL, NULL, NULL);
}

void Destroy ()	{

	RELEASE (Font);

	MainFrame.Destroy ();

	RELEASE (p_d3d_Device);
	RELEASE (p_d3d);
	
	input.Clean ();

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

		} else if (bActive || bAlwaysActive) {

			Update ();
			Render ();
		}
	} return 0;
}