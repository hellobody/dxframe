#include "dxObj.h"
#include "windows.h"

dxObj::dxObj () {

	using_d3d_Device = NULL;
	p_IndexBuffer = NULL;
	p_VertexBuffer = NULL;

	pOriginalVerts = NULL;
	pTransformedVerts = NULL;

	numVerts = 0;
	numFaces = 0;
	numTVerts = 0;
	
	pVertsWithNormals = NULL;
	pFaces = NULL;
}

dxObj::~dxObj () {

	DELA (pVertsWithNormals);
	DELA (pTransformedVerts);
	DELA (pOriginalVerts);
	DELA (pFaces);
	
	RELEASE (p_VertexBuffer);
	RELEASE (p_IndexBuffer);
}

void dxObj::Create (LPDIRECT3DDEVICE8 d3d_device, int numVerts, int numFaces) {

	if (d3d_device == NULL) {
		trace (_T("Direct3D device pointer is NULL."));
		return;
	}
	
	using_d3d_Device = d3d_device; //pointer to d3d device
	pOriginalVerts = new VERTEX_3DPNT [numVerts];
	pTransformedVerts = new VERTEX_3DPNT [numVerts];

	using_d3d_Device->CreateVertexBuffer (numVerts * sizeof (CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &p_VertexBuffer);
	using_d3d_Device->CreateIndexBuffer (numFaces * 12, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &p_IndexBuffer);

	D3DXMatrixIdentity (&transformM);
	D3DXMatrixIdentity (&rotationM);
	D3DXMatrixIdentity (&textureM);
	D3DXMatrixIdentity (&scaleM);
	D3DXMatrixIdentity (&mainM);
	D3DXMatrixIdentity (&tempM);

	///////////////////////////////////////////////////////////////////////////
	//temporary, rewrite it////////////////////////////////////////////////////
	forup (numVerts) {
		pOriginalVerts[i].position.x = pVertsWithNormals[i*8];
		pOriginalVerts[i].position.y = pVertsWithNormals[i*8+1];
		pOriginalVerts[i].position.z = pVertsWithNormals[i*8+2];

		pOriginalVerts[i].normal.x = pVertsWithNormals[i*8+3];
		pOriginalVerts[i].normal.y = pVertsWithNormals[i*8+4];
		pOriginalVerts[i].normal.z = pVertsWithNormals[i*8+5];

		pOriginalVerts[i].texture.x = pVertsWithNormals[i*8+6];
		pOriginalVerts[i].texture.y = pVertsWithNormals[i*8+7];
	}

	void *tPointer;
	p_IndexBuffer->Lock (0, numFaces * 12, (BYTE**) &tPointer, 0);
	memcpy (tPointer, pFaces, numFaces * 12);
	p_IndexBuffer->Unlock();
	///////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
}

void dxObj::RotateX (float ang) {

	D3DXMatrixRotationX (&tempM, ang);
	rotationM *= tempM;
}

void dxObj::RotateY (float ang) {

	D3DXMatrixRotationY (&tempM, ang);
	rotationM *= tempM;
}

void dxObj::RotateZ (float ang) {

	D3DXMatrixRotationZ (&tempM, ang);
	rotationM *= tempM;
}

void dxObj::Move (float x, float y, float z) {

	D3DXMatrixTranslation (&tempM, x, y, z);
	transformM *= tempM;
}

void dxObj::Scale (float x, float y, float z) {

	D3DXMatrixScaling (&tempM, x, y, z);
	scaleM *= tempM;
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

void dxObj::Render (D3DMATERIAL8 tempMtrl, LPDIRECT3DTEXTURE8 tempTex) {

	using_d3d_Device->SetVertexShader (D3DFVF_CUSTOMVERTEX);
	using_d3d_Device->SetStreamSource (0, p_VertexBuffer, sizeof (CUSTOMVERTEX));
	using_d3d_Device->SetIndices (p_IndexBuffer, 0);

	using_d3d_Device->SetTexture (0, tempTex);
	using_d3d_Device->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	using_d3d_Device->SetTextureStageState (0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	
	using_d3d_Device->DrawIndexedPrimitive (D3DPT_TRIANGLELIST, 0, numVerts, 0, numFaces);
}