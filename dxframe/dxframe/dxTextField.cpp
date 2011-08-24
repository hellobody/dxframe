#include "dxTextField.h"

extern LPDIRECT3DDEVICE8 p_d3d_Device;

int dxTextField::focusOnID = -1;

vector <dxTextField *> dxTextField::textFields;

int dxTextField::GetNewID () {

	return 0;
}

int dxTextField::GetID () {
	return ID;
}

dxTextField::dxTextField (const TCHAR *text, dxRect _rct, D3DCOLOR _fontColor) {
	
	// Create a D3DX font object
	if (p_d3d_Device) {
		hFont = CreateFont (20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PROOF_QUALITY, 0, _T("Comic Sans MS"));
		if (S_OK != D3DXCreateFont (p_d3d_Device, hFont, &Font)) {
			trace (_T ("Font did not created."));
		}
	} else trace (_T ("Direct3D device is bad."));

	_stprintf_s (textBuffer, _T("%s"), text);

	// Create a colour for the text - in this case blue
	fontColor = D3DCOLOR_ARGB (255,0,0,255);    

	// Create a rectangle to indicate where on the screen it should be drawn
	rct = _rct;

	ID = GetNewID ();

	textFields.push_back (this);
}

dxTextField::~dxTextField () {
	RELEASE (Font);
}

void dxTextField::Set (TCHAR *text) {
	_stprintf_s (textBuffer, _T("%s"), text);
}

void dxTextField::Set (const TCHAR *text) {
	_stprintf_s (textBuffer, _T("%s"), text);
}

void Update (float dt) {

}

void dxTextField::Draw () {
	if (Font) {
		Font->DrawText (textBuffer, -1, &(RECT) rct, 0, fontColor);
	}
}