#ifndef DXCONSOLE_H
#define DXCONSOLE_H

#include "defines.h"

class dxConsole {

public:
	dxConsole ();
	~dxConsole ();

	void Update ();

	void ProcCommand (TCHAR *Command);

	void Close ();
};

#endif