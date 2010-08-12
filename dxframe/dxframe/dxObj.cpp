#include "dxObj.h"
#include "windows.h"
#include "defines.h"

dxObj::dxObj () {

	numVerts = 0;
	numTVerts = 0;
	numFaces = 0;

	pVerts = NULL;
}

dxObj::~dxObj () {

	DEL (pVerts);
}