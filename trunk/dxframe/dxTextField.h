#ifndef DXTEXTFIELD_H
#define DXTEXTFIELD_H

#include "defines.h"

class dxTextField
{
protected:

	static const int maxTextSize = 10000;

	int ID;

	static int focusOnID;

	HFONT hFont;
	LPD3DXFONT Font;

	D3DCOLOR fontColor;
	dxRect rct;

	TCHAR textBuffer [maxTextSize];

public:

	static int GetNewID ();
	int GetID ();

	static vector <dxTextField *> textFields;

	dxTextField (const TCHAR *text, dxRect _rct, D3DCOLOR _fontColor = D3DCOLOR_ARGB (255,0,0,255));
	
	virtual ~dxTextField (); 

	void Set (TCHAR *text);
	void Set (const TCHAR *text);
	
	void Update (float dt);
	void Draw ();
};

#endif