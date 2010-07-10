#ifndef DEFINES_H
#define DEFINES_H

#include <windows.h>
#include <time.h>
#include <d3d8.h>
#include <fstream>

using namespace std;

#pragma comment (lib, "d3d8.lib") 

#define RELEASE(p) {if (p) {(p)->Release (); (p) = NULL;}}
#define DEL(p) {if (p) {delete (p); (p) = NULL;}}
#define DELA(p) {if (p) {delete [] (p); (p) = NULL;}}

struct CUSTOMVERTEX { float x, y, z, rhw; DWORD color; };

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

class dxObj {
public:

	int numVerts;
	int numTVerts;
	int numFaces;

	float * pVerts;

	dxObj () {
		numVerts = 0;
		numTVerts = 0;
		numFaces = 0;

		pVerts = NULL;
	};
	~dxObj () {
	}; 
};

#endif