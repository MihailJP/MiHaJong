#include "sound.h"

sound::SoundManipulator* sound::soundManipulator = nullptr;

SOUNDDLL_EXPORT int sound::Initialize() try {
	soundManipulator = new SoundManipulator();
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
	return 1;
}
