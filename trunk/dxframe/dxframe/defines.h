#ifndef DEFINES_H
#define DEFINES_H

#include <windows.h>
#include <time.h>
#include <d3d8.h>
#include <fstream>
#include "dxObj.h"

using namespace std;

#pragma comment (lib, "d3d8.lib") 

#define RELEASE(p) {if (p) {(p)->Release (); (p) = NULL;}}
#define DEL(p) {if (p) {delete (p); (p) = NULL;}}
#define DELA(p) {if (p) {delete [] (p); (p) = NULL;}}

struct CUSTOMVERTEX { 
	float x, y, z, rhw; 
	DWORD color; 
	CUSTOMVERTEX () {
		
	}
	CUSTOMVERTEX (float x, float y, float z, float rhw, DWORD color) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->rhw = rhw; 
		this->color = color;
	}
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

#define forup(x) for (int i=0; i<x; i++)

#endif