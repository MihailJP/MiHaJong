#ifndef SOUND_DLL_SOUNDOBJ_H
#define SOUND_DLL_SOUNDOBJ_H

#include <XAudio2.h>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <vector>
#include "snddata.h"

namespace sound {

	/* �T�E���h����p�N���X */
	class SoundManipulator {
	private:
		IXAudio2* xAudio;
		IXAudio2MasteringVoice* mVoice;
		std::vector<AudioData*> sounds;
	public:
		SoundManipulator();
		~SoundManipulator();
		void readWaveData(unsigned ID, const std::string& filename);
		void play(unsigned ID);
		void stop(unsigned ID);
	};

}

#endif