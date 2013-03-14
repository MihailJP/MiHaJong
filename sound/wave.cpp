#include "snddata.h"
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
	if (id == 1) format.wFormatTag = WAVE_FORMAT_PCM;
#if defined(USE_XAUDIO2)
	else if (id == 2) format.wFormatTag = WAVE_FORMAT_ADPCM;
#endif
	else throw CodeConv::tstring(_T("対応していないフォーマットです"));
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
#if defined(USE_XAUDIO2)
	std::memset(&bufInfo, 0, sizeof(buffer));
#endif
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

/* バッファの準備 */
#if defined(USE_XAUDIO2)
void sound::SoundData::PrepareBuffer(IXAudio2** Engine, bool looped) {
	HRESULT hr;
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
#else
void sound::SoundData::PrepareBuffer(LPDIRECTSOUND8* Engine, bool looped) {
	HRESULT hr;
	withLoop = looped;
	DSBUFFERDESC dsbd;
	memset(&dsbd, 0, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = 0;
	dsbd.dwBufferBytes = buffer.size();
	dsbd.lpwfxFormat = &format;
	dsbd.guid3DAlgorithm = GUID_NULL;
	if (FAILED(hr = (*Engine)->CreateSoundBuffer(&dsbd, &voice, nullptr))) {
		CodeConv::tostringstream o; o << _T("CreateSoundBuffer失敗！！ (0x") <<
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}
}
#endif

#if defined(USE_XAUDIO2)
sound::WaveData::WaveData(IXAudio2** Engine, const std::string& filename, bool looped) {
#else
sound::WaveData::WaveData(LPDIRECTSOUND8* Engine, const std::string& filename, bool looped) {
#endif
	Prepare(filename);
	PrepareBuffer(Engine, looped);
}

/* 再生 */
void sound::SoundData::Play() {
	Stop();
	HRESULT hr;
#if defined(USE_XAUDIO2)
	if (FAILED(hr = voice->SubmitSourceBuffer(&bufInfo))) {
		CodeConv::tostringstream o; o << _T("SubmitSourceBuffer失敗！！ (0x") <<
#else
	void* writePtr = nullptr; DWORD bufLen = 0;
	if (FAILED(hr = voice->Lock(0, 0, &writePtr, &bufLen, nullptr, nullptr, DSBLOCK_ENTIREBUFFER))) {
		CodeConv::tostringstream o; o << _T("Lock失敗！！ (0x") <<
#endif
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}
#if !defined(USE_XAUDIO2)
	else { // 書き込み
		memcpy(writePtr, &buffer[0], bufLen);
		voice->Unlock(writePtr, bufLen, nullptr, 0);
	}
#endif
#if defined(USE_XAUDIO2)
	if (FAILED(hr = voice->Start(0, XAUDIO2_COMMIT_NOW))) {
		CodeConv::tostringstream o; o << _T("Start失敗！！ (0x") <<
#else
	if (FAILED(hr = voice->Play(0, 0, withLoop ? DSBPLAY_LOOPING : 0))) {
		CodeConv::tostringstream o; o << _T("Play失敗！！ (0x") <<
#endif
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}
}

/* 停止 */
void sound::SoundData::Stop() {
	HRESULT hr;
	if (FAILED(hr = voice->Stop())) {
		CodeConv::tostringstream o; o << _T("Stop失敗！！ (0x") <<
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}
#if defined(USE_XAUDIO2)
	if (FAILED(hr = voice->FlushSourceBuffers())) {
		CodeConv::tostringstream o; o << _T("FlushSourceBuffers失敗！！ (0x") <<
#else
	if (FAILED(hr = voice->SetCurrentPosition(0))) {
		CodeConv::tostringstream o; o << _T("SetCurrentPosition失敗！！ (0x") <<
#endif
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}
}

/* デストラクタ */
sound::SoundData::~SoundData() {
	if (voice) {
		voice->Stop();
#if defined(USE_XAUDIO2)
		voice->DestroyVoice();
#else
		voice->Release();
#endif
	}
}

/* コンストラクタ(スーパークラス) */
sound::SoundData::SoundData() {
	voice = nullptr;
}
