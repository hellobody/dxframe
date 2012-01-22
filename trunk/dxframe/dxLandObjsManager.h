#ifndef DXLANDOBJSMANAGER_H
#define DXLANDOBJSMANAGER_H

#include "dxObj.h"
#include "dxLandObj.h"

class DXFRAME_API dxLandObjTypeInfo {

public:
	TCHAR fName [MAX_PATH];
	char oName [MAX_PATH];
};

class DXFRAME_API dxLandObjsManager {

public:

	static void LoadLandObjs (const TCHAR *pathToLandObjXML, vector <dxLandObj *> &vLandObjs);
	static void SaveLandObjs (const TCHAR *pathToLandObjXML, vector <dxLandObj *> &vLandObjs);

	static void LoadAvailableObjTypes (const TCHAR *pathToLandObjsTypeXML, vector <dxLandObjTypeInfo> &vLandObjTypes);
};

#endif
