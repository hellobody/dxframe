#include "stdafx.h"
#include "dxLandObj.h"

dxLandObj::dxLandObj () {
	body = NULL;
}

dxLandObj::~dxLandObj () {
	Destroy ();
}

void dxLandObj::Create () {
	DEL (body);
	body = new dxObj (_T("palm_tree.dxf"), "palm_tree");
}

void dxLandObj::Reset () {

}

void dxLandObj::Update () {
	
}

void dxLandObj::Render () {
	if (body) body->Render ();
}

void dxLandObj::Destroy () {
	
}

void dxLandObj::SetPosZ (float v) {

	if (body) {

		body->MoveToZ (v);
	}
}