#ifndef DX_FRAME_ANIM_H
#define DX_FRAME_ANIM_H

class DXFRAME_API dxFrameAnim
{
	int NumOfFrames;	//���-�� ������ � ����� ��������

	float Speed;		//�������� ������������ ����� ��������

public:

	dxFrameAnim ();
	~dxFrameAnim ();

	float GetSpeed ();	//�������� �������� �������� ������������ ��������

	void SetSpeed (float NewSpeed);
};

#endif