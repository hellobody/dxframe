//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "dxframeexp.h"
#include "stdmat.h"
#include "tchar.h"

//****************************added by me************************************/
#include <fstream>

using namespace std;

static ofstream fout;

Interface * ip;

const int nameSize = 256;

TriObject *GetTriObjFromNode(INode *node, int &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(ip->GetTime()).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID,0)))
	{
		TriObject *tri = (TriObject *) obj->ConvertToType(ip->GetTime(), 
			Class_ID(TRIOBJ_CLASS_ID, 0));
		if (obj != tri) deleteIt = TRUE;
		return tri;
	}
	else return NULL;
}

class SceneSaver: public ITreeEnumProc
{
public:
	// SubExporters
	void ExportFaces(TriObject *TObj);
	void ExportVerts(TriObject *TObj, Matrix3 tm);
	void ExportMaterial(INode *node);

	// Main functions
	int callback(INode *node);
	void ProcNode(INode *node);
};

int SceneSaver::callback(INode *node)
{
	ProcNode(node);
	return TREE_CONTINUE;
}

void getFileNameFromFullPath (const char *fullPath, char *fileName) {

	char s0 [nameSize];
	char s1 [nameSize] = "";
	strcpy_s (s0, nameSize, fullPath);
	strcpy_s (s0, nameSize, _strrev (s0));

	int i = 0;
	while (s0 [i] != '\\') {
		s1 [i] = s0 [i];
		i++;
	}
	strcpy_s (fileName, nameSize, _strrev (s1));
}

