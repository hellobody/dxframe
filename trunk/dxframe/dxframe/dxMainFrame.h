#ifndef MAINFRAME_H_
#define MAINFRAME_H_

class dxMainFrame
{
	//todo: place any members here	

public:
	dxMainFrame ();
	~dxMainFrame ();

	void Create ();
	void Update (float dt);	//dt - delta time between frames
	void Render ();
	void Destroy ();
};

#endif