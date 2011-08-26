#ifndef DXINIFILEINTERFACE
#define DXINIFILEINTERFACE

#include "defines.h"

class dxIniFileInterface
{
public:

	dxIniFileInterface ();
	~dxIniFileInterface ();

	bool Set (const TCHAR *key, int val);
	bool Set (const TCHAR *key, float val);
	bool Set (const TCHAR *key, const TCHAR *val);

	int GetInt (const TCHAR *key);
	float GetFloat (const TCHAR *key);
	const TCHAR *GetString (const TCHAR *key);
};

#endif