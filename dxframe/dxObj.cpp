#include "stdafx.h"
#include "dxObj.h"
#include "windows.h"

DXFRAME_API vector <dxObj *> dxObj::objs;

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

	opacity = 1.f;

	ZeroMemory (&material, sizeof(D3DMATERIAL8));
	material.Diffuse.r = material.Ambient.r = 1.0f;
	material.Diffuse.g = material.Ambient.g = 1.0f;
	material.Diffuse.b = material.Ambient.b = 1.0f;
	material.Diffuse.a = material.Ambient.a = opacity;

	////shader
	/*vertexDecl = NULL;
	constantTable = NULL;
	vertexShader = NULL;
	pixelShader = NULL;
	code = NULL;
	ShaderOn = false;*/
	////
}

dxObj::dxObj (const TCHAR *flName, const char *objName) {

	dxObj tObj;
	*this = tObj;

	CreateFromFile (flName, objName);
}

dxObj::~dxObj () {

	InternalDestroy ();
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

		//using_d3d_Device->CreateVertexBuffer (numVerts * sizeof (CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &p_VertexBuffer, NULL);	//ver 9
		//using_d3d_Device->CreateIndexBuffer (numFaces * 12, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &p_IndexBuffer, NULL);	//ver 9

		using_d3d_Device->CreateVertexBuffer (numVerts * sizeof (CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &p_VertexBuffer);
		using_d3d_Device->CreateIndexBuffer (numFaces * 12, 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &p_IndexBuffer);

		D3DXMatrixIdentity (&transformM);
		D3DXMatrixIdentity (&rotationXM);
		D3DXMatrixIdentity (&rotationYM);
		D3DXMatrixIdentity (&rotationZM);
		D3DXMatrixIdentity (&textureM);
		D3DXMatrixIdentity (&scaleM);
		D3DXMatrixIdentity (&mainM);
		D3DXMatrixIdentity (&tempM);

		///////////////////////////////////////////////////////////////////////////
		//temporary, rewrite it////////////////////////////////////////////////////
		forup (numVerts) {

			pOriginalVerts [i].position.x = pVertsWithNormals [i*8];
			pOriginalVerts [i].position.y = pVertsWithNormals [i*8+1];
			pOriginalVerts [i].position.z = pVertsWithNormals [i*8+2];

			pOriginalVerts [i].normal.x = pVertsWithNormals [i*8+3];
			pOriginalVerts [i].normal.y = pVertsWithNormals [i*8+4];
			pOriginalVerts [i].normal.z = pVertsWithNormals [i*8+5];

			pOriginalVerts [i].texture.x = pVertsWithNormals [i*8+6];
			pOriginalVerts [i].texture.y = pVertsWithNormals [i*8+7];
		}

		void *tPointer;
		//p_IndexBuffer->Lock (0, numFaces * 12, (void **) &tPointer, 0);	//ver 9
		p_IndexBuffer->Lock (0, numFaces * 12, (BYTE **) &tPointer, 0);
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

		objs.push_back (this);
	} else {

		trace (_T("File not found."));	//add here file path
		return false;
	}

	////shader
	// set up Pixel Shader (NEW)

	//D3DXGetPixelShaderProfile (using_d3d_Device);	//where is it!?

	//LPD3DXBUFFER pErrorMsgs = NULL;

	//HRESULT result;

	//D3DVERTEXELEMENT9 decl [] = {{0,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	//							 {0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
	//							 {0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	//																							D3DDECL_END ()};

	//result = using_d3d_Device->CreateVertexDeclaration (decl, &vertexDecl);

	//result = D3DXCompileShaderFromFile (	_T("vertex.vsh"),	//filepath
	//										NULL,            	//macro's
	//										NULL,            	//includes
	//										"vs_main",       	//main function
	//										"vs_1_1",        	//shader profile
	//										0,               	//flags
	//										&code,           	//compiled operations
	//										&pErrorMsgs,        //errors
	//										&constantTable); 	//constants

	//result = using_d3d_Device->CreateVertexShader ((DWORD *) code->GetBufferPointer (), &vertexShader);
	//
	//if (code) code->Release ();

	//if ((FAILED(result)) && (pErrorMsgs != NULL))
	//{
	//	char * message = (char *) pErrorMsgs->GetBufferPointer();
	//	trace (message);
	//	return false;
	//}
	//
	//result = D3DXCompileShaderFromFile (	_T("pixel.psh"),	//filepath
	//										NULL,				//macro's            
	//										NULL,				//includes           
	//										"main",				//main function
	//										"ps_2_0",      		//shader profile
	//										D3DXSHADER_DEBUG ,  //flags
	//										&code,         		//compiled operations
	//										&pErrorMsgs,   		//errors
	//										NULL	);     		//constants

	//if ((FAILED(result)) && (pErrorMsgs != NULL))
	//{
	//	char * message = (char *) pErrorMsgs->GetBufferPointer();
	//	trace (message);
	//	return false;
	//}

	//result = using_d3d_Device->CreatePixelShader ((DWORD *) code->GetBufferPointer (), &pixelShader);
	//if (code) code->Release();

	////

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

void dxObj::Temp_TurnOnShader ()
{
	//ShaderOn = true;
}

void dxObj::RotateX (float ang) {

	D3DXMatrixRotationX (&tempM, ang);
	rotationXM *= tempM;
}

void dxObj::RotateY (float ang) {

	D3DXMatrixRotationY (&tempM, ang);
	rotationYM *= tempM;
}

void dxObj::RotateZ (float ang) {

	D3DXMatrixRotationZ (&tempM, ang);
	rotationZM *= tempM;
}

void dxObj::Move (float x, float y, float z) {

	D3DXMatrixTranslation (&tempM, x, y, z);
	transformM *= tempM;
}

void dxObj::Move (D3DXVECTOR3 v) {

	D3DXMatrixTranslation (&tempM, v.x, v.y, v.z);
	transformM *= tempM;
}

void dxObj::MoveTo (D3DXVECTOR2 v) {

	transformM._41 = v.x;
	transformM._42 = v.y;
}

void dxObj::MoveTo (D3DXVECTOR3 v) {

	transformM._41 = v.x;
	transformM._42 = v.y;
	transformM._43 = v.z;
}

void dxObj::Scale (float x, float y, float z) {

	D3DXMatrixScaling (&tempM, x, y, z);
	scaleM *= tempM;
}

void dxObj::SetScale (float x, float y, float z) {
	
	D3DXMatrixScaling (&tempM, x, y, z);
	scaleM = tempM;
}

void dxObj::SetScale (float s) {

	D3DXMatrixScaling (&tempM, s, s, s);
	scaleM = tempM;
}

void dxObj::SetDirZ (D3DXVECTOR2 v) {

	SetAngZ (atan2 (v.y, v.x));
}

void dxObj::SetAngX (float a) {

	D3DXMatrixRotationX (&tempM, a);
	rotationXM = tempM;
}

void dxObj::SetAngY (float a) {

	D3DXMatrixRotationY (&tempM, a);
	rotationYM = tempM;
}

void dxObj::SetAngZ (float a) {

	D3DXMatrixRotationZ (&tempM, a);
	rotationZM = tempM;
}

D3DXVECTOR2 dxObj::GetPos2 () {

	return D3DXVECTOR2 (transformM._41, transformM._42);
}
D3DXVECTOR3 dxObj::GetPos3 () {

	return D3DXVECTOR3 (transformM._41, transformM._42, transformM._43);
}

void dxObj::SetOpacity (float v) {

	opacity = v;

	holdFloatValueFromZeroToOne (opacity);
}

float dxObj::GetOpacity () {

	return opacity;
}

void dxObj::Transform () {

	memcpy (pTransformedVerts, pOriginalVerts, numVerts * sizeof (CUSTOMVERTEX)); //copy original coordinates to buffer for transformation

	mainM = scaleM * rotationXM * rotationYM * rotationZM * transformM;			//computing eventual outcome matrix

	forup (numVerts) {

		D3DXVec3TransformCoord (&pTransformedVerts[i].position, &pTransformedVerts[i].position, &mainM); //transforming positions by eventual outcome matrix 
		D3DXVec3TransformNormal (&pTransformedVerts[i].normal, &pTransformedVerts[i].normal, &mainM); //transforming normals
		D3DXVec3Normalize (&pTransformedVerts[i].normal, &pTransformedVerts[i].normal);				//normalizing normals
	}

	void *tPointer;
	//p_VertexBuffer->Lock (0, numVerts * sizeof (CUSTOMVERTEX), (void**) &tPointer, 0);	//ver 9
	p_VertexBuffer->Lock (0, numVerts * sizeof (CUSTOMVERTEX), (BYTE **) &tPointer, 0);
	memcpy (tPointer, pTransformedVerts, numVerts * sizeof (CUSTOMVERTEX));
	p_VertexBuffer->Unlock ();

	//These are useful for increasing speed. e.g. if you do not want to keep existing data in the buffer but just want to overwrite it 
		//you can specify D3DLOCK_DISCARD, this can cause a speed increase
}

void dxObj::Render () {

	HRESULT hRes;

	//hRes = using_d3d_Device->SetPixelShader (g_lpPixelShader);

	//don't working fog
	/*hRes = using_d3d_Device->SetRenderState (D3DRS_FOGCOLOR, 0x00ffffff);
	hRes = using_d3d_Device->SetRenderState (D3DRS_FOGENABLE, TRUE);*/
	//
	
	//if (ShaderOn)
	//{
	//	//D3DXMATRIXA16 matWorldViewProj = matWorld * matView * matProj;
	//	//constantTable->SetMatrix (app.getDevice(), "WorldViewProj", &matWorldViewProj);

	//	hRes = using_d3d_Device->SetVertexShader (vertexShader);
	//	hRes = using_d3d_Device->SetPixelShader (pixelShader);
	//}
	//else
	//{
	//	
	//}

	//hRes = using_d3d_Device->SetFVF (D3DFVF_CUSTOMVERTEX);	//ver 9

	//hRes = using_d3d_Device->SetStreamSource (0, p_VertexBuffer, 0, sizeof (CUSTOMVERTEX));	//ver 9
	//hRes = using_d3d_Device->SetIndices (p_IndexBuffer); //ver 9

	hRes = using_d3d_Device->SetStreamSource (0, p_VertexBuffer, sizeof (CUSTOMVERTEX));
	hRes = using_d3d_Device->SetIndices (p_IndexBuffer, 0);

	material.Diffuse.a = material.Ambient.a = opacity;
	
	hRes = using_d3d_Device->SetMaterial (&material);
	hRes = using_d3d_Device->SetTexture (0, texture);
	
	//hRes = using_d3d_Device->DrawIndexedPrimitive (D3DPT_TRIANGLELIST, 0, 0, numVerts, 0, numFaces);	//ver 9
	hRes = using_d3d_Device->DrawIndexedPrimitive (D3DPT_TRIANGLELIST, 0, numVerts, 0, numFaces);

	//hRes = using_d3d_Device->SetVertexShader (NULL);
	//hRes = using_d3d_Device->SetPixelShader (NULL);
	
	//Эффективней использовать D3DPT_TRIANGLESTRIP или D3DPT_TRIANGLEFAN, чем D3DPT_TRIANGLELIST, т.к. в данном случае не происходит дублирование вершин.
}

bool dxObj::IsPick (LONG x, LONG y)
{
	D3DXVECTOR3 PickPos;
	D3DXVECTOR3 PickDir;
	
	PickPos.x = (float) x;
	PickPos.y = (float) y;
	PickPos.z = 1000;

	PickDir.x = 0;
	PickDir.y = 0;
	PickDir.z = -1;

	FLOAT a, b, c;

	for (int i=0; i<numFaces; i++)
	{
		if (D3DXIntersectTri (	&pTransformedVerts [pFaces [i * 3]].position,
								&pTransformedVerts [pFaces [i * 3 + 1]].position,
								&pTransformedVerts [pFaces [i * 3 + 2]].position,
								&PickPos,
								&PickDir,
								&a, &b, &c)) {
			return true;
		}
	}
	return false;
}