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

	SOUNDDLL_EXPORT int Initialize(); // èâä˙âª
	SOUNDDLL_EXPORT int Initialize(HWND hWnd); // èâä˙âª
	SOUNDDLL_EXPORT void Cleanup(); // å„énññ

	SOUNDDLL_EXPORT int LoadWave(unsigned ID, LPCSTR filename, int looped); // WAVEì«Ç›çûÇ›
	SOUNDDLL_EXPORT int LoadVorbis(unsigned ID, LPCSTR filename, int looped); // Vorbisì«Ç›çûÇ›
	SOUNDDLL_EXPORT int LoadMidi(unsigned ID, LPCSTR filename, int looped); // MIDIì«Ç›çûÇ›

	SOUNDDLL_EXPORT int Play(unsigned ID); // çƒê∂
	SOUNDDLL_EXPORT int Stop(unsigned ID); // í‚é~

	SOUNDDLL_EXPORT int SetVolume(unsigned ID, double volume); // âπó ê›íË
}
