#ifndef MAINFRAME_H_
#define MAINFRAME_H_

#include "dxInput.h"
#include "dxObj.h"
#include "dxTextField.h"
#include "dxButton.h"

class dxMainFrame
{
	//todo: place any members here

	dxObj *background;
	dxObj *b_default;
	dxObj *b_selected;

	dxButton *bPlay;
	dxButton *bOptions;
	dxButton *bScores;
	dxButton *bExit;

public:
	dxMainFrame ();
	~dxMainFrame ();

	void Create ();
	void Update (float dt);	//dt - delta time between frames
	void Render ();
	void Destroy ();
};

#endif