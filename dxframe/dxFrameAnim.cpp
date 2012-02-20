#include "stdafx.h"

#include "dxFrameAnim.h"

dxFrameAnim::dxFrameAnim ()
{
	NumOfFrames = 0;	//кол-во кадров в нашей анимации

	Speed = 1.f;		//скорость проигрывания нашей анимации
}

dxFrameAnim::~dxFrameAnim ()
{
	
}

float dxFrameAnim::GetSpeed ()	//получать значение скорости проигрывания анимации
{
	return Speed;
}

void dxFrameAnim::SetSpeed (float NewSpeed)
{
	Speed = NewSpeed;
}