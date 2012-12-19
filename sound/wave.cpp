#include "snddata.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include "logger.h"

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
	if (!checkTag(file, "WAVE")) throw std::string("WAVEチャンクがないです");
	if (!checkTag(file, "fmt ")) throw std::string("fmt チャンクがないです");
	// ヘッダサイズ
	std::uint32_t format_size; file.read(reinterpret_cast<char*>(&format_size), 4);
	// データフォーマット
	std::uint16_t id; file.read(reinterpret_cast<char*>(&id), 2);
	if (id == 1) format.wFormatTag = WAVE_FORMAT_PCM;
	else if (id == 2) format.wFormatTag = WAVE_FORMAT_ADPCM;
	else throw std::string("対応していないフォーマットです");
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
	if (!checkTag(file, "data")) throw std::string("dataチャンクがないです");
	std::uint32_t size; file.read(reinterpret_cast<char*>(&size), 4);
	buffer.resize(size);
	// 読み込み
	file.read(&buffer.front(), size);
}

/* WAVEファイル読み込み */
void sound::WaveData::Prepare(const std::string& filename) {
	std::memset(&format, 0, sizeof(format));
	std::memset(&bufInfo, 0, sizeof(buffer));
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	if (!file) throw std::string("ファイルを開けませんでした");
	if (!checkTag(file, "RIFF")) throw std::string("RIFFチャンクがないです");
	file.ignore(4);
	GetFormat(file);
	try {
		ReadWaveData(file);
	} catch (std::string&) {
		debug("'fmt ' チャンクの直後が 'data' チャンクではありません");
		file.seekg(-4, std::ios_base::cur);
		if (!checkTag(file, "fact")) throw std::string("factチャンクがないです");
		debug("'fact' チャンクを無視します");
		std::uint32_t fact_size; file.read(reinterpret_cast<char*>(&fact_size), 4);
		file.ignore(fact_size);
		ReadWaveData(file);
	}
}

sound::WaveData::WaveData(IXAudio2** Engine, const std::string& filename, bool looped) {
	HRESULT hr;
	Prepare(filename);
	std::memset(&bufInfo, 0, sizeof(bufInfo));
	bufInfo.AudioBytes = buffer.size();
	bufInfo.pAudioData = reinterpret_cast<BYTE*>(&buffer[0]);
	bufInfo.LoopCount = (looped ? XAUDIO2_LOOP_INFINITE : 0);
	if (FAILED(hr = (*Engine)->CreateSourceVoice(&voice, &format))) {
		std::ostringstream o; o << "CreateSourceVoice失敗！！ (0x" <<
			std::hex << std::setw(8) << std::setfill('0') << hr << ")";
		throw o.str();
	}
}

/* 再生 */
void sound::WaveData::Play() {
	Stop();
	HRESULT hr;
	if (FAILED(hr = voice->SubmitSourceBuffer(&bufInfo))) {
		std::ostringstream o; o << "SubmitSourceBuffer失敗！！ (0x" <<
			std::hex << std::setw(8) << std::setfill('0') << hr << ")";
		throw o.str();
	}
	if (FAILED(hr = voice->Start(0, XAUDIO2_COMMIT_NOW))) {
		std::ostringstream o; o << "Start失敗！！ (0x" <<
			std::hex << std::setw(8) << std::setfill('0') << hr << ")";
		throw o.str();
	}
}

/* 停止 */
void sound::WaveData::Stop() {
	HRESULT hr;
	if (FAILED(hr = voice->Stop())) {
		std::ostringstream o; o << "Stop失敗！！ (0x" <<
			std::hex << std::setw(8) << std::setfill('0') << hr << ")";
		throw o.str();
	}
	if (FAILED(hr = voice->FlushSourceBuffers())) {
		std::ostringstream o; o << "FlushSourceBuffers失敗！！ (0x" <<
			std::hex << std::setw(8) << std::setfill('0') << hr << ")";
		throw o.str();
	}
}

/* デストラクタ */
sound::SoundData::~SoundData() {
	if (voice) {
		voice->Stop();
		voice->DestroyVoice();
	}
}

/* コンストラクタ(スーパークラス) */
sound::SoundData::SoundData() {
	voice = nullptr;
}
