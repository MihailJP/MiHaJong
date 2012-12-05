#pragma once

#include <XAudio2.h>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <vector>
#include "snddata.h"

namespace sound {

	/* サウンド操作用クラス */
	class SoundManipulator {
	private:
		IXAudio2* xAudio;
		IXAudio2MasteringVoice* mVoice;
		std::vector<AudioData*> sounds;
		void InitXAudio();
	public:
		SoundManipulator();
		SoundManipulator(HWND hWnd);
		~SoundManipulator();
		void readWaveData(unsigned ID, const std::string& filename, bool looped = false);
		void readVorbisData(unsigned ID, const std::string& filename, bool looped = false);
		void readMidiData(unsigned ID, const std::string& filename, bool looped = false);
		void play(unsigned ID);
		void stop(unsigned ID);
	};

}
