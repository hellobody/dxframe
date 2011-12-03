#include "stdafx.h"
#include "dxLogger.h"
#include "dxEnvironmentVars.h"

dxLogger::dxLogger () {
	
	_tcscpy_s (osName, MAX_PATH, _T(""));
	_tcscpy_s (compName, MAX_PATH, _T(""));
	_tcscpy_s (userName, MAX_PATH, _T(""));
	_tcscpy_s (appDataPath, MAX_PATH, _T(""));
}

dxLogger::~dxLogger () {
	
}

void dxLogger::getSystemInfo () {

	size_t retVal;

	_tgetenv_s (&retVal, osName, MAX_PATH, _T("OS"));
	_tgetenv_s (&retVal, compName, MAX_PATH, _T("COMPUTERNAME"));
	_tgetenv_s (&retVal, userName, MAX_PATH, _T("USERNAME"));
	_tgetenv_s (&retVal, appDataPath, MAX_PATH, _T("APPDATA"));
}

void dxLogger::logSystemInfo () {

	dxTCHARofstream fout;
	
	fout.open (_T("log.txt"), ios::out);
	
	if (fout.fail ()) {
		trace (_T("Error write log file."));
		return;
	}

	fout << _T("Using OS: ") << osName << endl;
	fout << _T("Computer name: ") << compName << endl;
	fout << _T("User name: ") << userName << endl;
	fout << _T("Application data path: ") << appDataPath << endl;
	fout << endl;

	fout.close ();
}

void dxLogger::trace (const TCHAR *s) {

	dxTCHARofstream fout;
	
	fout.open (_T("log.txt"), ios::out | ios::app);
	
	if (fout.fail ()) {
		trace (_T("Error write log file."));
		return;
	}

	fout << s << endl;

	fout.close ();
}