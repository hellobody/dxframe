#ifndef DXLANDOBJ_H
#define DXLANDOBJ_H

#include "dxObj.h"
#include "dxInput.h"

class DXFRAME_API dxLandObj
{
protected:

	dxObj *body;

	static float UpCamOffset;

public:

	dxLandObj ();
	~dxLandObj ();

	void Create (const TCHAR *fName, const char *mName);
	void ReCreate (const TCHAR *fName, const char *mName);
	void Reset ();
	void Update (float dt);
	void Render ();
	void Destroy ();

	void SetPosX (float v);
	void SetPosY (float v);
	void SetPosZ (float v);
	void AddPos (D3DXVECTOR3 v);
	D3DXVECTOR3 GetUpCamPos ();
	D3DXVECTOR3 GetPos ();

	const TCHAR *GetFileName ();
	const TCHAR *GetObjName ();
};

#endif
