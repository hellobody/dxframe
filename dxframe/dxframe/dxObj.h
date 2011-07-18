#ifndef DXOBJ_H
#define DXOBJ_H

#include "defines.h"

class dxObj {

public:

	static objMap objs;
	static PDIRECT3DDEVICE8 using_d3d_Device;

	char Name [MAX_PATH];

	TCHAR TexName [nameSize];

	LPDIRECT3DINDEXBUFFER8 p_IndexBuffer;
	LPDIRECT3DVERTEXBUFFER8 p_VertexBuffer;

	VERTEX_3DPNT *pOriginalVerts;
	VERTEX_3DPNT *pTransformedVerts;

	int numVerts;	//number of vertex
	int numFaces;	//number of primitives
	int numTVerts;	//number of texture vertexes	//?
	
	D3DXMATRIX transformM;	//transformation matrix
	D3DXMATRIX rotationM;	//rotation matrix
	D3DXMATRIX textureM;	//texture matrix
	D3DXMATRIX scaleM;		//scale matrix
	D3DXMATRIX mainM;		//main model matrix
	D3DXMATRIX tempM;       //temp matrix

	float * pVertsWithNormals;	//pointer to array of vertexes and normal, like this 30,45,65,0,1,0 (first three vertex coordinates, last - normal)
	int * pFaces;				//pointer to array of indexes

	dxObj ();
	~dxObj ();

	void Create (LPDIRECT3DDEVICE8 d3d_device, int numVerts, int numIndexes);	//depracated, prepare to delete
	bool CreateNew (const TCHAR *flName, const TCHAR *objName);
	void InternalDestroy ();

	void Move (float, float, float);
	void Scale (float, float, float);
	void RotateX (float);
	void RotateY (float);
	void RotateZ (float);

	void Transform ();
	void Render (D3DMATERIAL8 tempMtrl, LPDIRECT3DTEXTURE8 tempTex);
};

#endif