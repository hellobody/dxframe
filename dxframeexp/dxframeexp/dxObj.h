#ifndef DXOBJ_H
#define DXOBJ_H

class dxObj {
public:

	int numVerts;
	int numTVerts;
	int numFaces;

	float * pVerts;

	dxObj ();
	~dxObj () {};
};

#endif