#ifndef DXINIFILEINTERFACE
#define DXINIFILEINTERFACE

#include "defines.h"

class dxIniFileInterface
{
public:

	TCHAR buf [MAX_PATH];

	dxIniFileInterface ();
	~dxIniFileInterface ();

	BOOL Set (const TCHAR *key, int val);
	BOOL Set (const TCHAR *key, float val);
	BOOL Set (const TCHAR *key, const TCHAR *val);

	int GetInt (const TCHAR *key);
	float GetFloat (const TCHAR *key);
	TCHAR *GetString (const TCHAR *key);
};

#endif