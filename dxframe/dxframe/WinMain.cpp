#include "WinMain.h"

HWND hWnd;

BOOL bActive;
BOOL bAlwaysActive = FALSE;

LPCWSTR APPNAME		= L"dxframe";
LPCWSTR APPTITLE	= L"dxframe";

LPDIRECT3D8			pD3DObject = NULL;			//direct 3d main interface
LPDIRECT3DDEVICE8	pD3DDevice = NULL;			//direct 3d device

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

bool fullScreen = false;

HINSTANCE handleThisInstance = NULL;
int NCmdShow = 0;

void Exit () {

	SendMessage (hWnd, WM_DESTROY, 0, 0);
}

void EnableCameraMove () {
	enableCameraMove = true;
}

void DisableCameraMove () {
	enableCameraMove = false;
}

void SwitchToOrthographicView () {
	D3DXMatrixOrthoRH (&matProj, WIDTH, HEIGHT, -10000, 10000);	//turn on orthographic camera
	if (pD3DDevice) {
		pD3DDevice->SetTransform (D3DTS_PROJECTION, &matProj);
	}
}

void SwitchToPerspectiveView () {
	D3DXMatrixPerspectiveFovRH (&matProj, D3DX_PI/2, 4.f/3.f, 1.f, 100000.f); //last two edges of drawing, do not set near val < 1.f
	if (pD3DDevice) {
		pD3DDevice->SetTransform (D3DTS_PROJECTION, &matProj);
	}
}

bool InitScreen (HINSTANCE hThisInst, int nCmdShow);

void SwitchScreenMode () {
	fullScreen = !fullScreen;

	InitScreen (handleThisInstance, NCmdShow);
}

