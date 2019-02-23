#pragma once

#include "../common/strcode.h"
#include "snddata.h"

namespace sound {

	/* Ogg Vorbis 用オブジェクト */
	class OggData : public SoundData {
#ifdef VORBIS_SUPPORT
	private:
		void Prepare(const std::string& filename);
	public:
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
		explicit OggData(void*, const std::string& filename, bool looped = false);
#else
		explicit OggData(IXAudio2** Engine, const std::string& filename, bool looped = false);
#endif
		OggData(const OggData&) = delete; // Delete unexpected copy constructor
		OggData& operator= (const OggData&) = delete; // Delete unexpected assign operator
#else
	private:
		void Prepare(const std::string&) {}
	public:
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
		explicit OggData(void*, const std::string& filename, bool looped = false) {
#else
		explicit OggData(IXAudio2** Engine, const std::string& filename, bool looped = false) {
#endif
			throw CodeConv::tstring(_T("Vorbisはサポートされていません"));
		}
		void Play() override {throw CodeConv::tstring(_T("Vorbisはサポートされていません"));}
		void Stop() override {throw CodeConv::tstring(_T("Vorbisはサポートされていません"));}
		virtual void setVolume(double volume) override {
			throw CodeConv::tstring(_T("Vorbisはサポートされていません"));
		}
#endif
	};

}
