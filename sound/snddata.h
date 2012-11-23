#ifndef SOUND_DLL_SNDDATA_H
#define SOUND_DLL_SNDDATA_H

#include <XAudio2.h>
#include <cstdint>
#include <cstring>
#include <vector>

namespace sound {

	/* �T�E���h�f�[�^�N���X */
	class AudioData {
		/* ������ */
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
		explicit SoundData();
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
			throw std::string("Vorbis�̓T�|�[�g����Ă��܂���");
		}
		void Play() {throw std::string("Vorbis�̓T�|�[�g����Ă��܂���");}
		void Stop() {throw std::string("Vorbis�̓T�|�[�g����Ă��܂���");}
#endif
	};
	class MidiData : public AudioData {
#if defined(MIDI_SUPPORT) && defined(_WIN32)
	private:
		int myID;
		bool loopFlag;
	public:
		explicit MidiData(unsigned ID, const std::string& filename, bool looped = false);
		void Play();
		void Stop();
		virtual ~MidiData();
#else
	public:
		explicit MidiData(unsigned ID, const std::string& filename, bool looped = false) {
			throw std::string("���̃o�[�W������MIDI�t�@�C���̍Đ����T�|�[�g���Ă��܂���");
		}
		void Play() {throw std::string("���̃o�[�W������MIDI�t�@�C���̍Đ����T�|�[�g���Ă��܂���");}
		void Stop() {throw std::string("���̃o�[�W������MIDI�t�@�C���̍Đ����T�|�[�g���Ă��܂���");}
#endif
	};

}

#endif
