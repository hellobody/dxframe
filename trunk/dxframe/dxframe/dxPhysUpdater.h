#ifndef DXPHYSUPDATER
#define DXPHYSUPDATER

#include "dxPhysPoint.h"

class dxPhysUpdater {

protected:

public:

	dxPhysUpdater ();
	~dxPhysUpdater ();

	void addPhysPoint (dxPhysPoint *newPhysPoint);

	void Update (float dt);
};

#endif