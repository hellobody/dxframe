#ifndef DXOBJ_H
#define DXOBJ_H

#include "defines.h"

class DXFRAME_API dxObj {

protected:

	LPDIRECT3DTEXTURE9 texture;

	char Name [MAX_PATH];

	char TexName [nameSize];

	float opacity;

	D3DMATERIAL9 material;

	LPDIRECT3DINDEXBUFFER9 p_IndexBuffer;
	LPDIRECT3DVERTEXBUFFER9 p_VertexBuffer;

	////shader
	LPDIRECT3DVERTEXDECLARATION9 vertexDecl;
	LPD3DXCONSTANTTABLE constantTable;
	LPDIRECT3DVERTEXSHADER9 vertexShader;
	LPDIRECT3DPIXELSHADER9 pixelShader;
	LPD3DXBUFFER code;

	bool ShaderOn;
	////

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
	static PDIRECT3DDEVICE9 using_d3d_Device;

	
	dxObj ();
	dxObj (const TCHAR *flName, const char *objName);
	~dxObj ();

	bool CreateFromFile (const TCHAR *flName, const char *objName);
	void InternalDestroy ();

	void Temp_TurnOnShader ();

	void Move (float, float, float);
	void Move (D3DXVECTOR3 v);
	void MoveTo (D3DXVECTOR2 v);
	void MoveTo (D3DXVECTOR3 v);
	void Scale (float, float, float);
	void SetScale (float, float, float);
	void SetScale (float);
	void RotateX (float);
	void RotateY (float);
	void RotateZ (float);

	void SetDirZ (D3DXVECTOR2 v);
	void SetAngZ (float);

	D3DXVECTOR2 GetPos2 ();
	D3DXVECTOR3 GetPos3 ();

	void SetOpacity (float v);
	float GetOpacity ();

	void Transform ();
	void Render ();

	bool IsPick (LONG x, LONG y);
};

#endif