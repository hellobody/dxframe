#ifndef DXBUTTON
#define DXBUTTON

#include "dxObj.h"

class dxButton {

protected:

	enum eState {eDefault, eSelected, ePressed, eNumOfStates};

	eState state;

	dxObj *objs [eNumOfStates];

public:

	dxButton (const TCHAR *FileName, const char *DefaultObjName, const char *SelectedObjName, const char *PressedObjName);
	~dxButton ();

	void Update (float dt);
	void Render ();
};

#endif 