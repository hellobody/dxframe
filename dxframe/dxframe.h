#include "dxInput.h"
#include "cCamera.h"

#ifdef DXFRAME_EXPORTS
#define DXFRAME_API __declspec(dllexport)
#else
#define DXFRAME_API __declspec(dllimport)
#endif

extern DXFRAME_API bool fullScreen;

extern DXFRAME_API dxInput input;

extern DXFRAME_API cCamera camera;

class DXFRAME_API dxFrame {

public:

	dxFrame ();
	~dxFrame ();

	void SwitchScreenMode (HWND hWnd);
	void ResetCameraPosition ();
	void SetDeviceParameters ();
	bool InitScreen (HWND hWnd);
	void GetAllDisplayModes ();

	bool Create (HINSTANCE hThisInst, int nCmdShow, HWND hWnd);
	float Update (HWND hWnd);
	HRESULT RenderBegin ();
	void RenderEnd ();
	void Destroy ();
};
