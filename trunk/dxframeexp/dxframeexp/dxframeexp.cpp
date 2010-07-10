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

//****************************added by me************************************/
#include <fstream>

using namespace std;

static ofstream pFile;
Interface * ip;

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

	dxObj obj;
	obj.numVerts = TObj->mesh.numVerts;
	obj.numTVerts = TObj->mesh.numTVerts;
	obj.numFaces = TObj->mesh.numFaces;
	
	Matrix3 tm = node->GetObjTMAfterWSM(ip->GetTime());
	
	TObj->mesh.buildNormals();
	
	for (int i = 0; i < TObj->mesh.numVerts; i++)
	{
		Point3 v = tm * TObj->mesh.verts[i];
		obj.vVerts.push_back (v.x);
		obj.vVerts.push_back (v.z);
		obj.vVerts.push_back (v.y);
	}

	pFile.write ((char *) &obj, sizeof (obj));
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

	pFile.open (name, ios::out | ios::binary);

	ei->theScene->EnumTree (&TreeEnum);

	pFile.close ();

	return TRUE;

	#pragma message(TODO("return TRUE If the file is exported properly"))
	return FALSE;
}


