#pragma once

#include <windows.h>
#include <time.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <fstream>
//#include <vector>

#pragma comment (lib, "d3d8.lib") 
#pragma comment (lib, "d3dx8.lib")

using namespace std;

#define RELEASE(p) {if (p) {(p)->Release (); (p) = NULL;}}
#define DEL(p) {if (p) {delete (p); (p) = NULL;}}
#define DELA(p) {if (p) {delete [] (p); (p) = NULL;}}

#define WIDTH 800
#define HEIGHT 600

struct CUSTOMVERTEX
{ 
	CUSTOMVERTEX () {}
	float x, y, z, nx, ny, nz;
	CUSTOMVERTEX (float x, float y, float z, float nx, float ny, float nz) 
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->nx = nx;
		this->ny = ny;
		this->nz = nz;
	}
};

struct VERTEX_3DPNT
{  
	D3DXVECTOR3 position; 
	D3DXVECTOR3 normal; 
	D3DXVECTOR2 texture; 
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL)

#define forup(x) for (int i=0; i<x; i++)