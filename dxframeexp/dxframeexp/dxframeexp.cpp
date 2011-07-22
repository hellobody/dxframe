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
#include "dxTCHARofstrem.h"

using namespace std;

static dxTCHARofstream fout;

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

void SceneSaver::ProcNode(INode *node)
{
	int Del;

	TriObject *TObj;
	TObj = GetTriObjFromNode(node, Del);
	if (!TObj) return;

	//Get and write name
	char *Name = new char [nameSize];
	for (int i=0; i<nameSize; i++) {
		Name [i] = '0';
	}
	strcpy_s (Name, nameSize, node->GetName ());
	fout.write (Name, nameSize);
	//

	fout.write ((char *) &TObj->mesh.numVerts, 4);
	fout.write ((char *) &TObj->mesh.numFaces, 4);
	
	Matrix3 tm = node->GetObjTMAfterWSM(ip->GetTime());
	
	TObj->mesh.buildNormals();

	Point3 v;
	
	for (int i = 0; i < TObj->mesh.numVerts; i++)
	{
		//write vertexes
		v = tm * TObj->mesh.verts[i];

		fout.write ((char *) &v.x, 4);
		fout.write ((char *) &v.y, 4);
		fout.write ((char *) &v.z, 4);

		//write noramals
		v = TObj->mesh.getNormal (i);

		fout.write ((char *) &v.x, 4);
		fout.write ((char *) &v.y, 4);
		fout.write ((char *) &v.z, 4);

		//write texture coordinates
		v = TObj->mesh.getTVert (i);
		fout.write ((char *) &v.x, 4);
		fout.write ((char *) &v.y, 4);
	}

	for (int i = 0; i < TObj->mesh.numFaces; i++)
	{
		fout.write ((char *) &TObj->mesh.faces [i].v [0], 4);
		fout.write ((char *) &TObj->mesh.faces [i].v [1], 4);
		fout.write ((char *) &TObj->mesh.faces [i].v [2], 4);
	}

	//export texture name
	TCHAR TexName [nameSize];
	for (int i=0; i<nameSize; i++) {
		TexName [i] = '0';
	}

	Mtl *m = node->GetMtl ();
	if (!m) {
		MessageBox (NULL, _T("Material is not found"), _T("Warning"), MB_OK);
		fout.write (TexName, nameSize * sizeof (TCHAR));
		return;
	}

	Texmap *tmap = m->GetSubTexmap (ID_DI); //get diffuse subtexture map
	if (!tmap) {
		MessageBox (NULL, _T("Texture is not found"), _T("Warning"), MB_OK);
		fout.write (TexName, nameSize * sizeof (TCHAR));
		return;
	}
  
	if (tmap->ClassID () != Class_ID (BMTEX_CLASS_ID, 0)) {
		MessageBox (NULL, _T("Texture is not a bitmap"), _T("Warning"), MB_OK);
		fout.write (TexName, nameSize * sizeof (TCHAR));
		return;
	}
	
	BitmapTex *bmt = (BitmapTex *) tmap;

	//Get and write texture name
	_tcscpy_s (TexName, nameSize, bmt->GetMapName ());

	fout.write (TexName,  nameSize * sizeof (TCHAR));
	//

	MCHAR
}

static SceneSaver TreeEnum;
//***************************************************************************/

#define dxframeexp_CLASS_ID	Class_ID(0x587a1079, 0xa2e5d156)

class dxframeexp : public SceneExport {
	public:

		static HWND hParams;
		
		int				ExtCount();					// Number of extensions supported
		const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
		const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
		const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
		const TCHAR *	AuthorName();				// ASCII Author name
		const TCHAR *	CopyrightMessage();			// ASCII Copyright message
		const TCHAR *	OtherMessage1();			// Other message #1
		const TCHAR *	OtherMessage2();			// Other message #2
		unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
		void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

		BOOL SupportsOptions(int ext, DWORD options);
		int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

		//Constructor/Destructor
		dxframeexp();
		~dxframeexp();		
};

class dxframeexpClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new dxframeexp(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return SCENE_EXPORT_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return dxframeexp_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("dxframeexp"); }	// returns fixed parsable name (scripter-visible name)
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

const TCHAR *dxframeexp::Ext(int n)
{		
	#pragma message(TODO("Return the 'i-th' file name extension (i.e. \"3DS\")."))
	if (n==0) return _T("dxf");
	return _T("");
}

const TCHAR *dxframeexp::LongDesc()
{
	#pragma message(TODO("Return long ASCII description (i.e. \"Targa 2.0 Image File\")"))
	return _T("dxframe object file");
}
	
const TCHAR *dxframeexp::ShortDesc() 
{			
	#pragma message(TODO("Return short ASCII description (i.e. \"Targa\")"))
	return _T("dxframe");
}

const TCHAR *dxframeexp::AuthorName()
{			
	#pragma message(TODO("Return ASCII Author name"))
	return _T("hellobody");
}

const TCHAR *dxframeexp::CopyrightMessage() 
{	
	#pragma message(TODO("Return ASCII Copyright message"))
	return _T("");
}

const TCHAR *dxframeexp::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *dxframeexp::OtherMessage2() 
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

int	dxframeexp::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	#pragma message(TODO("Implement the actual file Export here and"))

	ip = i;

	fout.open (name, ios::out | ios::binary);

	if (fout.fail ()) {
		MessageBox (NULL, _T("Failed open file"), _T("Error"), MB_OK);
		return FALSE;
	}

	ei->theScene->EnumTree (&TreeEnum);

	fout.close ();

	MessageBox (NULL, _T("Exported"), _T("Message"), MB_OK);

	return TRUE;

	#pragma message(TODO("return TRUE If the file is exported properly"))
	return FALSE;
}