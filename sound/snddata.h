#pragma once

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
#include <vector>
#include "../common/strcode.h"
#include "audioobj.h"

namespace sound {

	/* 音声データ用スーパークラス */
	class SoundData : public AudioData {
	protected:
#ifdef _WIN32
		WAVEFORMATEX format;
#else /* _WIN32 */
		WaveFormat format;
#endif /* _WIN32 */
		std::vector<char> buffer;
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
		ALuint mySource, myBuffer;
#elif defined(USE_XAUDIO2)
		XAUDIO2_BUFFER bufInfo;
		IXAudio2SourceVoice* voice;
#else
		bool withLoop;
		LPDIRECTSOUNDBUFFER voice;
#endif
		virtual void Prepare(const std::string& filename) = 0;
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
		void PrepareBuffer(void*, bool looped = false);
#elif defined(USE_XAUDIO2)
		void PrepareBuffer(IXAudio2** Engine, bool looped = false);
#else
		void PrepareBuffer(LPDIRECTSOUND8* Engine, bool looped);
#endif
	public:
		virtual void Play() override;
		virtual void Stop() override;
		virtual void setVolume(double volume) override;
		explicit SoundData();
		SoundData(const SoundData&) = delete; // Delete unexpected copy constructor
		SoundData& operator= (const SoundData&) = delete; // Delete unexpected assign operator
		virtual ~SoundData() = 0;
	};

}
