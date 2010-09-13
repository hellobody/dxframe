#include "dxObj.h"
#include "windows.h"

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

void dxObj::Create (LPDIRECT3DDEVICE8 d3d_device, int numVerts, int numFaces) {
	using_d3d_Device = d3d_device; //pointer to d3d device
	VERTEX_3DPNT *pOriginalVerts = new VERTEX_3DPNT [numVerts];
	VERTEX_3DPNT *pTransformedVerts = new VERTEX_3DPNT [numVerts];

	using_d3d_Device->CreateVertexBuffer (numVerts * sizeof (VERTEX_3DPNT), 0, D3DFVF_3DPNT, D3DPOOL_MANAGED, &p_VertexBuffer);
	using_d3d_Device->CreateIndexBuffer (numFaces * 12, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &p_IndexBuffer);
}

void dxObj::Transform ()
{
	memcpy (pTransformedVerts, pOriginalVerts, numVerts); //copy original coordinates to buffer for transformation

	mainM = scaleM * rotationM * transformM;			//computing eventual outcome matrix

	forup (numVerts)
	{
		D3DXVec3TransformCoord (&pTransformedVerts[i].position, &pTransformedVerts[i].position, &mainM); //transforming positions by eventual outcome matrix 
		D3DXVec3TransformNormal (&pTransformedVerts[i].normal, &pTransformedVerts[i].normal, &mainM); //transforming normals
		D3DXVec3Normalize (&pTransformedVerts[i].normal, &pTransformedVerts[i].normal);				//normalizing normals
	}

	void *tPointer = NULL;
	p_VertexBuffer->Lock (0, numVerts * sizeof (CUSTOMVERTEX), (BYTE**) &tPointer, 0);
	memcpy (tPointer, pTransformedVerts, numVerts * sizeof (CUSTOMVERTEX));
	p_VertexBuffer->Unlock ();
}