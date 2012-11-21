#include "soundobj.h"

sound::SoundManipulator::SoundManipulator() {
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		throw "CoInitializeEx失敗！！";

	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif
	if (FAILED(XAudio2Create(&xAudio, flags)))
		throw "XAudio2Create失敗！！";

	if (FAILED(xAudio->CreateMasteringVoice(&mVoice)))
		throw "CreateMasteringVoice失敗！！";
}

sound::SoundManipulator::~SoundManipulator() {
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
void sound::SoundManipulator::readWaveData(unsigned ID, const std::string& filename) {
	if (sounds.size() <= ID) sounds.resize(ID + 1, nullptr); // 配列を拡張
	sounds[ID] = new WaveData(&xAudio, filename);
}

/* 再生 */
void sound::SoundManipulator::play(unsigned ID) {
	if ((sounds.size() <= ID) || (!sounds[ID])) throw "サウンドが読み込まれてないです";
	sounds[ID]->Play();
}

/* 停止 */
void sound::SoundManipulator::stop(unsigned ID) {
	if ((sounds.size() <= ID) || (!sounds[ID])) throw "サウンドが読み込まれてないです";
	sounds[ID]->Stop();
}
