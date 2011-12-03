#ifndef DXENVIRONMENTVARS
#define DXENVIRONMENTVARS

#include "defines.h"

//extern LPCWSTR APPNAME;

class dxEnvironmentVars
{

public:

	TCHAR osName [MAX_PATH];
	TCHAR compName [MAX_PATH];
	TCHAR userName [MAX_PATH];

	TCHAR appDataPath [MAX_PATH];
	TCHAR curPath [MAX_PATH];

	TCHAR iniFilePath [MAX_PATH];

	dxEnvironmentVars () {

		size_t retVal;

		_tgetenv_s (&retVal, osName, MAX_PATH, _T("OS"));
		_tgetenv_s (&retVal, compName, MAX_PATH, _T("COMPUTERNAME"));
		_tgetenv_s (&retVal, userName, MAX_PATH, _T("USERNAME"));
		_tgetenv_s (&retVal, appDataPath, MAX_PATH, _T("APPDATA"));

		_tcscat_s (appDataPath, MAX_PATH, _T("\\"));
		_tcscat_s (appDataPath, MAX_PATH, _T("AppName"));

		GetModuleFileName (NULL, curPath, MAX_PATH);

		int strLen = (int) _tcslen (curPath);
		forup (strLen) {
			TCHAR c = curPath [strLen - 1 - i];
			if (c != '\\') {
				curPath [strLen - 1 - i] = 0;
			} else break;
		}

		if (DebugCompilerMode) {
			_tcscpy_s (iniFilePath, MAX_PATH, curPath);
			_tcscat_s (iniFilePath, MAX_PATH, dxIniFileName);
		} else {
			_tcscpy_s (iniFilePath, MAX_PATH, appDataPath);
			_tcscat_s (iniFilePath, MAX_PATH, _T("\\"));
			_tcscat_s (iniFilePath, MAX_PATH, dxIniFileName);
		}
	}
};

#endif