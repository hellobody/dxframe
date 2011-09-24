#ifdef DXFRAME_EXPORTS
#define DXFRAME_API __declspec(dllexport)
#else
#define DXFRAME_API __declspec(dllimport)
#endif

extern DXFRAME_API bool fullScreen;

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
	float Update (HWND hWnd);
	HRESULT RenderBegin ();
	void RenderEnd ();
	void Destroy ();
};
