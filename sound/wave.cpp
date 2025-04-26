#include "wave.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include "logger.h"
#include "../common/strcode.h"

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
	if (!checkTag(file, "WAVE")) throw CodeConv::tstring(_T("WAVEチャンクがないです"));
	if (!checkTag(file, "fmt ")) throw CodeConv::tstring(_T("fmt チャンクがないです"));
	// ヘッダサイズ
	std::uint32_t format_size; file.read(reinterpret_cast<char*>(&format_size), 4);
	// データフォーマット
	std::uint16_t id; file.read(reinterpret_cast<char*>(&id), 2);
#ifdef _WIN32
	if (id == 1) format.wFormatTag = WAVE_FORMAT_PCM;
#ifdef WAVE_FORMAT_ADPCM
	else if (id == 2) format.wFormatTag = WAVE_FORMAT_ADPCM;
#endif /* WAVE_FORMAT_ADPCM */
	else throw CodeConv::tstring(_T("対応していないフォーマットです"));
#else /* _WIN32 */
	if (id != 1)
		throw CodeConv::tstring(_T("対応していないフォーマットです"));
#endif /* _WIN32 */
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
	if (!checkTag(file, "data")) throw CodeConv::tstring(_T("dataチャンクがないです"));
	std::uint32_t size; file.read(reinterpret_cast<char*>(&size), 4);
	buffer.resize(size);
	// 読み込み
	file.read(&buffer.front(), size);
}

/* WAVEファイル読み込み */
void sound::WaveData::Prepare(const std::string& filename) {
	std::memset(&format, 0, sizeof(format));
#ifdef USE_XAUDIO2
	std::memset(&bufInfo, 0, sizeof(buffer));
#endif /* USE_XAUDIO2 */
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	if (!file) throw CodeConv::tstring(_T("ファイルを開けませんでした"));
	if (!checkTag(file, "RIFF")) throw CodeConv::tstring(_T("RIFFチャンクがないです"));
	file.ignore(4);
	GetFormat(file);
	try {
		ReadWaveData(file);
	} catch (CodeConv::tstring&) {
		debug(_T("'fmt ' チャンクの直後が 'data' チャンクではありません"));
		file.seekg(-4, std::ios_base::cur);
		if (!checkTag(file, "fact")) throw CodeConv::tstring(_T("factチャンクがないです"));
		debug(_T("'fact' チャンクを無視します"));
		std::uint32_t fact_size; file.read(reinterpret_cast<char*>(&fact_size), 4);
		file.ignore(fact_size);
		ReadWaveData(file);
	}
}

#ifdef USE_XAUDIO2
sound::WaveData::WaveData(IXAudio2** Engine, const std::string& filename, bool looped) {
#else /* USE_XAUDIO2 */
sound::WaveData::WaveData(void* Engine, const std::string& filename, bool looped) {
#endif /* USE_XAUDIO2 */
	Prepare(filename);
	PrepareBuffer(Engine, looped);
}
