#ifndef MAINFRAME_H_
#define MAINFRAME_H_

#include "dxObj.h"
#include "dxTextField.h"

class dxMainFrame
{
	//todo: place any members here

	dxObj *obj0;
	dxObj *obj1;

public:
	dxMainFrame ();
	~dxMainFrame ();

	void Create ();
	void Update (float dt);	//dt - delta time between frames
	void Destroy ();
};

#endif