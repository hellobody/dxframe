#include "stdafx.h"
#include "dxLandObj.h"

extern dxInput input;

float dxLandObj::UpCamOffset = 15;

dxLandObj::dxLandObj () {
	body = NULL;
}

dxLandObj::~dxLandObj () {
	Destroy ();
}

void dxLandObj::Create (const TCHAR *fName, const char *mName) {
	DEL (body);
	body = new dxObj (fName, mName);
	Reset ();
}

void dxLandObj::Reset () {
	
}

void dxLandObj::Update (float dt) {
	UpCamOffset += (float) input.GetMouseDeltaZ () * .05f;

	if (UpCamOffset < 0) {
		UpCamOffset = 0;
	}
}

void dxLandObj::Render () {
	if (body) body->Render ();
}

void dxLandObj::Destroy () {
	
}

void dxLandObj::SetPosX (float v) {

	if (body) {

		body->MoveToX (v);
	}
}

void dxLandObj::SetPosY (float v) {

	if (body) {

		body->MoveToY (v);
	}
}

void dxLandObj::SetPosZ (float v) {

	if (body) {

		body->MoveToZ (v);
	}
}

void dxLandObj::AddPos (D3DXVECTOR3 v) {

	if (body) {

		body->Move (v);
	}
}

D3DXVECTOR3 dxLandObj::GetUpCamPos () {

	if (body) {
		return body->GetPos3 () + D3DXVECTOR3 (0, -UpCamOffset, UpCamOffset);
	}

	return D3DXVECTOR3 (0, 0, 0);
}

D3DXVECTOR3 dxLandObj::GetPos () {

	if (body) {

		return body->GetPos3 ();
	}

	return D3DXVECTOR3 (0, 0, 0);
}

const TCHAR *dxLandObj::GetFileName () {
	if (body) {
		return body->GetFileName ();
	}
	return NULL;
}

const TCHAR *dxLandObj::GetObjName () {
	if (body) {
		return body->GetObjName ();
	}
	return NULL;
}