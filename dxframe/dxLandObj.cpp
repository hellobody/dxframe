#include "stdafx.h"
#include "dxLandObj.h"

dxLandObj::dxLandObj ()
{
	body = NULL;
}

dxLandObj::~dxLandObj ()
{
	Destroy ();
}

void dxLandObj::Create ()
{
	DEL (body);
	body = new dxObj (_T("file_name.dxf"), "model_name");
}

void dxLandObj::Reset ()
{

}

void dxLandObj::Update ()
{

}

void dxLandObj::Render ()
{

}

void dxLandObj::Destroy ()
{

}
