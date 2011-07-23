#ifndef MAINFRAME_H_
#define MAINFRAME_H_

#include "dxObj.h"

class dxMainFrame
{
	//todo: place any members here

	dxObj *obj2;
	dxObj *obj;

public:
	dxMainFrame ();
	~dxMainFrame ();

	void Create ();
	void Update (float dt);	//dt - delta time between frames
	void Destroy ();
};

#endif