void SceneSaver::ProcNode(INode *node)
{
	int Del;

	TriObject *TObj = GetTriObjFromNode (node, Del);
	
	if (!TObj) return;
	
	Point3 v;

	//temp test
	//node->Ge
	//

	//Get and write name
	char Name [nameSize] = "";
	strcpy_s (Name, nameSize, node->GetName ());
	fout.write (Name, nameSize);
	//

	Matrix3 tm = node->GetObjTMAfterWSM (ip->GetTime ());	//seems like got transformation matrix here


	/////////////////////////////////////////////////////////////////
	int numVerts = TObj->mesh.numVerts;				//кол-во вершин
	int numTVerts = TObj->mesh.numTVerts;			//кол-во текстурных вершин
	int numFaces = TObj->mesh.numFaces;				//кол-во полигонов (треугольников (фейсов (граней))) == кол-во текстурных полигонов

	if (numTVerts == 0) {
		//MessageBox (NULL, _T("There is no texture co-ordinates. \n Try to generate texture co-ordinates. \n (Use UVW Map modifier for example.)"), _T("Error"), MB_OK);
		return;
	}

	float *pVerts = new float [numVerts * 3];		//массив с вершинами
	float *pNormals = new float [numVerts * 3];		//массив с нормалями
	float *pTVerts = new float [numTVerts * 2];		//массив с текстурными вершинами
	int *pFaces = new int [numFaces * 3];			//массив с индексами
	int *pTFaces = new int [numFaces * 3];			//массив с текстурными индексами

	fout.write ((char *) &numTVerts, 4);			//новое кол-во вершин записываем в файл
	fout.write ((char *) &numFaces, 4);

	TCHAR tMess [MAX_PATH] = _T("");
	TCHAR tStr [MAX_PATH] = _T("");

	_stprintf_s (tMess, _T("Model name: %s \n\n counts: \n vertexes: %i \n texture vertexes: %i \n faces: %i"), Name, numVerts, numTVerts, numFaces);
	MessageBox (NULL, tMess, _T("Info"), MB_OK);

	_tcscpy_s (tMess, MAX_PATH, _T("vertexes:\n"));
	for (int i = 0; i < numVerts; i++) {

		v = tm * TObj->mesh.getVert (i);

		pVerts [i * 3]	   = v.x;
		pVerts [i * 3 + 1] = v.y;
		pVerts [i * 3 + 2] = v.z;

		/*_stprintf_s (tStr, _T(" %f,"), pVerts [i * 3]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_stprintf_s (tStr, _T(" %f,"), pVerts [i * 3 + 1]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_stprintf_s (tStr, _T(" %f,"), pVerts [i * 3 + 2]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_tcscat_s (tMess, MAX_PATH, _T("\n"));*/
	}
	//MessageBox (NULL, tMess, _T("Info"), MB_OK);

	TObj->mesh.buildNormals ();

	_tcscpy_s (tMess, MAX_PATH, _T("normals:\n"));
	for (int i = 0; i < numVerts; i++) {

		pNormals [i * 3]	 = TObj->mesh.getNormal (i).x;
		pNormals [i * 3 + 1] = TObj->mesh.getNormal (i).y;
		pNormals [i * 3 + 2] = TObj->mesh.getNormal (i).z;

		/*_stprintf_s (tStr, _T(" %f,"), pNormals [i * 3]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_stprintf_s (tStr, _T(" %f,"), pNormals [i * 3 + 1]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_stprintf_s (tStr, _T(" %f,"), pNormals [i * 3 + 2]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_tcscat_s (tMess, MAX_PATH, _T("\n"));*/
	}
	//MessageBox (NULL, tMess, _T("Info"), MB_OK);

	_tcscpy_s (tMess, MAX_PATH, _T("faces:\n"));
	for (int i = 0; i < numFaces; i++) {

		pFaces [i * 3]	   = TObj->mesh.faces [i].v [0];
		pFaces [i * 3 + 1] = TObj->mesh.faces [i].v [1];
		pFaces [i * 3 + 2] = TObj->mesh.faces [i].v [2];

		/*_stprintf_s (tStr, _T(" %i,"), pFaces [i * 3]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_stprintf_s (tStr, _T(" %i,"), pFaces [i * 3 + 1]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_stprintf_s (tStr, _T(" %i,"), pFaces [i * 3 + 2]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_tcscat_s (tMess, MAX_PATH, _T("\n"));*/
	}
	//MessageBox (NULL, tMess, _T("Info"), MB_OK);

	_tcscpy_s (tMess, MAX_PATH, _T("texture faces:\n"));
	for (int i = 0; i < numFaces; i++) {

		pTFaces [i * 3]	    = TObj->mesh.tvFace [i].t [0];
		pTFaces [i * 3 + 1] = TObj->mesh.tvFace [i].t [1];
		pTFaces [i * 3 + 2] = TObj->mesh.tvFace [i].t [2];

		/*_stprintf_s (tStr, _T(" %i,"), pTFaces [i * 3]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_stprintf_s (tStr, _T(" %i,"), pTFaces [i * 3 + 1]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_stprintf_s (tStr, _T(" %i,"), pTFaces [i * 3 + 2]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_tcscat_s (tMess, MAX_PATH, _T("\n"));*/
	}
	//MessageBox (NULL, tMess, _T("Info"), MB_OK);

	/*for (int i = 0; i < numTVerts; i++) {

		pTVerts [i * 2]	    = TObj->mesh.getTVert (i).x;
		pTVerts [i * 2 + 1] = TObj->mesh.getTVert (i).y;
	}*/

	float *pRecomputedVerts = new float [numTVerts * 3];
	float *pRecomputedNormals = new float [numTVerts * 3];

	for (int i=0; i<numFaces * 3; i++) {
		
		pRecomputedVerts [pTFaces [i] * 3]	   = pVerts [pFaces [i] * 3];
		pRecomputedVerts [pTFaces [i] * 3 + 1] = pVerts [pFaces [i] * 3 + 1];
		pRecomputedVerts [pTFaces [i] * 3 + 2] = pVerts [pFaces [i] * 3 + 2];
	}

	//_tcscpy_s (tMess, MAX_PATH, _T("recomputed vertexes:\n"));
	for (int i=0; i<numFaces * 3; i++) {

		/*_stprintf_s (tStr, _T(" %f,"), pRecomputedVerts [i * 3]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_stprintf_s (tStr, _T(" %f,"), pRecomputedVerts [i * 3 + 1]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_stprintf_s (tStr, _T(" %f,"), pRecomputedVerts [i * 3 + 2]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_tcscat_s (tMess, MAX_PATH, _T("\n"));*/
	}
	//MessageBox (NULL, tMess, _T("Info"), MB_OK);

	for (int i=0; i<numFaces * 3; i++) {

		pRecomputedNormals [pTFaces [i] * 3]	 = pNormals [pFaces [i] * 3];
		pRecomputedNormals [pTFaces [i] * 3 + 1] = pNormals [pFaces [i] * 3 + 1];
		pRecomputedNormals [pTFaces [i] * 3 + 2] = pNormals [pFaces [i] * 3 + 2];
	}

	//_tcscpy_s (tMess, MAX_PATH, _T("recomputed normals:\n"));
	for (int i=0; i<numFaces * 3; i++) {

		/*_stprintf_s (tStr, _T(" %f,"), pRecomputedNormals [i * 3]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_stprintf_s (tStr, _T(" %f,"), pRecomputedNormals [i * 3 + 1]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_stprintf_s (tStr, _T(" %f,"), pRecomputedNormals [i * 3 + 2]);
		_tcscat_s (tMess, MAX_PATH, tStr);
		_tcscat_s (tMess, MAX_PATH, _T("\n"));*/
	}
	//MessageBox (NULL, tMess, _T("Info"), MB_OK);
	
	for (int i = 0; i < numTVerts; i++)
	{
		//write vertexes
		fout.write ((char *) &pRecomputedVerts [i * 3],		4);
		fout.write ((char *) &pRecomputedVerts [i * 3 + 1], 4);
		fout.write ((char *) &pRecomputedVerts [i * 3 + 2], 4);

		//write normals
		fout.write ((char *) &pRecomputedNormals [i * 3],     4);
		fout.write ((char *) &pRecomputedNormals [i * 3 + 1], 4);
		fout.write ((char *) &pRecomputedNormals [i * 3 + 2], 4);

		//write texture coordinates
		v = TObj->mesh.tVerts [i];	//читаю только UV для 2d текстурирования
		v.y *= -1; //try to flip texture 
		fout.write ((char *) &v.x, 4);
		fout.write ((char *) &v.y, 4);
	}
	
	for (int i = 0; i < numFaces; i++)
	{
		fout.write ((char *) &pTFaces [i * 3],	   4);
		fout.write ((char *) &pTFaces [i * 3 + 1], 4);
		fout.write ((char *) &pTFaces [i * 3 + 2], 4);
	}

	//export texture name
	char TexName [nameSize] = "";
	char TexShortName [nameSize] = "";

	Mtl *m = node->GetMtl ();
	if (!m) {
		//MessageBox (NULL, _T("Material is not found"), _T("Warning"), MB_OK);
		fout.write (TexName, nameSize * sizeof (char));
		return;
	}

	Texmap *tmap = m->GetSubTexmap (ID_DI); //get diffuse subtexture map
	if (!tmap) {
		//MessageBox (NULL, _T("Texture is not found"), _T("Warning"), MB_OK);
		fout.write (TexName, nameSize * sizeof (char));
		return;
	}
  
	if (tmap->ClassID () != Class_ID (BMTEX_CLASS_ID, 0)) {
		//MessageBox (NULL, _T("Texture is not a bitmap"), _T("Warning"), MB_OK);
		fout.write (TexName, nameSize * sizeof (char));
		return;
	}
	
	BitmapTex *bmt = (BitmapTex *) tmap;

	//Get and write texture name
	_tcscpy_s (TexName, nameSize, bmt->GetMapName ());
	
	getFileNameFromFullPath (TexName, TexShortName);

	fout.write (TexShortName,  nameSize * sizeof (char));
	//
}

