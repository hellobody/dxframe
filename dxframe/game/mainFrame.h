#ifndef MAINFRAME_H_
#define MAINFRAME_H_

//#include "dxInput.h"

//#include "dxButton.h"

#include "../dxframe/dxObj.h"

class mainFrame
{
	//todo: place any members here

	dxObj *skySphere;
	dxObj *background;
	dxObj *box;
	dxObj *sphere;

	/*dxButton *bPlay;
	dxButton *bOptions;
	dxButton *bScores;
	dxButton *bExit;*/

public:
	mainFrame ();
	~mainFrame ();

	void Create ();
	void Update (float dt);	//dt - delta time between frames
	void Render ();
	void Destroy ();
};

#endif