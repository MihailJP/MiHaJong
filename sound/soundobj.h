#ifndef SOUND_DLL_SOUNDOBJ_H
#define SOUND_DLL_SOUNDOBJ_H

#include <XAudio2.h>

namespace sound {

	class SoundManipulator {
	private:
		IXAudio2* xAudio;
		IXAudio2MasteringVoice* mVoice;
	public:
		SoundManipulator();
		~SoundManipulator();
	};

}

#endif
