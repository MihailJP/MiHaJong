#include "soundobj.h"
#include "midi.h"
#if defined(MIDI_SUPPORT) && defined(_WIN32)
#include "GuruGuruSMF/GuruGuruSMF4_Cpp.h"
#endif
#include <sstream>
#include <iomanip>
#include "../common/strcode.h"

#ifdef _WIN32
void sound::SoundManipulator::InitXAudio(HWND hWnd)
#else /* _WIN32 */
void sound::SoundManipulator::InitXAudio(Window hWnd)
#endif /* _WIN32 */
{
#if defined(_WIN32) && defined(WITH_DIRECTX)
	HRESULT hr;

	/* COM初期化 */
	if (FAILED(hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))) {
		CodeConv::tostringstream o; o << _T("CoInitializeEx失敗！！ (0x") <<
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}

	/* デバイスの初期化 */
#if defined(USE_XAUDIO2)
	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif
	if (FAILED(hr = XAudio2Create(&xAudio, flags))) {
		CodeConv::tostringstream o; o << _T("XAudio2Create失敗！！ (0x") <<
#else
	if (FAILED(hr = DirectSoundCreate8(nullptr, &pDSound, nullptr))) {
		CodeConv::tostringstream o; o << _T("DirectSoundCreate8失敗！！ (0x") <<
#endif
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}
#if !defined(USE_XAUDIO2)
	pDSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
#endif

	/* マスターバッファ初期化 */
#if defined(USE_XAUDIO2)
	if (FAILED(hr = xAudio->CreateMasteringVoice(&mVoice))) {
		CodeConv::tostringstream o; o << _T("CreateMasteringVoice失敗！！ (0x") <<
#else
	DSBUFFERDESC dsbd;
	memset(&dsbd, 0, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsbd.dwBufferBytes = 0;
	if (FAILED(hr = pDSound->CreateSoundBuffer(&dsbd, &mVoice, nullptr))) {
		CodeConv::tostringstream o; o << _T("CreateMasteringVoice失敗！！ (0x") <<
#endif
			std::hex << std::setw(8) << std::setfill(_T('0')) << hr << _T(")");
		throw o.str();
	}
#else
	/* OpenAL 初期化 */
	myDevice = alcOpenDevice(nullptr);
	if (!myDevice) throw _T("alcOpenDevice失敗！！");

	myContext = alcCreateContext(myDevice, nullptr);
	if (!myContext) throw _T("alcCreateContext失敗！！");
	alcMakeContextCurrent(myContext);
#endif /* defined(_WIN32) && defined(WITH_DIRECTX) */
}

sound::SoundManipulator::SoundManipulator() {
	mVoice = nullptr;
	xAudio = nullptr;
	InitXAudio();
#if defined(MIDI_SUPPORT) && defined(_WIN32)
	if (GGSINITIALIZE() != GuruGuruSmf::GgsError::NoError)
		throw CodeConv::tstring(_T("GGSINITIALIZE失敗！！"));
	if (GGS->OpenDevice(GuruGuruSmf::Device::DirectMusic, nullptr))
		throw CodeConv::tstring(_T("GGS->OpenDevice失敗！！"));
#endif
}
#ifdef _WIN32
sound::SoundManipulator::SoundManipulator(HWND hWnd)
#else /* _WIN32 */
sound::SoundManipulator::SoundManipulator(Window hWnd)
#endif /* _WIN32 */
{
	mVoice = nullptr;
	xAudio = nullptr;
	InitXAudio(hWnd);
#if defined(MIDI_SUPPORT) && defined(_WIN32)
	if (GGSINITIALIZE() != GuruGuruSmf::GgsError::NoError)
		throw CodeConv::tstring(_T("GGSINITIALIZE失敗！！"));
	if (GGS->OpenDevice(GuruGuruSmf::Device::DirectMusic, hWnd))
		throw CodeConv::tstring(_T("GGS->OpenDevice失敗！！"));
#endif
}

sound::SoundManipulator::~SoundManipulator() {
#if defined(MIDI_SUPPORT) && defined(_WIN32)
	GGS->CloseDevice();
	GGSFREE();
#endif
	for (auto& k : sounds) {
		delete k; k = nullptr;
	}
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
	if (myDevice) {
		alcCloseDevice(myDevice); myDevice = nullptr;
	}
	if (myContext) {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(myContext); myContext = nullptr;
	}
#elif defined(USE_XAUDIO2)
	if (mVoice) {
		mVoice->DestroyVoice(); mVoice = nullptr;
	}
	if (xAudio) {
		xAudio->Release(); xAudio = nullptr;
	}
	CoUninitialize();
#else
	if (mVoice) {
		mVoice->Release(); mVoice = nullptr;
	}
	if (pDSound) {
		pDSound->Release(); pDSound = nullptr;
	}
	CoUninitialize();
#endif
}

/* ファイル読み込み */
void sound::SoundManipulator::readWaveData(unsigned ID, const std::string& filename, bool looped) {
	if (sounds.size() <= ID) sounds.resize(ID + 1, nullptr); // 配列を拡張
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
	sounds[ID] = new WaveData(nullptr, filename, looped);
#elif defined(USE_XAUDIO2)
	sounds[ID] = new WaveData(&xAudio, filename, looped);
#else
	sounds[ID] = new WaveData(&pDSound, filename, looped);
#endif
}
void sound::SoundManipulator::readVorbisData(unsigned ID, const std::string& filename, bool looped) {
	if (sounds.size() <= ID) sounds.resize(ID + 1, nullptr); // 配列を拡張
#if !defined(_WIN32) || !defined(WITH_DIRECTX)
	sounds[ID] = new OggData(nullptr, filename, looped);
#elif defined(USE_XAUDIO2)
	sounds[ID] = new OggData(&xAudio, filename, looped);
#else
	sounds[ID] = new OggData(&pDSound, filename, looped);
#endif
}
void sound::SoundManipulator::readMidiData(unsigned ID, const std::string& filename, bool looped) {
	if (sounds.size() <= ID) sounds.resize(ID + 1, nullptr); // 配列を拡張
	sounds[ID] = new MidiData(ID, filename, looped);
}

/* 再生 */
void sound::SoundManipulator::play(unsigned ID) {
	if ((sounds.size() <= ID) || (!sounds[ID])) {
		CodeConv::tostringstream o; o << _T("サウンド ID [") << ID << _T("] は読み込まれてないです");
		throw o.str();
	}
	sounds[ID]->Play();
}

/* 停止 */
void sound::SoundManipulator::stop(unsigned ID) {
	if ((sounds.size() <= ID) || (!sounds[ID])) {
		CodeConv::tostringstream o; o << _T("サウンド ID [") << ID << _T("] は読み込まれてないです");
		throw o.str();
	}
	sounds[ID]->Stop();
}

/* 音量設定 */
void sound::SoundManipulator::setVolume(unsigned ID, double volume) {
	if ((sounds.size() <= ID) || (!sounds[ID])) {
		CodeConv::tostringstream o; o << _T("サウンド ID [") << ID << _T("] は読み込まれてないです");
		throw o.str();
	}
	sounds[ID]->setVolume(volume);
}
