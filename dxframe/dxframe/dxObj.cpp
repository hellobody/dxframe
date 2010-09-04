#include "dxObj.h"
#include "windows.h"
#include "defines.h"

dxObj::dxObj () {

	numVerts = 0;
	numTVerts = 0;
	numFaces = 0;

	pVertsWithNormals = NULL;
	pFaces = NULL;
}

dxObj::~dxObj () {
	if (pVertsWithNormals) delete [] pVertsWithNormals;
	if (pFaces) delete [] pFaces;
}