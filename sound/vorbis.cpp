#include "snddata.h"
#ifdef VORBIS_SUPPORT
#ifdef __MINGW32__ /* Workaround */
#undef __MINGW32__
#undef _WIN32
#include <vorbis/vorbisfile.h>
#define _WIN32
#define __MINGW32__
#else /* __MINGW32__ */
#include <vorbis/vorbisfile.h>
#endif /* __MINGW32__ */
#include <cstdio>
#include <sstream>
#include <iomanip>
#include "../common/strcode.h"
#include <cassert>

/* OGGファイル読み込み */
void sound::OggData::Prepare(const std::string& filename) {
	std::memset(&format, 0, sizeof(format));
#if defined(USE_XAUDIO2)
	std::memset(&bufInfo, 0, sizeof(buffer));
#endif
	// ファイルを開く
	FILE* file;
#ifdef _MSC_VER
	if (fopen_s(&file, filename.c_str(), "rb"))
#else
	if ((file = fopen(filename.c_str(), "rb")) == nullptr)
#endif
		throw CodeConv::tstring(_T("ファイルを開けませんでした"));
	assert(file != nullptr);
	OggVorbis_File* ovFile(new OggVorbis_File());
	if (ov_open(file, ovFile, nullptr, 0)) {
		fclose(file); throw CodeConv::tstring(_T("Ogg Vorbis ファイルではありません"));
	}
	// メタデータ読み込み
	const vorbis_info *info(ov_info(ovFile, -1));
	if (!info) throw CodeConv::tstring(_T("Ogg Vorbis フォーマットが取得できません"));
	std::memset(&format, 0, sizeof(format));
#ifdef _WIN32
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = static_cast<WORD>(info->channels);
	format.nBlockAlign = static_cast<WORD>(info->channels * 2);
#else /* _WIN32 */
	format.nChannels = static_cast<uint16_t>(info->channels);
	format.nBlockAlign = static_cast<uint16_t>(info->channels * 2);
#endif /* _WIN32 */
	format.nSamplesPerSec = info->rate;
	format.wBitsPerSample = 16;
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
	// データ読み込み
	char* buf = new char[1 << 22 /* 4 MiB */]();
	long bytes_read = 0; int current = 0;
	do {
		bytes_read = ov_read(ovFile, buf, (1 << 22) - 1, 0, 2, 1, &current);
		if (bytes_read == OV_HOLE) throw CodeConv::tstring(_T("読み込みはOV_HOLEで失敗しました"));
		else if (bytes_read == OV_EBADLINK) throw CodeConv::tstring(_T("読み込みはOV_EBADLINKで失敗しました"));
		else if (bytes_read == OV_EINVAL) throw CodeConv::tstring(_T("読み込みはOV_EINVALで失敗しました"));
		else if (bytes_read)
			buffer.insert(buffer.end(), &buf[0], &buf[bytes_read]);
	} while (bytes_read);
	// 読み終わり
	fclose(file); delete ovFile;
	delete[] buf; buf = nullptr;
}

#if !defined(_WIN32) || !defined(WITH_DIRECTX)
sound::OggData::OggData(void* Engine, const std::string& filename, bool looped) {
#elif defined(USE_XAUDIO2)
sound::OggData::OggData(IXAudio2** Engine, const std::string& filename, bool looped) {
#else
sound::OggData::OggData(LPDIRECTSOUND8* Engine, const std::string& filename, bool looped) {
#endif
	Prepare(filename);
	PrepareBuffer(Engine, looped);
}


#endif /* VORBIS_SUPPORT */
