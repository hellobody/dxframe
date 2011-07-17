#ifndef MAINFRAME_H_
#define MAINFRAME_H_

class dxMainFrame
{
	float dt;	//delta time between frames

public:
	dxMainFrame ();
	~dxMainFrame ();

	void SetFrameTime (float dt);

	void Create ();
	void Update ();
	void Render ();
	void Destroy ();
};

#endif