#ifndef DEFINES_H
#define DEFINES_H

#include <windows.h>
#include <time.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <fstream>
#include "dxObj.h"

#pragma comment (lib, "d3d8.lib") 
#pragma comment (lib, "d3dx8.lib")
//#pragma comment (lib, "winmm.lib")

using namespace std;

#define RELEASE(p) {if (p) {(p)->Release (); (p) = NULL;}}
#define DEL(p) {if (p) {delete (p); (p) = NULL;}}
#define DELA(p) {if (p) {delete [] (p); (p) = NULL;}}

#define WIDTH 800
#define HEIGHT 600

struct CUSTOMVERTEX { 
	float x, y, z;
	CUSTOMVERTEX () {};
	CUSTOMVERTEX (float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ)

#define forup(x) for (int i=0; i<x; i++)

#endif