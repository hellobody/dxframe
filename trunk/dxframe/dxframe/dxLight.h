#ifndef DXLIGHT_H
#define DXLIGHT_H

#include "defines.h"

class dxLight
{
protected:

	D3DLIGHT8 light;

public:

	dxLight ();
	~dxLight ();
};

#endif