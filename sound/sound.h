#pragma once

#include <Windows.h>
#ifdef SOUND_EXPORTS
#include <dsound.h>

#define SOUNDDLL_EXPORT __declspec(dllexport)
#include "soundobj.h"
#else
#define SOUNDDLL_EXPORT __declspec(dllimport)
#endif

namespace sound {

#ifdef SOUND_EXPORTS
	extern SoundManipulator* soundManipulator;
#endif

	SOUNDDLL_EXPORT int Initialize(); // ������
	SOUNDDLL_EXPORT int Initialize(HWND hWnd); // ������
	SOUNDDLL_EXPORT void Cleanup(); // ��n��

	SOUNDDLL_EXPORT int LoadWave(unsigned ID, LPCSTR filename, int looped); // WAVE�ǂݍ���
	SOUNDDLL_EXPORT int LoadVorbis(unsigned ID, LPCSTR filename, int looped); // Vorbis�ǂݍ���
	SOUNDDLL_EXPORT int LoadMidi(unsigned ID, LPCSTR filename, int looped); // MIDI�ǂݍ���

	SOUNDDLL_EXPORT int Play(unsigned ID); // �Đ�
	SOUNDDLL_EXPORT int Stop(unsigned ID); // ��~

	SOUNDDLL_EXPORT int SetVolume(unsigned ID, double volume); // ���ʐݒ�
}
