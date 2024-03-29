#ifndef DXCAMERA_H
#define DXCAMERA_H

#include "defines.h"

class DXFRAME_API cCamera {
	
	D3DXVECTOR3 up;
	D3DXVECTOR3 look;
	D3DXVECTOR3 right;
	D3DXVECTOR3 pos;

public:

	enum eCameraType { LANDOBJECT, AIRCRAFT };

	eCameraType type;
	
	cCamera ();
	cCamera (eCameraType);
	cCamera (D3DXVECTOR3);
	~cCamera ();

	void strafe (float);
	void walk (float);
	void fly (float);

	void pitch (float);
	void roll (float);
	void yaw (float);

	D3DXMATRIX getViewMatrix ();
	D3DXVECTOR3 getPos ();
	
	void setPos (D3DXVECTOR3 *);

	void AddPosX (float v);
	void AddPosY (float v);
	void AddPosZ (float v);

	void setType (eCameraType);

	D3DXVECTOR3 getUp ();
	D3DXVECTOR3 getLook ();
	D3DXVECTOR3 getRight ();
};

#endif