#pragma once

#if !defined(_WIN32) || !defined(WITH_DIRECTX)
#include <AL/al.h>
#include <AL/alc.h>
#else
#include <xaudio2.h>
#endif
#include <cstdint>
#include <cstring>
#include "../common/strcode.h"
#include "snddata.h"

namespace sound {

#ifndef _WIN32
	struct WaveFormat {
		uint32_t nSamplesPerSec;
		uint32_t nAvgBytesPerSec;
		uint16_t nBlockAlign;
		uint16_t wBitsPerSample;
		uint16_t nChannels;
	};
#endif /* _WIN32 */

	/* Waveデータ用オブジェクト */
	class WaveData : public SoundData {
	private:
		bool checkTag(std::ifstream& file, const std::string& tag);
		void GetFormat(std::ifstream& file);
		void ReadWaveData(std::ifstream& file);
		void Prepare(const std::string& filename);
	public:
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
		explicit WaveData(void*, const std::string& filename, bool looped = false);
#else
		explicit WaveData(IXAudio2** Engine, const std::string& filename, bool looped = false);
#endif
		WaveData(const WaveData&) = delete; // Delete unexpected copy constructor
		WaveData& operator= (const WaveData&) = delete; // Delete unexpected assign operator
	};

}
