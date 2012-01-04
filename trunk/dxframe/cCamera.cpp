#include "stdafx.h"
#include "cCamera.h"

cCamera::cCamera () {
	type = AIRCRAFT;

	up = D3DXVECTOR3 (0,1,0);
	look = D3DXVECTOR3 (0,0,1);
	right = D3DXVECTOR3 (1,0,0);
	pos = D3DXVECTOR3 (0,0,250);
}

cCamera::cCamera (eCameraType type) {
	cCamera tCam;
	*this = tCam;

	this->type = type;
}

cCamera::cCamera (D3DXVECTOR3 pos) {
	cCamera tCam;
	*this = tCam;

	this->pos = pos;
}

cCamera::~cCamera () {

}

void cCamera::strafe (float units) {
	
	if (type == LANDOBJECT) pos += D3DXVECTOR3 (right.x, 0, right.z) * units;
	else if (type == AIRCRAFT) pos += right * units;
}

void cCamera::walk (float units) {

	if (type == LANDOBJECT) pos += D3DXVECTOR3 (look.x, 0, look.z) * units;
	else if (type == AIRCRAFT) pos += look * units;
}

void cCamera::fly (float units) {

}

void cCamera::pitch (float angle) {
	
	D3DXMATRIX T;
	D3DXMatrixRotationAxis (&T, &right, angle);
	D3DXVec3TransformCoord (&up, &up, &T);
	D3DXVec3TransformCoord (&look, &look, &T);
}

void cCamera::roll (float angle) {

}

void cCamera::yaw (float angle) {

	D3DXMATRIX T;
	/*if (type == LANDOBJECT) D3DXMatrixRotationY (&T, angle);
	else if (type == AIRCRAFT) D3DXMatrixRotationAxis (&T, &up, angle);*/
	D3DXMatrixRotationY (&T, angle);
	D3DXVec3TransformCoord (&right, &right, &T);
	D3DXVec3TransformCoord (&look, &look, &T);
}

D3DXMATRIX cCamera::getViewMatrix () {

	D3DXVec3Normalize (&look, &look);

	D3DXVec3Cross (&up, &look, &right);
	D3DXVec3Normalize (&up, &up);

	D3DXVec3Cross (&right, &up, &look);
	D3DXVec3Normalize (&right, &right);

	D3DXMATRIX M;

	float x = -D3DXVec3Dot (&right, &pos);
	float y = -D3DXVec3Dot (&up, &pos);
	float z = -D3DXVec3Dot (&look, &pos);

	M (0, 0) = right.x;
	M (0, 1) = up.x;
	M (0, 2) = look.x;
	M (0, 3) = 0;

	M (1, 0) = right.y;
	M (1, 1) = up.y;
	M (1, 2) = look.y;
	M (1, 3) = 0;

	M (2, 0) = right.z;
	M (2, 1) = up.z;
	M (2, 2) = look.z;
	M (2, 3) = 0;

	M (3, 0) = x;
	M (3, 1) = y;
	M (3, 2) = z;
	M (3, 3) = 1;

	return M;
}

D3DXVECTOR3 cCamera::getPos () {
	D3DXVECTOR3 V;
	return V;
}

void cCamera::setPos (D3DXVECTOR3* pos) {
	
	this->pos = *pos;
}

void cCamera::setType (eCameraType type) {
	this->type = type;
}

D3DXVECTOR3 cCamera::getUp () {
	D3DXVECTOR3 V;
	return V;
}

D3DXVECTOR3 cCamera::getLook () {
	D3DXVECTOR3 V;
	return V;
}

D3DXVECTOR3 cCamera::getRight () {
	D3DXVECTOR3 V;
	return V; 
}