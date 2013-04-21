#pragma once

#ifndef _WIN32
#include <X11/Xlib.h>
#endif
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
#include <AL/al.h>
#include <AL/alc.h>
#elif defined(USE_XAUDIO2)
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
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
		ALCdevice* myDevice;
		ALCcontext* myContext;
#elif defined(USE_XAUDIO2)
		IXAudio2* xAudio;
		IXAudio2MasteringVoice* mVoice;
#else
		LPDIRECTSOUND8 pDSound;
		LPDIRECTSOUNDBUFFER mVoice;
#endif
		std::vector<AudioData*> sounds;
#ifdef _WIN32
		void InitXAudio(HWND hWnd = nullptr);
#else /* _WIN32 */
		void InitXAudio(Window hWnd = 0);
#endif /* _WIN32 */
	public:
		SoundManipulator();
#ifdef _WIN32
		SoundManipulator(HWND hWnd);
#else /* _WIN32 */
		SoundManipulator(Window hWnd);
#endif /* _WIN32 */
		~SoundManipulator();
		void readWaveData(unsigned ID, const std::string& filename, bool looped = false);
		void readVorbisData(unsigned ID, const std::string& filename, bool looped = false);
		void readMidiData(unsigned ID, const std::string& filename, bool looped = false);
		void play(unsigned ID);
		void stop(unsigned ID);
		void setVolume(unsigned ID, double volume);
	};

}