static SceneSaver TreeEnum;
//***************************************************************************/

#define dxframeexp_CLASS_ID	Class_ID(0x587a1079, 0xa2e5d156)

class dxframeexp : public SceneExport {
	public:

		static HWND hParams;
		
		int				ExtCount();					// Number of extensions supported
		const char *	Ext(int n);					// Extension #n (i.e. "3DS")
		const char *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
		const char *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
		const char *	AuthorName();				// ASCII Author name
		const char *	CopyrightMessage();			// ASCII Copyright message
		const char *	OtherMessage1();			// Other message #1
		const char *	OtherMessage2();			// Other message #2
		unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
		void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

		BOOL SupportsOptions(int ext, DWORD options);
		int				DoExport(const char *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

		//Constructor/Destructor
		dxframeexp();
		~dxframeexp();		
};

class dxframeexpClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new dxframeexp(); }
	virtual const char *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return dxframeexp_CLASS_ID; }
	virtual const char* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const char* InternalName() 			{ return _T("dxframeexp"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
};

static dxframeexpClassDesc dxframeexpDesc;
ClassDesc2* GetdxframeexpDesc() { return &dxframeexpDesc; }

INT_PTR CALLBACK dxframeexpOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static dxframeexp *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (dxframeexp *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}


//--- dxframeexp -------------------------------------------------------
dxframeexp::dxframeexp()
{

}

dxframeexp::~dxframeexp() 
{

}

int dxframeexp::ExtCount()
{
	#pragma message(TODO("Returns the number of file name extensions supported by the plug-in."))
	return 1;
}

const char *dxframeexp::Ext(int n)
{		
	#pragma message(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	if (n==0) return _T("dxf");
	return _T("");
}

const char *dxframeexp::LongDesc()
{
	#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
	return _T("dxframe object file");
}
	
const char *dxframeexp::ShortDesc() 
{			
	#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("dxframe");
}

const char *dxframeexp::AuthorName()
{			
	#pragma message(TODO("Return ASCII Author name"))
	return _T("hellobody");
}

const char *dxframeexp::CopyrightMessage() 
{	
	#pragma message(TODO("Return ASCII Copyright message"))
	return _T("");
}

const char *dxframeexp::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const char *dxframeexp::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int dxframeexp::Version()
{				
	#pragma message(TODO("Return Version number * 100 (i.e. v3.01 = 301)"))
	return 100;
}

void dxframeexp::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL dxframeexp::SupportsOptions(int ext, DWORD options)
{
	#pragma message(TODO("Decide which options to support.  Simply return true for each option supported by each Extension the exporter supports."))
	return TRUE;
}

int	dxframeexp::DoExport(const char *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	#pragma message(TODO("Implement the actual file Export here and"))

	ip = i;

	fout.open (name, ios::out | ios::binary);

	if (fout.fail ()) {
		//MessageBox (NULL, _T("Failed open file"), _T("Error"), MB_OK);
		return FALSE;
	}

	ei->theScene->EnumTree (&TreeEnum);

	fout.close ();

	//MessageBox (NULL, _T("Exported"), _T("Message"), MB_OK);

	return TRUE;

	#pragma message(TODO("return TRUE If the file is exported properly"))
	return FALSE;
}