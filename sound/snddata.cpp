#include "snddata.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "logger.h"
#include "../common/strcode.h"

/* バッファの準備 */
#ifdef USE_XAUDIO2
void sound::SoundData::PrepareBuffer(IXAudio2** Engine, bool looped) {
	HRESULT hr;
	std::memset(&bufInfo, 0, sizeof(bufInfo));
	bufInfo.AudioBytes = buffer.size();
	bufInfo.pAudioData = reinterpret_cast<BYTE*>(&buffer[0]);
	bufInfo.LoopCount = (looped ? XAUDIO2_LOOP_INFINITE : 0);
	bufInfo.LoopBegin = loopStart;
	bufInfo.LoopLength = loopLength;
	if (FAILED(hr = (*Engine)->CreateSourceVoice(&voice, &format))) {
		CodeConv::tostringstream o; o << _T("CreateSourceVoice失敗！！ (0x") <<
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}
}
#else /* USE_XAUDIO2 */
void sound::SoundData::PrepareBuffer(void* Engine, bool looped) {
	alGenBuffers(1, &myBuffer);
	ALenum bufFormat;
	if (format.nChannels == 1) // mono
		bufFormat = (format.wBitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
	else // stereo
		bufFormat = (format.wBitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
	alBufferData(myBuffer, bufFormat, &buffer[0], buffer.size(), format.nSamplesPerSec);
	alGenSources(1, &mySource);
	alSourcei(mySource, AL_BUFFER, myBuffer);
	alSourcei(mySource, AL_LOOPING, looped ? AL_TRUE : AL_FALSE);
}
#endif /* USE_XAUDIO2 */

/* 再生 */
void sound::SoundData::Play() {
	waitUntilLoaded();
	Stop();
#ifdef USE_XAUDIO2
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
#else /* USE_XAUDIO2 */
	alSourcePlay(mySource);
#endif /* USE_XAUDIO2 */
}

/* 停止 */
void sound::SoundData::Stop() {
	waitUntilLoaded();
#ifdef USE_XAUDIO2
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
#else /* USE_XAUDIO2 */
	alSourceStop(mySource);
	alSourceRewind(mySource);
#endif /* USE_XAUDIO2 */
}

/* 音量設定 */
void sound::SoundData::setVolume(double volume) {
	waitUntilLoaded();
#ifdef USE_XAUDIO2
	HRESULT hr;
	double ampvol;
	if (volume == 0.0)
		ampvol = 0.0;
	else
		ampvol = pow(10.0, (abs(volume) - 1.0) * 100.0 / 40.0);
	if (FAILED(hr = voice->SetVolume(static_cast<float>(ampvol)))) {
		CodeConv::tostringstream o; o << _T("SetVolume失敗！！ (0x") <<
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}
#else /* USE_XAUDIO2 */
	alSourcef(mySource, AL_GAIN, volume);
#endif /* USE_XAUDIO2 */
}

/* デストラクタ */
sound::SoundData::~SoundData() {
	waitUntilLoaded();
#ifdef USE_XAUDIO2
	if (voice) {
		voice->Stop();
		voice->DestroyVoice();
	}
#else /* USE_XAUDIO2 */
	alDeleteSources(1, &mySource);
	alDeleteBuffers(1, &myBuffer);
#endif /* USE_XAUDIO2 */
}

/* コンストラクタ(スーパークラス) */
sound::SoundData::SoundData() {
#ifdef USE_XAUDIO2
	memset(&bufInfo, 0, sizeof(bufInfo));
	voice = nullptr;
#endif /* USE_XAUDIO2 */
}
