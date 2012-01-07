#ifndef DXHEIGHTMAP
#define DXHEIGHTMAP

#include "defines.h"

class DXFRAME_API dxHeightMap {

protected:

	int W, H;

	WORD **data;

public:

	dxHeightMap ();
	~dxHeightMap ();

	int GetHeight (int x, int y);

	void Create (const TCHAR *fName, int w, int h);
	void Destroy ();
};

#endif