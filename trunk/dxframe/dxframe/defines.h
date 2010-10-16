#pragma once

//#include "vld.h" //comment me
#include <time.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <dinput.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <ctime>
#include <map>
#include "tchar.h"

#pragma comment (lib, "d3d8.lib") 
#pragma comment (lib, "d3dx8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")

#define RELEASE(p) {if (p) {(p)->Release (); (p) = NULL;}}
#define DELA(p) {if (p) {delete [] (p); (p) = NULL;}}
#define DEL(p) {if (p) {delete (p); (p) = NULL;}}

#define D3DFVF_3DPNT (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL)

#define forup(x) for (int i=0; i<x; i++)

static void trace (TCHAR *str) {

	TCHAR tStr [500] = _T("");
	_tcscat_s (tStr, 400, str);
	_tcscat_s (tStr, 8, _T("\n"));
	OutputDebugString (tStr);
}

static void trace (float val) {

	TCHAR tStr [500];
	_stprintf_s (tStr, _T("%f"), val);
	_tcscat_s (tStr, 20, _T("f\n"));
	OutputDebugString (tStr);
}

static void trace (int val) {

	trace (float (val));
}

#define WIDTH 1024
#define HEIGHT 768

#define nameSize 256 

class dxObj;

using namespace std;

typedef pair <string, dxObj*> objPair;
typedef map <string, dxObj*> objMap;

struct CUSTOMVERTEX { 

	CUSTOMVERTEX () {}
	float x, y, z, nx, ny, nz;
	CUSTOMVERTEX (float x, float y, float z, float nx, float ny, float nz) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->nx = nx;
		this->ny = ny;
		this->nz = nz;
	}
};

struct VERTEX_3DPNT {  

	D3DXVECTOR3 position; 
	D3DXVECTOR3 normal; 
	//D3DXVECTOR2 texture; 
};