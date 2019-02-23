#pragma once

#ifndef _WIN32
#include <X11/Xlib.h>
#endif
#ifdef USE_XAUDIO2
#include <xaudio2.h>
#else /* USE_XAUDIO2 */
#include <AL/al.h>
#include <AL/alc.h>
#endif /* USE_XAUDIO2 */
#include <cstdint>
#include <cstring>
#include <fstream>
#include <vector>
#include "audioobj.h"

namespace sound {

	/* サウンド操作用クラス */
	class SoundManipulator {
	private:
#ifdef USE_XAUDIO2
		IXAudio2* xAudio;
		IXAudio2MasteringVoice* mVoice;
#else /* USE_XAUDIO2 */
		ALCdevice* myDevice;
		ALCcontext* myContext;
#endif /* USE_XAUDIO2 */
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
		SoundManipulator(const SoundManipulator&) = delete; // Delete unexpected copy constructor
		SoundManipulator& operator= (const SoundManipulator&) = delete; // Delete unexpected assign operator
		~SoundManipulator();
		void readWaveData(unsigned ID, const std::string& filename, bool looped = false);
		void readVorbisData(unsigned ID, const std::string& filename, bool looped = false);
		void readMidiData(unsigned ID, const std::string& filename, bool looped = false);
		void play(unsigned ID);
		void stop(unsigned ID);
		void setVolume(unsigned ID, double volume);
	};

}
