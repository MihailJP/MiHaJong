#pragma once

#ifdef USE_XAUDIO2
#include <xaudio2.h>
#else /* USE_XAUDIO2 */
#include <AL/al.h>
#include <AL/alc.h>
#endif /* USE_XAUDIO2 */
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
#ifdef USE_XAUDIO2
		XAUDIO2_BUFFER bufInfo;
		IXAudio2SourceVoice* voice;
#else /* USE_XAUDIO2 */
		ALuint mySource, myBuffer;
#endif /* USE_XAUDIO2 */
		virtual void Prepare(const std::string& filename) = 0;
#ifdef USE_XAUDIO2
		void PrepareBuffer(IXAudio2** Engine, bool looped = false);
#else /* USE_XAUDIO2 */
		void PrepareBuffer(void*, bool looped = false);
#endif /* USE_XAUDIO2 */
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
