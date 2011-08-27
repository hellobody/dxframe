#ifndef DXCONSOLE_H
#define DXCONSOLE_H

#include "defines.h"

class dxConsole {

protected:

	TCHAR *buf;

	HANDLE hIConsole;
	HANDLE hOConsole;

public:
	dxConsole ();
	~dxConsole ();

	bool Update ();

	bool ProcCommand (TCHAR *Command);
};

#endif