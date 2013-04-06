#pragma once

#if defined(USE_XAUDIO2)
#include <xaudio2.h>
#else
#include <dsound.h>
#endif
#include <cstdint>
#include <cstring>
#include <fstream>
#include <vector>
#include "snddata.h"

namespace sound {

	/* サウンド操作用クラス */
	class SoundManipulator {
	private:
#if defined(USE_XAUDIO2)
		IXAudio2* xAudio;
		IXAudio2MasteringVoice* mVoice;
#else
		LPDIRECTSOUND8 pDSound;
		LPDIRECTSOUNDBUFFER mVoice;
#endif
		std::vector<AudioData*> sounds;
		void InitXAudio(HWND hWnd = nullptr);
	public:
		SoundManipulator();
		SoundManipulator(HWND hWnd);
		~SoundManipulator();
		void readWaveData(unsigned ID, const std::string& filename, bool looped = false);
		void readVorbisData(unsigned ID, const std::string& filename, bool looped = false);
		void readMidiData(unsigned ID, const std::string& filename, bool looped = false);
		void play(unsigned ID);
		void stop(unsigned ID);
		void setVolume(unsigned ID, double volume);
	};

}
