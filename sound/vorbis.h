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
#ifdef USE_XAUDIO2
		explicit OggData(IXAudio2** Engine, const std::string& filename, bool looped = false);
#else /* USE_XAUDIO2 */
		explicit OggData(void*, const std::string& filename, bool looped = false);
#endif /* USE_XAUDIO2 */
		OggData(const OggData&) = delete; // Delete unexpected copy constructor
		OggData& operator= (const OggData&) = delete; // Delete unexpected assign operator
#else /* VORBIS_SUPPORT */
	private:
		void Prepare(const std::string&) override {}
	public:
#ifdef USE_XAUDIO2
		explicit OggData(IXAudio2** Engine, const std::string& filename, bool looped = false) {
#else /* USE_XAUDIO2 */
		explicit OggData(void*, const std::string& filename, bool looped = false) {
#endif /* USE_XAUDIO2 */
			throw CodeConv::tstring(_T("Vorbisはサポートされていません"));
		}
		void Play() override {throw CodeConv::tstring(_T("Vorbisはサポートされていません"));}
		void Stop() override {throw CodeConv::tstring(_T("Vorbisはサポートされていません"));}
		virtual void setVolume(double volume) override {
			throw CodeConv::tstring(_T("Vorbisはサポートされていません"));
		}
#endif /* VORBIS_SUPPORT */
	};

}
