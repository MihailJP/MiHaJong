#pragma once

#ifdef USE_XAUDIO2
#include <xaudio2.h>
#else /* USE_XAUDIO2 */
#include <AL/al.h>
#include <AL/alc.h>
#endif /* USE_XAUDIO2 */
#include <cstdint>
#include <cstring>
#include "../common/strcode.h"
#include "snddata.h"

namespace sound {

	/* Waveデータ用オブジェクト */
	class WaveData : public SoundData {
	private:
		bool checkTag(std::ifstream& file, const std::string& tag);
		void GetFormat(std::ifstream& file);
		void ReadWaveData(std::ifstream& file);
		void Prepare(const std::string& filename);
	public:
#ifdef USE_XAUDIO2
		explicit WaveData(IXAudio2** Engine, const std::string& filename, bool looped = false);
#else /* USE_XAUDIO2 */
		explicit WaveData(void*, const std::string& filename, bool looped = false);
#endif /* USE_XAUDIO2 */
		WaveData(const WaveData&) = delete; // Delete unexpected copy constructor
		WaveData& operator= (const WaveData&) = delete; // Delete unexpected assign operator
	};

}
