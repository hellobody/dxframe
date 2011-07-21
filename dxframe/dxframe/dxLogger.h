#ifndef DXLOGGER_H
#define DXLOGGER_H

#include "defines.h"
#include "Share.h"
#include "SYS\Stat.h"
#include "Fcntl.h"

#include "dxTCHARofstrem.h"

class dxLogger {

private:

	TCHAR osName [MAX_PATH];
	TCHAR compName [MAX_PATH];
	TCHAR userName [MAX_PATH];

public:

	dxLogger ();
	~dxLogger ();

	void GetSystemInfo ();
	bool LogSystemInfo ();
};

#endif