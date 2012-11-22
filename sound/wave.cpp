#include "snddata.h"
#include <fstream>

/* 4バイト単位のチャンクのチェック */
bool sound::WaveData::checkTag(std::ifstream& file, const std::string& tag) {
	char chkdat[8] = {0,};
	file.read(chkdat, 4);
	return std::string(chkdat).compare(tag) == 0;
}

/* フォーマット読み込み */
void sound::WaveData::GetFormat(std::ifstream& file) {
	static_assert(sizeof(std::uint32_t) == 4, "sizeof(uint32_t) is not 4");
	static_assert(sizeof(std::uint16_t) == 2, "sizeof(uint16_t) is not 2");
	if (!checkTag(file, "WAVE")) throw "WAVEチャンクがないです";
	if (!checkTag(file, "fmt ")) throw "fmt チャンクがないです";
	// ヘッダサイズ
	std::uint32_t format_size; file.read(reinterpret_cast<char*>(&format_size), 4);
	// データフォーマット
	std::uint16_t id; file.read(reinterpret_cast<char*>(&id), 2);
	if (id == 1) format.wFormatTag = WAVE_FORMAT_PCM;
	else if (id == 2) format.wFormatTag = WAVE_FORMAT_ADPCM;
	else throw "対応していないフォーマットです";
	// ヘッダ読み込み
	file.read(reinterpret_cast<char*>(&format.nChannels), sizeof(format.nChannels));
	file.read(reinterpret_cast<char*>(&format.nSamplesPerSec), sizeof(format.nSamplesPerSec));
	file.read(reinterpret_cast<char*>(&format.nAvgBytesPerSec), sizeof(format.nAvgBytesPerSec));
	file.read(reinterpret_cast<char*>(&format.nBlockAlign), sizeof(format.nBlockAlign));
	file.read(reinterpret_cast<char*>(&format.wBitsPerSample), sizeof(format.wBitsPerSample));
	// 拡張ヘッダ情報は無視
	file.ignore(format_size - 16);
}

/* 波形データ読み込み */
void sound::WaveData::ReadWaveData(std::ifstream& file) {
	static_assert(sizeof(std::uint32_t) == 4, "sizeof(uint32_t) is not 4");
	if (!checkTag(file, "data")) throw "dataチャンクがないです";
	std::uint32_t size; file.read(reinterpret_cast<char*>(&size), 4);
	buffer.resize(size);
	// 読み込み
	file.read(&buffer.front(), size);
}

/* WAVEファイル読み込み */
void sound::WaveData::Prepare(const std::string& filename) {
	std::memset(&format, 0, sizeof(format));
	std::memset(&buffer, 0, sizeof(buffer));
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	if (!file) throw "ファイルを開けませんでした";
	if (!checkTag(file, "RIFF")) throw "RIFFチャンクがないです";
	file.ignore(4);
	GetFormat(file);
	ReadWaveData(file);
}

sound::WaveData::WaveData(IXAudio2** Engine, const std::string& filename, bool looped) {
	Prepare(filename);
	std::memset(&bufInfo, 0, sizeof(bufInfo));
	bufInfo.AudioBytes = buffer.size();
	bufInfo.pAudioData = reinterpret_cast<BYTE*>(&buffer[0]);
	bufInfo.LoopCount = (looped ? XAUDIO2_LOOP_INFINITE : 0);
	if (FAILED((*Engine)->CreateSourceVoice(&voice, &format)))
		throw "CreateSourceVoice失敗！！";
	if (FAILED(voice->SubmitSourceBuffer(&bufInfo)))
		throw "SubmitSourceBuffer失敗！！";
}

/* 再生 */
void sound::WaveData::Play() {
	if (FAILED(voice->Start(0, XAUDIO2_COMMIT_NOW)))
		throw "Start失敗！！";
}

/* 停止 */
void sound::WaveData::Stop() {
	if (FAILED(voice->Stop()))
		throw "Stop失敗！！";
}

/* デストラクタ */
sound::SoundData::~SoundData() {
	if (voice) {
		voice->Stop();
		voice->DestroyVoice();
	}
}