void ResetCameraPosition () {

	cCamera newCamera;
	camera = newCamera;

	matView = camera.getViewMatrix ();
	if (pD3DDevice) {
		pD3DDevice->SetTransform (D3DTS_VIEW, &matView);
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

	if (fullScreen)
	{
		hWnd = CreateWindowEx (0, APPNAME, APPTITLE, WS_BORDER | WS_POPUP, d3ddm.Width, d3ddm.Height, WIDTH, HEIGHT, NULL, NULL, hThisInst, NULL);
	}
	else
	{
		int sx = GetSystemMetrics (SM_CXSCREEN);
		int sy = GetSystemMetrics (SM_CYSCREEN);

		hWnd = CreateWindowEx (0, APPNAME, APPTITLE, WS_BORDER | WS_POPUP, sx/2-WIDTH/2, sy/2-HEIGHT/2, WIDTH, HEIGHT, NULL, NULL, hThisInst, NULL);
	}
	
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

void SetDeviceParameters () {

	matView = camera.getViewMatrix ();

	pD3DDevice->SetTransform (D3DTS_VIEW, &matView);
	pD3DDevice->SetTransform (D3DTS_WORLD, &matWorld);
	pD3DDevice->SetTransform (D3DTS_PROJECTION, &matProj);

	pD3DDevice->SetRenderState (D3DRS_CULLMODE, D3DCULL_NONE);
	pD3DDevice->SetRenderState (D3DRS_ZENABLE, D3DZB_TRUE);
	pD3DDevice->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE);
	pD3DDevice->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	
}

bool InitScreen (HINSTANCE hThisInst, int nCmdShow) {

	ZeroMemory (&d3dpp, sizeof (d3dpp));

	if (fullScreen) {

		SetWindowLong (hWnd, GWL_STYLE, WS_POPUP);
		SetWindowPos (hWnd, 0, 0, 0, 640, 480, 0);
		d3dpp.BackBufferWidth = 640;
		d3dpp.BackBufferHeight = 480;
		d3dpp.BackBufferCount = 3;
		d3dpp.FullScreen_RefreshRateInHz = 60;

		d3dpp.BackBufferFormat = D3DFMT_R5G6B5;				//set format of surface of second buffer
	}
	else {

		d3dpp.BackBufferFormat = d3ddm.Format;				//set format of surface of second buffer	
	}

	d3dpp.Windowed = !fullScreen;							//windowed mode
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//set method of window update
	
	//for Z-buffer
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//
	
	if (pD3DDevice) {

		if (FAILED (pD3DDevice->Reset (&d3dpp))) {

			int a = 0;
		}

		SetDeviceParameters ();
	}
	
	
	

	/*if (pD3DDevice) {
		
		if (FAILED (pD3DDevice->Reset (&d3dpp))) {
			
			RELEASE (pD3DDevice);

			if (FAILED (pD3DObject->CreateDevice (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pD3DDevice))) {
				trace (_T("Direct3D device did not created."));
				return false;
			}
		}
		
			
	}*/

	return true;
}

bool AppInit (HINSTANCE hThisInst, int nCmdShow) {

	srand ((int) time (0)); // randomize timer

	if ((pD3DObject = Direct3DCreate8 (D3D_SDK_VERSION)) == NULL) {
		trace (_T("Direct3D instance did not created."));
		return false;
	}

	pD3DObject->GetAdapterDisplayMode (D3DADAPTER_DEFAULT, &d3ddm);

	if (!WindowInit (hThisInst, nCmdShow)) {
		trace (_T("Window initialize error."));
		return false;
	}
	
	if (!InitScreen (handleThisInstance, NCmdShow)) {
		trace (_T("Initialize screen error."));
		return false;
	}

	if (FAILED (pD3DObject->CreateDevice (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pD3DDevice))) {
		trace (_T("Direct3D device did not created."));
		return false;
	}

	if (FAILED (pD3DDevice->Reset (&d3dpp))) {

		return false;
	}

	input.Initialize (hThisInst, hWnd);

	dxObj::objs.clear ();
	dxObj::using_d3d_Device = pD3DDevice;

	D3DXMatrixRotationZ (&matWorld, 0.0f);
	
	D3DXMatrixOrthoRH (&matProj, WIDTH, HEIGHT, -10000, 10000);

	SetDeviceParameters ();

	ShowWindow (hWnd, nCmdShow);

	// Create a D3DX font object
	/*hFont = CreateFont (20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PROOF_QUALITY, 0, _T("Comic Sans MS"));
	if (S_OK != D3DXCreateFont (pD3DDevice, hFont, &Font)) {
		trace (_T ("Font did not created."));
	}*/
	
	MainFrame.Create ();

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

	for (vector <dxObj *>::iterator it = dxObj::objs.begin (); it != dxObj::objs.end (); it++) {

		if (*it) {
			
			(*it)->Transform ();
		}
	}

	static float speed = 800;

	if (enableCameraMove) {

		if (input.IsKeyDown (DIK_W)) camera.walk (-dt * speed);
		if (input.IsKeyDown (DIK_S)) camera.walk (dt * speed);
		if (input.IsKeyDown (DIK_A)) camera.strafe (-dt * speed);
		if (input.IsKeyDown (DIK_D)) camera.strafe (dt * speed);

		camera.yaw (input.GetMouseDeltaX () * -.01f);
		camera.pitch (input.GetMouseDeltaY () * -.01f);

		matView = camera.getViewMatrix ();
		pD3DDevice->SetTransform (D3DTS_VIEW, &matView);
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

	/*if ((input.IsKeyDown (DIK_LALT) || input.IsKeyDown (DIK_RALT)) && input.IsKeyToggledDown (DIK_RETURN)) {
		SwitchScreenMode ();
	}*/

	if (input.IsLeftMouseKeyToggledDown ()) {
		SwitchScreenMode ();
	}
}

void Render () {

	if (pD3DDevice == NULL) 
		return;

	HRESULT hr;
	hr = pD3DDevice->TestCooperativeLevel ();
	if (hr == D3DERR_DEVICELOST) 
		return;
	if (hr == D3DERR_DEVICENOTRESET) {
		
		pD3DDevice->Reset (&d3dpp);
		SetDeviceParameters ();
		/*Font->OnLostDevice ();
		Font->OnResetDevice ();*/
	}

	pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB (255, 0, 0), 1.0f, 0);
	pD3DDevice->BeginScene ();

	static bool q = true;
	
	/*for (objMap::iterator it = dxObj::objs.begin (); it != dxObj::objs.end (); it++) {
		it->second->Render ();
	}*/

	MainFrame.Render ();

	forup ((int) dxTextField::textFields.size ()) {
		if (dxTextField::textFields [i]) {
			dxTextField::textFields [i]->Draw ();
		}
	} 

	//if (showFPS) {
	//	// Create a colour for the text - in this case blue
	//	D3DCOLOR fontColor = D3DCOLOR_ARGB(255,0,0,255);    

	//	// Create a rectangle to indicate where on the screen it should be drawn
	//	RECT rct;
	//	rct.left = 10;
	//	rct.right = 780;
	//	rct.top = 10;
	//	rct.bottom = rct.top + 20;

	//	if (Font) {
	//		TCHAR tS [MAX_PATH];
	//		_stprintf_s (tS, _T("fps: %i"), fps);
	//		Font->DrawText (tS, -1, &rct, 0, fontColor);
	//	}
	//}

	pD3DDevice->EndScene ();
	pD3DDevice->Present (NULL, NULL, NULL, NULL);
}

void Destroy ()	{

	RELEASE (Font);

	MainFrame.Destroy ();

	RELEASE (pD3DDevice);
	RELEASE (pD3DObject);
	
	input.Clean ();

	for (vector <dxObj *>::iterator it = dxObj::objs.begin (); it != dxObj::objs.end (); it++)
	{
		DEL ((*it));
	}
}

int APIENTRY WinMain (HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {

	handleThisInstance = hThisInst;
	NCmdShow = nCmdShow;

	MSG msg;
	ZeroMemory (&msg, sizeof (msg));

	if (!AppInit (hThisInst, nCmdShow)) return false;

	while (msg.message != WM_QUIT) {

		if (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE)) {

			if(!GetMessage (&msg, NULL, 0, 0)) break;
			TranslateMessage (&msg); 
			DispatchMessage (&msg);

		} else if (bActive || bAlwaysActive) {

			Update ();
			Render ();
		}

	} return 0;

	/*
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );

	while( msg.message!=WM_QUIT )
	{
	while( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
	{
	TranslateMessage( &msg );
	DispatchMessage( &msg );
	}

	.. update our game, render etc. here
	}
	*/
}