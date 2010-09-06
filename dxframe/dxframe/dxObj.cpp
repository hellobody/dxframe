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

void dxObj::Transform ()
{
	//memcpy (pTransformedVerts, pOriginalVerts, numVerts);

	mainM = rotationM * transformM * scaleM;

	forup (numVerts)
	{
		
	}
}

//csc3Dobj::Transform (void)
//{
//	memcpy (p_Vertices, p_ObjVert, numVerts);
//
//	objMatr = objMatrS*objMatrR*objMatrT;
//
//	for(DWORD i=0; i<dwNumVerticies; i++)
//	{
//		D3DXVec3TransformCoord (&p_Vertices[i].position,
//			&p_Vertices[i].position, &objMatr);
//		D3DXVec3TransformNormal (&p_Vertices[i].normal, 
//			&p_Vertices[i].normal, &objMatr);
//		D3DXVec3Normalize (&p_Vertices[i].normal, &p_Vertices[i].normal);
//	};
//	p_VertexBuffer->Lock (0, sizeOfAllVerteces, (BYTE**)&t_pVertices, 0);
//	memcpy (t_pVertices, p_Vertices, sizeOfAllVerteces);
//	p_VertexBuffer->Unlock();
//	return 0;
//};