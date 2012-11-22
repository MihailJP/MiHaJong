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

/* �T�E���h�f�[�^�ǂݍ��� */
SOUNDDLL_EXPORT int sound::LoadWave(unsigned ID, LPCSTR filename) try {
	if (!soundManipulator) throw "����������Ă��܂���I�I�I";
	soundManipulator->readWaveData(ID, filename);
	return 0;
} catch (...) {
	return -1;
}

/* �T�E���h�Đ� */
SOUNDDLL_EXPORT int sound::Play(unsigned ID) try {
	if (!soundManipulator) throw "����������Ă��܂���I�I�I";
	soundManipulator->play(ID);
	return 0;
} catch (...) {
	return -1;
}

/* �T�E���h��~ */
SOUNDDLL_EXPORT int sound::Stop(unsigned ID) try {
	if (!soundManipulator) throw "����������Ă��܂���I�I�I";
	soundManipulator->stop(ID);
	return 0;
} catch (...) {
	return -1;
}
