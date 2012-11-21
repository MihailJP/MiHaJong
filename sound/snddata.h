#ifndef SOUND_DLL_SNDDATA_H
#define SOUND_DLL_SNDDATA_H

#include <XAudio2.h>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <vector>

namespace sound {

	/* �T�E���h�f�[�^�N���X */
	class AudioData {
		/* ������ */
	public:
		virtual ~AudioData() {};
	};
	class SoundData : public AudioData {
	protected:
		WAVEFORMATEX format;
		std::vector<char> buffer;
		virtual void Prepare(const std::string& filename) = 0;
	public:
		virtual ~SoundData() {}
	};
	class WaveData : public SoundData {
	private:
		bool checkTag(std::ifstream& file, const std::string& tag);
		void GetFormat(std::ifstream& file);
		void ReadWaveData(std::ifstream& file);
		void Prepare(const std::string& filename);
	public:
		WaveData(const std::string& filename);
	};
	class OggData : public SoundData {
		/* ������ */
	};
	class MidiData : public AudioData {
		/* ������ */
	};

}

#endif
