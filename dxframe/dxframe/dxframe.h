// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DXFRAME_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DXFRAME_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DXFRAME_EXPORTS
#define DXFRAME_API __declspec(dllexport)
#else
#define DXFRAME_API __declspec(dllimport)
#endif

extern DXFRAME_API bool fullScreen;

// This class is exported from the dxframe.dll
class DXFRAME_API dxFrame {

public:

	dxFrame ();
	~dxFrame ();

	void SwitchScreenMode (HINSTANCE hThisInst, int nCmdShow, HWND hWnd);
	void ResetCameraPosition ();
	void SetDeviceParameters ();
	bool InitScreen (HINSTANCE hThisInst, int nCmdShow, HWND hWnd);
	void GetAllDisplayModes ();

	bool Create (HINSTANCE hThisInst, int nCmdShow, HWND hWnd);
	void Update (HWND hWnd);
	HRESULT RenderBegin ();
	void RenderEnd ();
	void Destroy ();
};

extern DXFRAME_API int ndxframe;

DXFRAME_API int fndxframe (void);
