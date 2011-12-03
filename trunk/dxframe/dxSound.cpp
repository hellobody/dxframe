#include "stdafx.h"
#include "dxSound.h"

extern dxAudio audio;

extern HRESULT hr;

dxSound::dxSound (TCHAR *name) {

	m_pSegment = NULL;

	m_pPerformance = audio.GetPerformance ();

	m_pLoader = audio.GetLoader ();

	hr = CoCreateInstance (CLSID_DirectMusicSegment, NULL, CLSCTX_INPROC, IID_IDirectMusicSegment8, (void **) &m_pSegment);

	if (m_pLoader) {

		hr = m_pLoader->LoadObjectFromFile (CLSID_DirectMusicSegment, IID_IDirectMusicSegment8, name, (void **) &m_pSegment);
	}

	if (m_pSegment) {

		hr = m_pSegment->Download (m_pPerformance);
	}
}

dxSound::~dxSound () {

	if (m_pSegment) {

		m_pSegment->Unload (m_pPerformance);
		m_pSegment->Release ();
	}
}

void dxSound::Play () {

	if (m_pPerformance) {

		hr = m_pPerformance->PlaySegmentEx (m_pSegment, 0, NULL, DMUS_SEGF_SECONDARY, 0, 0, NULL, NULL);
	}
}

void dxSound::Stop () {

	if (m_pPerformance) {

		hr = m_pPerformance->StopEx (m_pSegment, 0, 0);
	}
}

void dxSound::SetRepeats (DWORD dwRepeats) {

	if (m_pSegment) {

		hr = m_pSegment->SetRepeats (dwRepeats);
	}
}

bool dxSound::IsPlaying () {

	if (m_pPerformance) {
		return (m_pPerformance->IsPlaying (m_pSegment, NULL) == S_OK);
	}
	return false;
}