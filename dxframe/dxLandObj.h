#ifndef MYCLASS_H
#define MYCLASS_H

#include "dxObj.h"

class DXFRAME_API dxLandObj
{
protected:

	dxObj *body;

public:

	dxLandObj ();
	~dxLandObj ();

	void Create ();
	void Reset ();
	void Update ();
	void Render ();
	void Destroy ();
};

#endif
