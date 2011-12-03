#include "stdafx.h"
#include "dxPhysPoint.h"

dxPhysPoint::dxPhysPoint () {

	Reset ();
}

dxPhysPoint::~dxPhysPoint () {

	
}

void dxPhysPoint::Reset () {

	P = D3DXVECTOR2 (0, 0);
	V = D3DXVECTOR2 (0, 0);
	A = D3DXVECTOR2 (0, 0);
}

void dxPhysPoint::Update (float dt) {

	P += V * dt;
	V += A * dt;
}

void dxPhysPoint::SetPos (D3DXVECTOR2 v) {

	P = v;
}

D3DXVECTOR2 dxPhysPoint::GetPos () {

	return P;
}

void dxPhysPoint::SetVel (D3DXVECTOR2 v) {

	V = v;
}

D3DXVECTOR2 dxPhysPoint::GetVel () {

	return V;
}

void dxPhysPoint::SetAcl (D3DXVECTOR2 v) {

	A = v;
}

D3DXVECTOR2 dxPhysPoint::GetAcl () {

	return A;
}