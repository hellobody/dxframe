#include "stdafx.h"
#include "dxSound.h"

extern dxAudio audio;

extern HRESULT hr;

map <std::basic_string <TCHAR>, dxSound *> dxSound::mSounds;

map <std::basic_string <TCHAR>, dxSound *>::iterator it;

void dxSound::CreateSounds () {

	xml_document doc;

	xml_parse_result result = doc.load_file (PATHTO_SOUNDS_XML);

	if (result) {

		xml_node tagRoot = doc.child (_T("sounds"));
		
		for (xml_node tagSound = tagRoot.child (_T("sound")); tagSound; tagSound = tagSound.next_sibling (_T("sound")))
		{
			const TCHAR *key = (TCHAR *) tagSound.attribute (_T("key")).value ();

			const TCHAR *file = (TCHAR *) tagSound.attribute (_T("file")).value ();
			
			dxSound *snd = new dxSound (file);
			
			mSounds [key] = snd;
		}
	
	}
}

void dxSound::DestroySounds () {

	//RELEASE_VECTOR_OR_MAP (mSounds);
}

void dxSound::Play (const TCHAR *key) {

	it = mSounds.find (key);
	if (it->second) {

		it->second->Play ();
	}
}

void dxSound::PlayLoop (const TCHAR *key) {

	it = mSounds.find (key);
	if (it->second) {

		it->second->SetRepeats (DMUS_SEG_REPEAT_INFINITE);
		it->second->Play ();
	}
}

bool dxSound::IsPlaying (const TCHAR *key) {

	it = mSounds.find (key);
	if (it->second) {

		return it->second->IsPlaying ();
	}
	return false;
}

void dxSound::Stop (const TCHAR *key) {

	it = mSounds.find (key);
	if (it->second) {

		it->second->Stop ();
	}
}

dxSound::dxSound (const TCHAR *name) {

	m_pSegment = NULL;

	m_pPerformance = audio.GetPerformance ();

	m_pLoader = audio.GetLoader ();

	hr = CoCreateInstance (CLSID_DirectMusicSegment, NULL, CLSCTX_INPROC, IID_IDirectMusicSegment8, (void **) &m_pSegment);

	if (m_pLoader) {

		hr = m_pLoader->LoadObjectFromFile (CLSID_DirectMusicSegment, IID_IDirectMusicSegment8, (TCHAR *) name, (void **) &m_pSegment);
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