#ifndef MAINFRAME_H_
#define MAINFRAME_H_

#include "dxInput.h"
#include "dxObj.h"
#include "dxTextField.h"

class dxMainFrame
{
	//todo: place any members here

	dxObj *background;
	dxObj *b_default;
	dxObj *b_selected;

public:
	dxMainFrame ();
	~dxMainFrame ();

	void Create ();
	void Update (float dt);	//dt - delta time between frames
	void Destroy ();
};

#endif