#include "dxIniFileInterface.h"
#include "dxEnvironmentVars.h"

extern dxEnvironmentVars environmentVars;

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

BOOL dxIniFileInterface::Set (const TCHAR *key, int val)
{
	return true;
}

BOOL dxIniFileInterface::Set (const TCHAR *key, float val)
{
	return true;
}

BOOL dxIniFileInterface::Set (const TCHAR *key, const TCHAR *val)
{
	return WritePrivateProfileString (dxIniSectionName, key, val, environmentVars.iniFilePath);
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

TCHAR *dxIniFileInterface::GetString (const TCHAR *key)
{
	GetPrivateProfileString (dxIniSectionName, key, NULL, buf, MAX_PATH, environmentVars.iniFilePath);

	return buf;
}