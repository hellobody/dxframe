#include "dxIniFileInterface.h"

/*
char *IniRead(char *filename, char *section, char *key){
 char *out = new char[512];
 GetPrivateProfileString(
 (LPCSTR)section,
 (LPCSTR)key,
 NULL,
 out,
 200,
 (LPCSTR)filename
 );
 return out;
}
bool IniWrite(char *filename, char *section, char *key, char *data){
 return WritePrivateProfileString(
 (LPCSTR)section,
 (LPCSTR)key,
 (LPCSTR)data,
 (LPCSTR)filename
 );
}
*/

dxIniFileInterface::dxIniFileInterface ()
{
	
}

dxIniFileInterface::~dxIniFileInterface ()
{
	
}

bool dxIniFileInterface::Set (const TCHAR *key, int val)
{
	return true;
}

bool dxIniFileInterface::Set (const TCHAR *key, float val)
{
	return true;
}

bool dxIniFileInterface::Set (const TCHAR *key, const TCHAR *val)
{
	return true;
}

int dxIniFileInterface::GetInt (const TCHAR *key)
{
	int val = 0;

	return val;
}

float dxIniFileInterface::GetFloat (const TCHAR *key)
{
	float val = 0;

	return val;
}

const TCHAR *dxIniFileInterface::GetString (const TCHAR *key)
{
	TCHAR tStr [MAX_PATH];

	return tStr;
}