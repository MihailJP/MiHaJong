#include "soundobj.h"
#if defined(MIDI_SUPPORT) && defined(_WIN32)
#include "GuruGuruSMF/GuruGuruSMF4_Cpp.h"
#endif
#include <string>

void sound::SoundManipulator::InitXAudio() {
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		throw std::string("CoInitializeEx���s�I�I");

	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif
	if (FAILED(XAudio2Create(&xAudio, flags)))
		throw std::string("XAudio2Create���s�I�I");

	if (FAILED(xAudio->CreateMasteringVoice(&mVoice)))
		throw std::string("CreateMasteringVoice���s�I�I");
}

sound::SoundManipulator::SoundManipulator() {
	InitXAudio();
#if defined(MIDI_SUPPORT) && defined(_WIN32)
	if (GGSINITIALIZE() != GuruGuruSmf::GgsError::NoError)
		throw std::string("GGSINITIALIZE���s�I�I");
	if (GGS->OpenDevice(GuruGuruSmf::Device::DirectMusic, nullptr))
		throw std::string("GGS->OpenDevice���s�I�I");
#endif
}
sound::SoundManipulator::SoundManipulator(HWND hWnd) {
	InitXAudio();
#if defined(MIDI_SUPPORT) && defined(_WIN32)
	if (GGSINITIALIZE() != GuruGuruSmf::GgsError::NoError)
		throw std::string("GGSINITIALIZE���s�I�I");
	if (GGS->OpenDevice(GuruGuruSmf::Device::DirectMusic, hWnd))
		throw std::string("GGS->OpenDevice���s�I�I");
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

/* �t�@�C���ǂݍ��� */
void sound::SoundManipulator::readWaveData(unsigned ID, const std::string& filename, bool looped) {
	if (sounds.size() <= ID) sounds.resize(ID + 1, nullptr); // �z����g��
	sounds[ID] = new WaveData(&xAudio, filename, looped);
}
void sound::SoundManipulator::readVorbisData(unsigned ID, const std::string& filename, bool looped) {
	if (sounds.size() <= ID) sounds.resize(ID + 1, nullptr); // �z����g��
	sounds[ID] = new OggData(&xAudio, filename, looped);
}
void sound::SoundManipulator::readMidiData(unsigned ID, const std::string& filename, bool looped) {
	if (sounds.size() <= ID) sounds.resize(ID + 1, nullptr); // �z����g��
	sounds[ID] = new MidiData(ID, filename, looped);
}

/* �Đ� */
void sound::SoundManipulator::play(unsigned ID) {
	if ((sounds.size() <= ID) || (!sounds[ID])) throw std::string("�T�E���h���ǂݍ��܂�ĂȂ��ł�");
	sounds[ID]->Play();
}

/* ��~ */
void sound::SoundManipulator::stop(unsigned ID) {
	if ((sounds.size() <= ID) || (!sounds[ID])) throw std::string("�T�E���h���ǂݍ��܂�ĂȂ��ł�");
	sounds[ID]->Stop();
}
