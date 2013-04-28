#include "sound.h"
#include "logger.h"
#include <string>
#include <sstream>
#include "../common/strcode.h"

sound::SoundManipulator* sound::soundManipulator = nullptr;

SOUNDDLL_EXPORT int sound::Initialize() try {
	soundManipulator = new SoundManipulator();
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}

SOUNDDLL_EXPORT int sound::Initialize(HWND hWnd) try {
	soundManipulator = new SoundManipulator(hWnd);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}

SOUNDDLL_EXPORT void sound::Cleanup() {
	delete soundManipulator;
	soundManipulator = nullptr;
}

/* �T�E���h�f�[�^�ǂݍ��� */
SOUNDDLL_EXPORT int sound::LoadWave(unsigned ID, LPCSTR filename, int looped) try {
	{
		CodeConv::tostringstream o; o << _T("WAVE �t�@�C���ǂݍ��� ID [") << ID << _T("] �t�@�C���� [") << filename << _T("]");
		debug(o.str().c_str());
	}
	if (!soundManipulator) throw CodeConv::tstring(_T("����������Ă��܂���I�I�I"));
	soundManipulator->readWaveData(ID, filename, looped);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}
SOUNDDLL_EXPORT int sound::LoadVorbis(unsigned ID, LPCSTR filename, int looped) try {
	{
		CodeConv::tostringstream o; o << _T("Ogg Vorbis �t�@�C���ǂݍ��� ID [") << ID << _T("] �t�@�C���� [") << filename << _T("]");
		debug(o.str().c_str());
	}
	if (!soundManipulator) throw CodeConv::tstring(_T("����������Ă��܂���I�I�I"));
	soundManipulator->readVorbisData(ID, filename, looped);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}
SOUNDDLL_EXPORT int sound::LoadMidi(unsigned ID, LPCSTR filename, int looped) try {
	{
		CodeConv::tostringstream o; o << _T("MIDI �t�@�C���ǂݍ��� ID [") << ID << _T("] �t�@�C���� [") << filename << _T("]");
		debug(o.str().c_str());
	}
	if (!soundManipulator) throw CodeConv::tstring(_T("����������Ă��܂���I�I�I"));
	soundManipulator->readMidiData(ID, filename, looped);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}

/* �T�E���h�Đ� */
SOUNDDLL_EXPORT int sound::Play(unsigned ID) try {
	{
		CodeConv::tostringstream o; o << _T("�T�E���h�Đ� ID [") << ID << _T("]");
		debug(o.str().c_str());
	}
	if (!soundManipulator) throw CodeConv::tstring(_T("����������Ă��܂���I�I�I"));
	soundManipulator->play(ID);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}

/* �T�E���h��~ */
SOUNDDLL_EXPORT int sound::Stop(unsigned ID) try {
	if (!soundManipulator) throw CodeConv::tstring(_T("����������Ă��܂���I�I�I"));
	soundManipulator->stop(ID);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}

/* ���ʐݒ� */
SOUNDDLL_EXPORT int sound::SetVolume(unsigned ID, double volume) try {
	if (!soundManipulator) throw CodeConv::tstring(_T("����������Ă��܂���I�I�I"));
	soundManipulator->setVolume(ID, volume);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}