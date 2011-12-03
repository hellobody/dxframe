#ifndef DXAUDIO_H
#define DXAUDIO_H

class DXFRAME_API dxAudio {

	IDirectMusicPerformance8 *m_pPerformance;
    IDirectMusicLoader8	*m_pLoader;

public:

	dxAudio ();
	~dxAudio ();

	void Initialize ();

	void SetAudioFilesDirectory (TCHAR *path);
	
	void SetMasterVolume (long vol);

	//two-kings say: "These functions are declared inline as they are very short and calling them might be faster that way."
	inline IDirectMusicPerformance8 *GetPerformance () {return m_pPerformance;}
    inline IDirectMusicLoader8 *GetLoader () { return m_pLoader;}

	void Destroy ();
};

#endif