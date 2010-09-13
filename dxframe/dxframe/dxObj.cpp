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
	DELA (pVertsWithNormals);
	DELA (pFaces);

	DELA (pOriginalVerts);
	DELA (pTransformedVerts);

	RELEASE (p_VertexBuffer);
	RELEASE (p_IndexBuffer);
}

void dxObj::Create (LPDIRECT3DDEVICE8 d3d_device, int numVerts, int numFaces) {
	using_d3d_Device = d3d_device; //pointer to d3d device
	pOriginalVerts = new VERTEX_3DPNT [numVerts];
	pTransformedVerts = new VERTEX_3DPNT [numVerts];

	using_d3d_Device->CreateVertexBuffer (numVerts * sizeof (CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &p_VertexBuffer);
	using_d3d_Device->CreateIndexBuffer (numFaces * 12, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &p_IndexBuffer);

	D3DXMatrixIdentity (&mainM);
	D3DXMatrixIdentity (&rotationM);
	D3DXMatrixIdentity (&transformM);
	D3DXMatrixIdentity (&scaleM);
	D3DXMatrixIdentity (&textureM);

	///////////////////////////////////////////////////////////////////////////
	//temporary, rewrite it////////////////////////////////////////////////////
	forup (numVerts) {
		pOriginalVerts[i].position.x = pVertsWithNormals[i*6];
		pOriginalVerts[i].position.y = pVertsWithNormals[i*6+2];
		pOriginalVerts[i].position.z = pVertsWithNormals[i*6+1];

		pOriginalVerts[i].normal.x = pVertsWithNormals[i*6+3];
		pOriginalVerts[i].normal.y = pVertsWithNormals[i*6+4];
		pOriginalVerts[i].normal.z = pVertsWithNormals[i*6+5];
	}

	void *tPointer;
	p_IndexBuffer->Lock (0, numFaces * 12, (BYTE**) &tPointer, 0);
	memcpy (tPointer, pFaces, numFaces * 12);
	p_IndexBuffer->Unlock();
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
}

void dxObj::Transform () {
	memcpy (pTransformedVerts, pOriginalVerts, numVerts * sizeof (CUSTOMVERTEX)); //copy original coordinates to buffer for transformation

	mainM = scaleM * rotationM * transformM;			//computing eventual outcome matrix

	forup (numVerts) {
		D3DXVec3TransformCoord (&pTransformedVerts[i].position, &pTransformedVerts[i].position, &mainM); //transforming positions by eventual outcome matrix 
		D3DXVec3TransformNormal (&pTransformedVerts[i].normal, &pTransformedVerts[i].normal, &mainM); //transforming normals
		D3DXVec3Normalize (&pTransformedVerts[i].normal, &pTransformedVerts[i].normal);				//normalizing normals
	}

	void *tPointer;
	p_VertexBuffer->Lock (0, numVerts * sizeof (CUSTOMVERTEX), (BYTE**) &tPointer, 0);
	memcpy (tPointer, pTransformedVerts, numVerts * sizeof (CUSTOMVERTEX));
	p_VertexBuffer->Unlock ();
}

void dxObj::Render () {
	using_d3d_Device->SetStreamSource (0, p_VertexBuffer, sizeof (CUSTOMVERTEX));
	using_d3d_Device->SetIndices (p_IndexBuffer, 0);
	using_d3d_Device->DrawIndexedPrimitive (D3DPT_TRIANGLELIST, 0, numVerts, 0, numFaces);
}