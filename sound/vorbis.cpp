#include "snddata.h"
#ifdef VORBIS_SUPPORT
#include <vorbis/vorbisfile.h>
#include <cstdio>
#include <sstream>
#include <iomanip>
#include "../mjcore/strcode.h"

/* OGGファイル読み込み */
void sound::OggData::Prepare(const std::string& filename) {
	std::memset(&format, 0, sizeof(format));
	std::memset(&bufInfo, 0, sizeof(buffer));
	// ファイルを開く
	FILE* file;
	if (fopen_s(&file, filename.c_str(), "rb")) throw CodeConv::tstring(_T("ファイルを開けませんでした"));
	OggVorbis_File* ovFile(new OggVorbis_File());
	if (ov_open(file, ovFile, nullptr, 0)) {
		fclose(file); throw CodeConv::tstring(_T("Ogg Vorbis ファイルではありません"));
	}
	// メタデータ読み込み
	const vorbis_info *info(ov_info(ovFile, -1));
	if (!info) throw CodeConv::tstring(_T("Ogg Vorbis フォーマットが取得できません"));
	std::memset(&format, 0, sizeof(format));
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = static_cast<WORD>(info->channels);
	format.nSamplesPerSec = info->rate;
	format.wBitsPerSample = 16;
	format.nBlockAlign = static_cast<WORD>(info->channels * 2);
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
	// データ読み込み
	char* buf = (char*)calloc(1, 1 << 22 /* 4 MiB */);
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
	free(buf); buf = nullptr;
}

sound::OggData::OggData(IXAudio2** Engine, const std::string& filename, bool looped) {
	HRESULT hr;
	Prepare(filename);
	std::memset(&bufInfo, 0, sizeof(bufInfo));
	bufInfo.AudioBytes = buffer.size();
	bufInfo.pAudioData = reinterpret_cast<BYTE*>(&buffer[0]);
	bufInfo.LoopCount = (looped ? XAUDIO2_LOOP_INFINITE : 0);
	if (FAILED(hr = (*Engine)->CreateSourceVoice(&voice, &format))) {
		CodeConv::tostringstream o; o << _T("CreateSourceVoice失敗！！ (0x") <<
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}
}

/* 再生 */
void sound::OggData::Play() {
	Stop();
	HRESULT hr;
	if (FAILED(hr = voice->SubmitSourceBuffer(&bufInfo))) {
		CodeConv::tostringstream o; o << _T("SubmitSourceBuffer失敗！！ (0x") <<
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}
	if (FAILED(hr = voice->Start(0, XAUDIO2_COMMIT_NOW))) {
		CodeConv::tostringstream o; o << _T("Start失敗！！ (0x") <<
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}
}

/* 停止 */
void sound::OggData::Stop() {
	HRESULT hr;
	if (FAILED(hr = voice->Stop())) {
		CodeConv::tostringstream o; o << _T("Stop失敗！！ (0x") <<
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}
	if (FAILED(hr = voice->FlushSourceBuffers())) {
		CodeConv::tostringstream o; o << _T("FlushSourceBuffers失敗！！ (0x") <<
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}
}


#endif /* VORBIS_SUPPORT */
