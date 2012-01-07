#include "stdafx.h"

#include "dxHeightMap.h"

dxHeightMap::dxHeightMap () {

	W = H = 0;

	data = NULL;
}

dxHeightMap::~dxHeightMap () {

	Destroy ();
}

int dxHeightMap::GetHeight (int x, int y) {

	if (x >= -W/2 && x <= W/2 &&
		y >= -H/2 && y <= H/2) {

		return (int) data [x + W/2] [H - (y + H/2)];
	}
	return 0;
}

void dxHeightMap::Create (const TCHAR *fName, int w, int h) {

	W = w;
	H = h;

	ifstream fin;

	TCHAR fPath [MAX_PATH];
	_tcscpy_s (fPath, MAX_PATH, PATHTO_HEIGHTMAPS);
	_tcscat_s (fPath, MAX_PATH, fName);

	fin.open (fPath, ios::in | ios::binary);

	if (!fin.fail ()) {
		
		data = new WORD *[W+1];

		forup (W+1) {
			
			data [i] = new WORD [H+1];
		}

		forup (H+1) {
			forupj (W+1) {
				fin.read ((char *) &data [j] [i], 2);
			}
		}
	}
}

void dxHeightMap::Destroy () {

	forup (W+1) {
		DELA (data [i]);
	}

	DELA (data);
}