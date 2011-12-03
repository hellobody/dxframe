#include "stdafx.h"
#include "dxAudio.h"
#include "dxLogger.h"

HRESULT hr;

dxAudio::dxAudio () {

	m_pPerformance = NULL;
	m_pLoader = NULL;
}

dxAudio::~dxAudio () {

	Destroy ();
}

void dxAudio::Initialize () {

	hr = CoInitialize (NULL);

	hr = CoCreateInstance (CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, IID_IDirectMusicLoader8, (void **) &m_pLoader);

	hr = CoCreateInstance (CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, IID_IDirectMusicPerformance8, (void **) &m_pPerformance);

	if (m_pPerformance) {
		//here initialize DirectMusic & DirectSound
		hr = m_pPerformance->InitAudio (NULL, NULL, NULL, DMUS_APATH_DYNAMIC_STEREO, 64, DMUS_AUDIOF_ALL, NULL);
	}
}

void dxAudio::SetAudioFilesDirectory (TCHAR *path) {

	if (m_pLoader) {

		hr = m_pLoader->SetSearchDirectory (GUID_DirectMusicAllTypes, path, false);
	}
}

void dxAudio::SetMasterVolume (long vol) {

	if (m_pPerformance) {

		hr = m_pPerformance->SetGlobalParam (GUID_PerfMasterVolume, &vol, sizeof (long));
	}
}

void dxAudio::Destroy () {

	if (m_pPerformance) {

		m_pPerformance->Stop (NULL, NULL, 0, 0);
	}

	if (m_pLoader) m_pLoader->Release ();
	if (m_pPerformance) m_pPerformance->CloseDown ();
    
	CoUninitialize ();
}