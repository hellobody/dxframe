#ifndef DXSOUND_H
#define DXSOUND_H

class DXFRAME_API dxSound {

	IDirectMusicPerformance8 *m_pPerformance;
    IDirectMusicLoader8 *m_pLoader;
    IDirectMusicSegment8 *m_pSegment;

public:

	dxSound (TCHAR *name);
	~dxSound ();

	void Play ();
    void Stop ();

    void SetRepeats (DWORD);
    bool IsPlaying ();
};

#endif