#ifndef DXOBJ_H
#define DXOBJ_H

#include "defines.h"

class dxObj {
public:

	PDIRECT3DDEVICE8 using_d3d_Device;
	LPDIRECT3DVERTEXBUFFER8 p_VertexBuffer;
	LPDIRECT3DINDEXBUFFER8 p_IndexBuffer;

	int numVerts;	//number of vertex
	int numTVerts;	//number of texture vertexes - do not using now
	int numFaces;	//number of primitives

	D3DXMATRIX mainM;		//main model matrix
	D3DXMATRIX rotationM;	//rotation matrix
	D3DXMATRIX transformM;	//transformation matrix
	D3DXMATRIX scaleM;		//scale matrix
	D3DXMATRIX textureM;	//texture matrix

	float * pVertsWithNormals;	//pointer to array of vertexes and normal, like this 30,45,65,0,1,0 (first three vertex coordinates, last - normal)
	int * pFaces;				//pointer to array of indexes

	dxObj ();
	~dxObj ();
};

#endif