#ifndef DXSOUND_H
#define DXSOUND_H

class DXFRAME_API dxSound {

	IDirectMusicPerformance8 *m_pPerformance;
    IDirectMusicLoader8 *m_pLoader;
    IDirectMusicSegment8 *m_pSegment;

public:

	static map <std::basic_string <TCHAR>, dxSound *> mSounds;

	static void CreateSounds ();
	static void DestroySounds ();

	static void Play (const TCHAR *key);
	static void PlayLoop (const TCHAR *key);
	static bool IsPlaying (const TCHAR *key);
	static void Stop (const TCHAR *key);

	dxSound (const TCHAR *name);
	~dxSound ();

	void Play ();
    void Stop ();

    void SetRepeats (DWORD);
    bool IsPlaying ();
};

#endif