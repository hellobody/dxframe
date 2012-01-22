#ifndef DXLANDOBJSMANAGER_H
#define DXLANDOBJSMANAGER_H

#include "dxObj.h"
#include "dxLandObj.h"

class DXFRAME_API dxLandObjsManager {

public:

	static void LoadLandObjs (const TCHAR *pathToLandObjXML, vector <dxLandObj *> &vLandObjs);
	static void SaveLandObjs (const TCHAR *pathToLandObjXML, vector <dxLandObj *> &vLandObjs);
};

#endif
