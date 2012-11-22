#include "snddata.h"
#ifdef VORBIS_SUPPORT
#include <vorbis/vorbisfile.h>
#include <cstdio>

/* OGGファイル読み込み */
void sound::OggData::Prepare(const std::string& filename) {
	std::memset(&format, 0, sizeof(format));
	std::memset(&buffer, 0, sizeof(buffer));
	// ファイルを開く
	FILE* file;
	if (fopen_s(&file, filename.c_str(), "rb")) throw std::string("ファイルを開けませんでした");
	OggVorbis_File* ovFile(new OggVorbis_File());
	if (ov_open(file, ovFile, nullptr, 0)) {
		fclose(file); throw std::string("Ogg Vorbis ファイルではありません");
	}
	// メタデータ読み込み
	const vorbis_info *info(ov_info(ovFile, -1));
	if (!info) throw std::string("Ogg Vorbis フォーマットが取得できません");
	std::memset(&format, 0, sizeof(format));
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = static_cast<WORD>(info->channels);
	format.nSamplesPerSec = info->rate;
	format.wBitsPerSample = 16;
	format.nBlockAlign = static_cast<WORD>(info->channels * 2);
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
	// データ読み込み
	char buf[1 << 22 /* 4 MiB */] = {0,}; long bytes_read = 0; int current = 0;
	do {
		bytes_read = ov_read(ovFile, buf, 1 << 22, 0, 1, 1, &current);
		if (bytes_read == OV_HOLE) throw std::string("読み込みはOV_HOLEで失敗しました");
		else if (bytes_read == OV_EBADLINK) throw std::string("読み込みはOV_EBADLINKで失敗しました");
		else if (bytes_read == OV_EINVAL) throw std::string("読み込みはOV_EINVALで失敗しました");
		else if (bytes_read)
			buffer.insert(buffer.end(), &buf[0], &buf[bytes_read - 1]);
	} while (bytes_read);
	// 読み終わり
	fclose(file); delete ovFile;
}

sound::OggData::OggData(IXAudio2** Engine, const std::string& filename, bool looped) {
	Prepare(filename);
	std::memset(&bufInfo, 0, sizeof(bufInfo));
	bufInfo.AudioBytes = buffer.size();
	bufInfo.pAudioData = reinterpret_cast<BYTE*>(&buffer[0]);
	bufInfo.LoopCount = (looped ? XAUDIO2_LOOP_INFINITE : 0);
	if (FAILED((*Engine)->CreateSourceVoice(&voice, &format)))
		throw std::string("CreateSourceVoice失敗！！");
	if (FAILED(voice->SubmitSourceBuffer(&bufInfo)))
		throw std::string("SubmitSourceBuffer失敗！！");
}

/* 再生 */
void sound::OggData::Play() {
	if (FAILED(voice->Start(0, XAUDIO2_COMMIT_NOW)))
		throw std::string("Start失敗！！");
}

/* 停止 */
void sound::OggData::Stop() {
	if (FAILED(voice->Stop()))
		throw std::string("Stop失敗！！");
}


#endif /* VORBIS_SUPPORT */
