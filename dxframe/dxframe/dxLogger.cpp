#include "dxLogger.h"

dxLogger::dxLogger () {
	
	_tcscpy_s (osName, MAX_PATH, _T(""));
	_tcscpy_s (compName, MAX_PATH, _T(""));
	_tcscpy_s (userName, MAX_PATH, _T(""));
}

dxLogger::~dxLogger () {
	
}

void dxLogger::GetSystemInfo () {

	size_t retVal;

	_tgetenv_s (&retVal, osName, MAX_PATH, _T("OS"));
	_tgetenv_s (&retVal, compName, MAX_PATH, _T("COMPUTERNAME"));
	_tgetenv_s (&retVal, userName, MAX_PATH, _T("USERNAME"));
}

bool dxLogger::LogSystemInfo () {

	dxTCHARofstream fout;
	
	fout.open (_T("log.txt"), ios::out);
	
	if (fout.fail ()) {
		trace (_T("Error write log file."));
		return false;
	}

	fout << _T("Using OS: ") << osName << endl;
	fout << _T("Computer name: ") << compName << endl;
	fout << _T("User name: ") << userName << endl;

	fout.close ();

	return true;
}