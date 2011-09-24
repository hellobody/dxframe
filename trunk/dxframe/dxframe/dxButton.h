#ifndef DXBUTTON
#define DXBUTTON

#include "dxObj.h"

class DXFRAME_API dxButton {

protected:

	enum eState {eDefault, eSelected, ePressed, eNumOfStates};

	eState lstState;
	eState curState;

	dxObj *objs [eNumOfStates];

public:

	dxButton (const TCHAR *FileName, const char *DefaultObjName, const char *SelectedObjName, const char *PressedObjName);
	~dxButton ();

	void Move (float, float, float);
	void Scale (float, float, float);

	bool IsDown ();
	bool IsUp ();
	bool IsToggledDown ();
	bool IsToggledUp ();

	void Update (float dt);
	void Render ();
};

#endif 