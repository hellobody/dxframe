#include "dxObj.h"
#include "windows.h"

objMap dxObj::objs;

PDIRECT3DDEVICE8 dxObj::using_d3d_Device = NULL;

dxObj::dxObj () {

	strcpy_s (TexName, nameSize, "");

	p_IndexBuffer = NULL;
	p_VertexBuffer = NULL;

	pOriginalVerts = NULL;
	pTransformedVerts = NULL;

	numVerts = 0;
	numFaces = 0;
	
	pVertsWithNormals = NULL;
	pFaces = NULL;

	texture = NULL;
}

dxObj::~dxObj () {

	InternalDestroy ();
}

void dxObj::Create (LPDIRECT3DDEVICE8 d3d_device, int numVerts, int numFaces) {	//depracated, prepare to delete

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

bool dxObj::CreateFromFile (const TCHAR *flName, const char *objName) {

	InternalDestroy ();
	
	if (using_d3d_Device == NULL) {
		trace (_T("Direct3D device pointer is NULL."));
		return false;
	}

	TCHAR flPath [MAX_PATH];
	_tcscpy_s (flPath, MAX_PATH, _T("data\\"));
	_tcscat_s (flPath, MAX_PATH, flName);

	//open file with .dxf models
	ifstream fin;

	fin.open (flPath, ios::in | ios::binary);

	if (!fin.fail ()) {
		
		do {

			DELA (pVertsWithNormals);
			DELA (pFaces);

			fin.read ((char *) &Name, nameSize);
			fin.read ((char *) &numVerts, 4);
			fin.read ((char *) &numFaces, 4);

			pVertsWithNormals = new float [numVerts * (3 * 2 + 2)]; //because verts with normals + texture coord

			forup (numVerts * (3 * 2 + 2)) {
				fin.read ((char *) &pVertsWithNormals [i], 4);
			}

			pFaces = new int [numFaces * 3];

			forup (numFaces * 3) {
				fin.read ((char *) &pFaces [i], 4);
			}

			fin.read ((char *) &TexName, nameSize);
			
			char endByte;
			fin.read ((char *) &endByte, 1);

			if (!fin.eof ()) {
				fin.seekg (int (fin.tellg ()) - 1);
			}
		} while (!fin.eof () && strcmp (Name, objName) != 0);

		fin.close ();

		//
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
		//

		//load textures
		char tStr [MAX_PATH] = "";
		strcat_s (tStr, PATHTO_DATA_A);
		strcat_s (tStr, TexName);
		if (S_OK != D3DXCreateTextureFromFileA (using_d3d_Device, tStr, &texture))
		{
			trace (_T("Error create texture."));
		}
		//

		objs.insert (objPair (Name, this)); //push new model to map
	} else {

		trace (_T("File not found."));	//add here file path
		return false;
	}

	return true;
}

void dxObj::InternalDestroy () {

	DELA (pVertsWithNormals);
	DELA (pTransformedVerts);
	DELA (pOriginalVerts);
	DELA (pFaces);
	
	RELEASE (p_VertexBuffer);
	RELEASE (p_IndexBuffer);

	RELEASE (texture);
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

void dxObj::Render () {

	using_d3d_Device->SetVertexShader (D3DFVF_CUSTOMVERTEX);
	using_d3d_Device->SetStreamSource (0, p_VertexBuffer, sizeof (CUSTOMVERTEX));
	using_d3d_Device->SetIndices (p_IndexBuffer, 0);

	using_d3d_Device->SetTexture (0, texture);
	using_d3d_Device->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	using_d3d_Device->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	
	using_d3d_Device->DrawIndexedPrimitive (D3DPT_TRIANGLELIST, 0, numVerts, 0, numFaces);
}