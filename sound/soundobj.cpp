#include "soundobj.h"
#if defined(MIDI_SUPPORT) && defined(_WIN32)
#include "GuruGuruSMF/GuruGuruSMF4_Cpp.h"
#endif
#include <sstream>
#include <iomanip>

void sound::SoundManipulator::InitXAudio() {
	HRESULT hr;
	if (FAILED(hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))) {
		std::ostringstream o; o << "CoInitializeEx失敗！！ (0x" <<
			std::hex << std::setw(8) << std::setfill('0') << hr << ")";
		throw o.str();
	}

	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif
	if (FAILED(hr = XAudio2Create(&xAudio, flags))) {
		std::ostringstream o; o << "XAudio2Create失敗！！ (0x" <<
			std::hex << std::setw(8) << std::setfill('0') << hr << ")";
		throw o.str();
	}

	if (FAILED(hr = xAudio->CreateMasteringVoice(&mVoice))) {
		std::ostringstream o; o << "CreateMasteringVoice失敗！！ (0x" <<
			std::hex << std::setw(8) << std::setfill('0') << hr << ")";
		throw o.str();
	}
}

sound::SoundManipulator::SoundManipulator() {
	InitXAudio();
#if defined(MIDI_SUPPORT) && defined(_WIN32)
	if (GGSINITIALIZE() != GuruGuruSmf::GgsError::NoError)
		throw std::string("GGSINITIALIZE失敗！！");
	if (GGS->OpenDevice(GuruGuruSmf::Device::DirectMusic, nullptr))
		throw std::string("GGS->OpenDevice失敗！！");
#endif
}
sound::SoundManipulator::SoundManipulator(HWND hWnd) {
	InitXAudio();
#if defined(MIDI_SUPPORT) && defined(_WIN32)
	if (GGSINITIALIZE() != GuruGuruSmf::GgsError::NoError)
		throw std::string("GGSINITIALIZE失敗！！");
	if (GGS->OpenDevice(GuruGuruSmf::Device::DirectMusic, hWnd))
		throw std::string("GGS->OpenDevice失敗！！");
#endif
}

sound::SoundManipulator::~SoundManipulator() {
#if defined(MIDI_SUPPORT) && defined(_WIN32)
	GGS->CloseDevice();
	GGSFREE();
#endif
	for (auto k = sounds.begin(); k != sounds.end(); ++k) {
		delete (*k); (*k) = nullptr;
	}
	if (mVoice) {
		mVoice->DestroyVoice(); mVoice = nullptr;
	}
	if (xAudio) {
		xAudio->Release(); xAudio = nullptr;
	}
	CoUninitialize();
}

/* ファイル読み込み */
void sound::SoundManipulator::readWaveData(unsigned ID, const std::string& filename, bool looped) {
	if (sounds.size() <= ID) sounds.resize(ID + 1, nullptr); // 配列を拡張
	sounds[ID] = new WaveData(&xAudio, filename, looped);
}
void sound::SoundManipulator::readVorbisData(unsigned ID, const std::string& filename, bool looped) {
	if (sounds.size() <= ID) sounds.resize(ID + 1, nullptr); // 配列を拡張
	sounds[ID] = new OggData(&xAudio, filename, looped);
}
void sound::SoundManipulator::readMidiData(unsigned ID, const std::string& filename, bool looped) {
	if (sounds.size() <= ID) sounds.resize(ID + 1, nullptr); // 配列を拡張
	sounds[ID] = new MidiData(ID, filename, looped);
}

/* 再生 */
void sound::SoundManipulator::play(unsigned ID) {
	if ((sounds.size() <= ID) || (!sounds[ID])) throw std::string("サウンドが読み込まれてないです");
	sounds[ID]->Play();
}

/* 停止 */
void sound::SoundManipulator::stop(unsigned ID) {
	if ((sounds.size() <= ID) || (!sounds[ID])) throw std::string("サウンドが読み込まれてないです");
	sounds[ID]->Stop();
}
