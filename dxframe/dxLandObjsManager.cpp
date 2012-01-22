#include "stdafx.h"
#include "dxLandObjsManager.h"

void dxLandObjsManager::LoadLandObjs (const TCHAR *pathToLandObjXML, vector <dxLandObj *> &vLandObjs) {
	xml_document doc;

	xml_parse_result result = doc.load_file (pathToLandObjXML);

	if (result) {

		xml_node tagRoot = doc.child (_T("LandObjs"));

		for (xml_node tag = tagRoot.child (_T("LandObj")); tag; tag = tag.next_sibling (_T("LandObj")))
		{
			TCHAR *file = (TCHAR *) tag.attribute (_T("file")).value ();
			TCHAR *model = (TCHAR *) tag.attribute (_T("model")).value ();

			TCHAR tStr [MAX_PATH];
			char chModel [MAX_PATH];
			ZeroMemory (&chModel, MAX_PATH);
			_tcscpy_s (tStr, MAX_PATH, model);
			forup (MAX_PATH) {
				chModel [i] = (char) tStr [i];
			}

			float X = tag.attribute (_T("posOnMapX")).as_float ();
			float Y = tag.attribute (_T("posOnMapY")).as_float ();

			dxLandObj *tLandObj = new dxLandObj;
			tLandObj->Create (file, chModel);
			tLandObj->SetPosX (X);
			tLandObj->SetPosY (Y);
			vLandObjs.push_back (tLandObj);
		}
	}
}

void dxLandObjsManager::SaveLandObjs (const TCHAR *pathToLandObjXML, vector <dxLandObj *> &vLandObjs) {
	xml_document doc;
	
	xml_node tagRoot = doc.append_child (_T("LandObjs"));

	forup ((int) vLandObjs.size ()) {
		if (vLandObjs [i]) {
			xml_node landObj = tagRoot.append_child (_T("LandObj"));
			landObj.append_attribute (_T("file")) = vLandObjs [i]->GetFileName ();
			landObj.append_attribute (_T("model")) = vLandObjs [i]->GetObjName ();
			landObj.append_attribute (_T("posOnMapX")) = vLandObjs [i]->GetPos ().x;
			landObj.append_attribute (_T("posOnMapY")) = vLandObjs [i]->GetPos ().y;
		}
	}

	doc.save_file (pathToLandObjXML);
}