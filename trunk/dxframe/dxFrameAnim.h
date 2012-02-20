#ifndef DX_FRAME_ANIM_H
#define DX_FRAME_ANIM_H

class DXFRAME_API dxFrameAnim
{
	int NumOfFrames;	//кол-во кадров в нашей анимации

	float Speed;		//скорость проигрывания нашей анимации

public:

	dxFrameAnim ();
	~dxFrameAnim ();

	float GetSpeed ();	//получать значение скорости проигрывания анимации

	void SetSpeed (float NewSpeed);
};

#endif