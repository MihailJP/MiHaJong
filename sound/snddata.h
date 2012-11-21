#ifndef SOUND_DLL_SNDDATA_H
#define SOUND_DLL_SNDDATA_H

#include <XAudio2.h>
#include <cstdint>
#include <cstring>
#include <vector>

namespace sound {

	/* サウンドデータクラス */
	class AudioData {
		/* 未実装 */
	public:
		virtual ~AudioData() {};
		virtual void Play() = 0;
		virtual void Stop() = 0;
	};
	class SoundData : public AudioData {
	protected:
		WAVEFORMATEX format;
		std::vector<char> buffer;
		XAUDIO2_BUFFER bufInfo;
		IXAudio2SourceVoice* voice;
	public:
		virtual ~SoundData();
	};
	class WaveData : public SoundData {
	private:
		bool checkTag(std::ifstream& file, const std::string& tag);
		void GetFormat(std::ifstream& file);
		void ReadWaveData(std::ifstream& file);
		void Prepare(const std::string& filename);
	public:
		explicit WaveData(IXAudio2** Engine, const std::string& filename, bool looped = false);
		void Play();
		void Stop();
	};
	class OggData : public SoundData {
	private:
		void Prepare(const std::string& filename);
#ifdef VORBIS_SUPPORT
	public:
		explicit OggData(IXAudio2** Engine, const std::string& filename, bool looped = false);
		void Play();
		void Stop();
#else
	public:
		explicit OggData(IXAudio2** Engine, const std::string& filename, bool looped = false) {
			throw "Vorbisはサポートされていません";
		}
		void Play() {throw "Vorbisはサポートされていません";}
		void Stop() {throw "Vorbisはサポートされていません";}
#endif
	};
	class MidiData : public AudioData {
		/* 未実装 */
	};

}

#endif
