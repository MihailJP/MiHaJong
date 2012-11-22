#include "sound.h"

sound::SoundManipulator* sound::soundManipulator = nullptr;

SOUNDDLL_EXPORT int sound::Initialize() try {
	soundManipulator = new SoundManipulator();
	return 0;
} catch (...) {
	return -1;
}

SOUNDDLL_EXPORT int sound::Initialize(HWND hWnd) try {
	soundManipulator = new SoundManipulator(hWnd);
	return 0;
} catch (...) {
	return -1;
}

SOUNDDLL_EXPORT void sound::Cleanup() {
	delete soundManipulator;
	soundManipulator = nullptr;
}

/* サウンドデータ読み込み */
SOUNDDLL_EXPORT int sound::LoadWave(unsigned ID, LPCSTR filename) try {
	if (!soundManipulator) throw "初期化されていません！！！";
	soundManipulator->readWaveData(ID, filename);
	return 0;
} catch (...) {
	return -1;
}

/* サウンド再生 */
SOUNDDLL_EXPORT int sound::Play(unsigned ID) try {
	if (!soundManipulator) throw "初期化されていません！！！";
	soundManipulator->play(ID);
	return 0;
} catch (...) {
	return -1;
}

/* サウンド停止 */
SOUNDDLL_EXPORT int sound::Stop(unsigned ID) try {
	if (!soundManipulator) throw "初期化されていません！！！";
	soundManipulator->stop(ID);
	return 0;
} catch (...) {
	return -1;
}
