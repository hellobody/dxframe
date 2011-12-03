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