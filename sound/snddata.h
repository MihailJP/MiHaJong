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
		virtual void Play();
		virtual void Stop();
		virtual void setVolume(double volume);
		explicit SoundData();
		SoundData(const SoundData&) = delete; // Delete unexpected copy constructor
		SoundData& operator= (const SoundData&) = delete; // Delete unexpected assign operator
		virtual ~SoundData() = 0;
	};
	class WaveData : public SoundData {
	private:
		bool checkTag(std::ifstream& file, const std::string& tag);
		void GetFormat(std::ifstream& file);
		void ReadWaveData(std::ifstream& file);
		void Prepare(const std::string& filename);
	public:
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
		explicit WaveData(void*, const std::string& filename, bool looped = false);
#elif defined(USE_XAUDIO2)
		explicit WaveData(IXAudio2** Engine, const std::string& filename, bool looped = false);
#else
		explicit WaveData(LPDIRECTSOUND8* Engine, const std::string& filename, bool looped = false);
#endif
		WaveData(const WaveData&) = delete; // Delete unexpected copy constructor
		WaveData& operator= (const WaveData&) = delete; // Delete unexpected assign operator
	};
	class OggData : public SoundData {
#ifdef VORBIS_SUPPORT
	private:
		void Prepare(const std::string& filename);
	public:
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
		explicit OggData(void*, const std::string& filename, bool looped = false);
#elif defined(USE_XAUDIO2)
		explicit OggData(IXAudio2** Engine, const std::string& filename, bool looped = false);
#else
		explicit OggData(LPDIRECTSOUND8* Engine, const std::string& filename, bool looped = false);
#endif
		OggData(const OggData&) = delete; // Delete unexpected copy constructor
		OggData& operator= (const OggData&) = delete; // Delete unexpected assign operator
#else
	private:
		void Prepare(const std::string&) {}
	public:
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
		explicit OggData(void*, const std::string& filename, bool looped = false) {
#elif defined(USE_XAUDIO2)
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
		MidiData(const MidiData&) = delete; // Delete unexpected copy constructor
		MidiData& operator= (const MidiData&) = delete; // Delete unexpected assign operator
		void Play();
		void Stop();
		virtual void setVolume(double volume);
		virtual ~MidiData();
#else
	public:
		explicit MidiData(unsigned ID, const std::string& filename, bool looped = false) {
			throw CodeConv::tstring(_T("このバージョンはMIDIファイルの再生をサポートしていません"));
		}
		MidiData(const MidiData&) = delete; // Delete unexpected copy constructor
		MidiData& operator= (const MidiData&) = delete; // Delete unexpected assign operator
		void Play() {throw CodeConv::tstring(_T("このバージョンはMIDIファイルの再生をサポートしていません"));}
		void Stop() {throw CodeConv::tstring(_T("このバージョンはMIDIファイルの再生をサポートしていません"));}
		virtual void setVolume(double volume) {
			throw CodeConv::tstring(_T("このバージョンはMIDIファイルの再生をサポートしていません"));
		}
#endif
	};

}
