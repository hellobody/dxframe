#include "stdafx.h"

#include "dxFrameAnim.h"

dxFrameAnim::dxFrameAnim ()
{
	NumOfFrames = 0;	//���-�� ������ � ����� ��������

	Speed = 1.f;		//�������� ������������ ����� ��������
}

dxFrameAnim::~dxFrameAnim ()
{
	
}

float dxFrameAnim::GetSpeed ()	//�������� �������� �������� ������������ ��������
{
	return Speed;
}

void dxFrameAnim::SetSpeed (float NewSpeed)
{
	Speed = NewSpeed;
}