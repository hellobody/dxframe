#ifndef DXOBJ_H
#define DXOBJ_H

#include "defines.h"

class DXFRAME_API dxObj {

protected:

	LPDIRECT3DTEXTURE8 texture;

	char Name [MAX_PATH];

	char TexName [nameSize];

	float opacity;

	D3DMATERIAL8 material;

	LPDIRECT3DINDEXBUFFER8 p_IndexBuffer;
	LPDIRECT3DVERTEXBUFFER8 p_VertexBuffer;

	VERTEX_3DPNT *pOriginalVerts;
	VERTEX_3DPNT *pTransformedVerts;

	int numVerts;	//number of vertex
	int numFaces;	//number of primitives

	D3DXMATRIX transformM;	//transformation matrix
	D3DXMATRIX rotationM;	//rotation matrix
	D3DXMATRIX textureM;	//texture matrix
	D3DXMATRIX scaleM;		//scale matrix
	D3DXMATRIX mainM;		//main model matrix
	D3DXMATRIX tempM;       //temp matrix

	float *pVertsWithNormals;	//pointer to array of vertexes and normal, like this 30,45,65,0,1,0 (first three vertex coordinates, last - normal)
	int *pFaces;				//pointer to array of indexes

public:

	static vector <dxObj *> objs;
	static PDIRECT3DDEVICE8 using_d3d_Device;

	
	dxObj ();
	dxObj (const TCHAR *flName, const char *objName);
	~dxObj ();

	bool CreateFromFile (const TCHAR *flName, const char *objName);
	void InternalDestroy ();

	void Move (float, float, float);
	void Scale (float, float, float);
	void RotateX (float);
	void RotateY (float);
	void RotateZ (float);

	void SetOpacity (float v);
	float GetOpacity ();

	void Transform ();
	void Render ();

	bool IsPick (LONG x, LONG y);
};

#endif