#include "stdafx.h"
#include "dxframe.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

LPDIRECT3D8			pD3DObject = NULL;			//direct 3d main interface
LPDIRECT3DDEVICE8	pD3DDevice = NULL;			//direct 3d device

D3DDISPLAYMODE d3ddmW;							//display mode parameters for windewed mode
D3DDISPLAYMODE d3ddmFS;							//display mode parameters for full screen mode
D3DDISPLAYMODE d3ddm;
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

cCamera camera (D3DXVECTOR3 (0, 0, 250));

dxEnvironmentVars environmentVars;
dxIniFileInterface iniFileInterface;

//dxMainFrame MainFrame;

DXFRAME_API dxInput input;
DXFRAME_API dxAudio audio;
dxLogger logger;

HANDLE hThreadConsole = NULL;

DXFRAME_API bool fullScreen = false;

bool enableCameraMove = false;
bool showFPS = true;

vector <D3DDISPLAYMODE> vVideoModes;

BOOL APIENTRY DllMain (HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

dxFrame::dxFrame () {
	
	
}

dxFrame::~dxFrame () {

	
}

void dxFrame::SwitchScreenMode (HWND hWnd) {

	fullScreen = !fullScreen;

	if (fullScreen) {
		d3ddm = d3ddmFS;
	} else {
		d3ddm = d3ddmW;
	}

	InitScreen (hWnd);
}

void dxFrame::ResetCameraPosition () {

	cCamera newCamera;
	camera = newCamera;

	matView = camera.getViewMatrix ();
	if (pD3DDevice) {
		pD3DDevice->SetTransform (D3DTS_VIEW, &matView);
	}
}

void dxFrame::SetDeviceParameters () {

	matView = camera.getViewMatrix ();

	pD3DDevice->SetTransform (D3DTS_VIEW, &matView);
	pD3DDevice->SetTransform (D3DTS_WORLD, &matWorld);
	pD3DDevice->SetTransform (D3DTS_PROJECTION, &matProj);

	pD3DDevice->SetRenderState (D3DRS_CULLMODE, D3DCULL_NONE);
	pD3DDevice->SetRenderState (D3DRS_ZENABLE, D3DZB_TRUE);
	pD3DDevice->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE);
	pD3DDevice->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3DDevice->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pD3DDevice->SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pD3DDevice->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pD3DDevice->SetTextureStageState (0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pD3DDevice->SetVertexShader (D3DFVF_CUSTOMVERTEX);	//for ver 8
}

bool dxFrame::InitScreen (HWND hWnd) {

	ZeroMemory (&d3dpp, sizeof (d3dpp));

	if (fullScreen) {

		SetWindowLong (hWnd, 0, WS_BORDER | WS_POPUP);
		SetWindowPos (hWnd, 0, 0, 0, d3ddm.Width, d3ddm.Height, 0);
		d3dpp.BackBufferWidth = d3ddm.Width;
		d3dpp.BackBufferHeight = d3ddm.Height;
		d3dpp.BackBufferCount = 1;
		d3dpp.FullScreen_RefreshRateInHz = d3ddm.RefreshRate;

	} else {

		int sx = GetSystemMetrics (SM_CXSCREEN);
		int sy = GetSystemMetrics (SM_CYSCREEN);

		SetWindowLong (hWnd, 0, WS_BORDER | WS_POPUP);
		SetWindowPos (hWnd, 0, sx/2-WIDTH/2, sy/2-HEIGHT/2, WIDTH, HEIGHT, 0);
		d3dpp.BackBufferWidth = WIDTH;
		d3dpp.BackBufferHeight = HEIGHT;

	}

	d3dpp.Windowed = !fullScreen;				//windowed mode
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		//set method of window update
	d3dpp.BackBufferFormat = d3ddm.Format;				//set format of surface of second buffer

	//for Z-buffer
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//

	if (pD3DDevice) {
		if (FAILED (pD3DDevice->Reset (&d3dpp))) {
			return false;
		}
		SetDeviceParameters ();
	}

	return true;
}

void dxFrame::GetAllDisplayModes () {

	//int adapterModeCount = pD3DObject->GetAdapterModeCount (D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);	//ver 9
	int adapterModeCount = pD3DObject->GetAdapterModeCount (D3DADAPTER_DEFAULT);

	D3DDISPLAYMODE td3ddm;

	forup (adapterModeCount) {

		//pD3DObject->EnumAdapterModes (D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, i, &td3ddm);	//ver 9
		pD3DObject->EnumAdapterModes (D3DADAPTER_DEFAULT, i, &td3ddm);
		vVideoModes.push_back (td3ddm);

		if (td3ddm.Width == WIDTH &&
			td3ddm.Height == HEIGHT) {

				d3ddmW = td3ddm;	//get last mode for this window resolution
		}
	}

	d3ddmFS = vVideoModes [vVideoModes.size () - 1];
}

bool dxFrame::Create (HINSTANCE hThisInst, int nCmdShow, HWND hWnd) {

	if ((pD3DObject = Direct3DCreate8 (D3D_SDK_VERSION)) == NULL) {
		trace (_T("Direct3D instance did not created."));
		return false;
	}

	GetAllDisplayModes ();

	if (fullScreen) {
		d3ddm = d3ddmFS;
	} else {
		d3ddm = d3ddmW;
	}
	
	if (!InitScreen (hWnd)) {
		trace (_T("Initialize screen error."));
		return false;
	}

	if (FAILED (pD3DObject->CreateDevice (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pD3DDevice))) {
		trace (_T("Direct3D device did not created."));
		return false;
	}

	//init light
	D3DXVECTOR3 vecDir;
	ZeroMemory (&light, sizeof (D3DLIGHT8));
	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;

	vecDir = D3DXVECTOR3 (0.0f, 0.0f, -1.0f);
	D3DXVec3Normalize ((D3DXVECTOR3*) &light.Direction, &vecDir);

	light.Range = 10000.0f;

	pD3DDevice->SetLight (0, &light);
	pD3DDevice->LightEnable (0, true);

	pD3DDevice->SetRenderState (D3DRS_LIGHTING, true);
	pD3DDevice->SetRenderState (D3DRS_AMBIENT, 0);
	//

	input.Initialize (hThisInst, hWnd);

	audio.Initialize ();
	audio.SetAudioFilesDirectory (_T("data\\sounds\\"));

	dxObj::objs.clear ();
	dxObj::using_d3d_Device = pD3DDevice;

	D3DXMatrixRotationZ (&matWorld, 0.0f);
	
	D3DXMatrixOrthoRH (&matProj, WIDTH, HEIGHT, -10000, 10000);

	SetDeviceParameters ();

	return true;
}

float dxFrame::Update (HWND hWnd) {

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

	//MainFrame.Update (dt);

	for (vector <dxObj *>::iterator it = dxObj::objs.begin (); it != dxObj::objs.end (); it++) {

		if (*it) {

			(*it)->Transform ();
		}
	}

	/*static float col = 0;
	col += dt;

	float val = abs (cos (col));

	light.Diffuse.r  = val;
	light.Diffuse.g  = val;
	light.Diffuse.b  = val;
	light.Diffuse.a  = val;

	if (pD3DDevice) pD3DDevice->SetLight (0, &light);*/

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

	/*if (input.IsKeyToggledDown (DIK_ESCAPE)) {
		SendMessage (hWnd, WM_DESTROY, 0, 0);
	}*/
	
	if ((input.IsKeyDown (DIK_LALT) || input.IsKeyDown (DIK_RALT)) && 
		(input.IsKeyToggledDown (DIK_RETURN) || input.IsKeyToggledDown (DIK_NUMPADENTER))) {
		SwitchScreenMode (hWnd);
	}

	return dt;
}

HRESULT dxFrame::RenderBegin () {

	if (pD3DDevice == NULL) 
		return S_FALSE;

	HRESULT hr;
	hr = pD3DDevice->TestCooperativeLevel ();
	if (hr == D3DERR_DEVICELOST) 
		return S_FALSE;
	if (hr == D3DERR_DEVICENOTRESET) {
		
		pD3DDevice->Reset (&d3dpp);
		SetDeviceParameters ();
	}

	pD3DDevice->Clear (0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB (0, 0, 0), 1.0f, 0);
	pD3DDevice->BeginScene ();

	return hr;
}

void dxFrame::RenderEnd () {

	pD3DDevice->EndScene ();
	pD3DDevice->Present (NULL, NULL, NULL, NULL);
}

void dxFrame::Destroy () {

	RELEASE (pD3DDevice);
	RELEASE (pD3DObject);

	input.Clean ();

	for (vector <dxObj *>::iterator it = dxObj::objs.begin (); it != dxObj::objs.end (); it++)
	{
		DEL ((*it));
	}
}
