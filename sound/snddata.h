#pragma once

#if defined(USE_XAUDIO2)
#include <xaudio2.h>
#else
#include <dsound.h>
#endif
#include <cstdint>
#include <cstring>
#include <vector>
#include "../common/strcode.h"

namespace sound {

	/* サウンドデータクラス */
	class AudioData {
		/* 未実装 */
	public:
		virtual ~AudioData() {};
		virtual void Play() = 0;
		virtual void Stop() = 0;
		virtual void setVolume(double volume) = 0;
	};
	class SoundData : public AudioData {
	protected:
		WAVEFORMATEX format;
		std::vector<char> buffer;
#if defined(USE_XAUDIO2)
		XAUDIO2_BUFFER bufInfo;
		IXAudio2SourceVoice* voice;
#else
		bool withLoop;
		LPDIRECTSOUNDBUFFER voice;
#endif
		virtual void Prepare(const std::string& filename) = 0;
#if defined(USE_XAUDIO2)
		void PrepareBuffer(IXAudio2** Engine, bool looped = false);
#else
		void PrepareBuffer(LPDIRECTSOUND8* Engine, bool looped);
#endif
	public:
		virtual void Play();
		virtual void Stop();
		virtual void setVolume(double volume);
		explicit SoundData();
		virtual ~SoundData() = 0;
	};
	class WaveData : public SoundData {
	private:
		bool checkTag(std::ifstream& file, const std::string& tag);
		void GetFormat(std::ifstream& file);
		void ReadWaveData(std::ifstream& file);
		void Prepare(const std::string& filename);
	public:
#if defined(USE_XAUDIO2)
		explicit WaveData(IXAudio2** Engine, const std::string& filename, bool looped = false);
#else
		explicit WaveData(LPDIRECTSOUND8* Engine, const std::string& filename, bool looped = false);
#endif
	};
	class OggData : public SoundData {
#ifdef VORBIS_SUPPORT
	private:
		void Prepare(const std::string& filename);
	public:
#if defined(USE_XAUDIO2)
		explicit OggData(IXAudio2** Engine, const std::string& filename, bool looped = false);
#else
		explicit OggData(LPDIRECTSOUND8* Engine, const std::string& filename, bool looped = false);
#endif
#else
	private:
		void Prepare(const std::string&) {}
	public:
#if defined(USE_XAUDIO2)
		explicit OggData(IXAudio2** Engine, const std::string& filename, bool looped = false) {
#else
		explicit OggData(LPDIRECTSOUND8* Engine, const std::string& filename, bool looped = false) {
#endif
			throw CodeConv::tstring(_T("Vorbisはサポートされていません"));
		}
		void Play() {throw CodeConv::tstring(_T("Vorbisはサポートされていません"));}
		void Stop() {throw CodeConv::tstring(_T("Vorbisはサポートされていません"));}
		virtual void setVolume(double volume) {
			throw CodeConv::tstring(_T("Vorbisはサポートされていません"));
		}
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
		virtual void setVolume(double volume);
		virtual ~MidiData();
#else
	public:
		explicit MidiData(unsigned ID, const std::string& filename, bool looped = false) {
			throw CodeConv::tstring(_T("このバージョンはMIDIファイルの再生をサポートしていません"));
		}
		void Play() {throw CodeConv::tstring(_T("このバージョンはMIDIファイルの再生をサポートしていません"));}
		void Stop() {throw CodeConv::tstring(_T("このバージョンはMIDIファイルの再生をサポートしていません"));}
		virtual void setVolume(double volume) {
			throw CodeConv::tstring(_T("このバージョンはMIDIファイルの再生をサポートしていません"));
		}
#endif
	};

}
