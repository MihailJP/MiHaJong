#pragma once

#ifdef _WIN32
#include <windows.h>
#endif
#if defined(_WIN32) && defined(WITH_DIRECTX)
#ifdef SOUND_EXPORTS
#include <dsound.h>
#endif /* SOUND_EXPORTS */
#endif /* defined(_WIN32) && defined(WITH_DIRECTX) */

#if !defined(_WIN32)
#define SOUNDDLL_EXPORT /* */
#elif defined(SOUND_EXPORTS)
#define SOUNDDLL_EXPORT __declspec(dllexport)
#else
#define SOUNDDLL_EXPORT __declspec(dllimport)
#endif /* SOUND_EXPORTS */

#ifdef SOUND_EXPORTS
#include "soundobj.h"
#endif /* SOUND_EXPORTS */

namespace sound {

#ifdef SOUND_EXPORTS
	extern SoundManipulator* soundManipulator;
#endif

	SOUNDDLL_EXPORT int Initialize(); // èâä˙âª
#ifdef _WIN32
	SOUNDDLL_EXPORT int Initialize(HWND hWnd); // èâä˙âª
#else /* _WIN32 */
	SOUNDDLL_EXPORT int Initialize(void*); // èâä˙âª
#endif /* _WIN32 */
	SOUNDDLL_EXPORT void Cleanup(); // å„énññ

	SOUNDDLL_EXPORT int LoadWave(unsigned ID, const char* filename, int looped); // WAVEì«Ç›çûÇ›
	SOUNDDLL_EXPORT int LoadVorbis(unsigned ID, const char* filename, int looped); // Vorbisì«Ç›çûÇ›
	SOUNDDLL_EXPORT int LoadMidi(unsigned ID, const char* filename, int looped); // MIDIì«Ç›çûÇ›

	SOUNDDLL_EXPORT int Play(unsigned ID); // çƒê∂
	SOUNDDLL_EXPORT int Stop(unsigned ID); // í‚é~

	SOUNDDLL_EXPORT int SetVolume(unsigned ID, double volume); // âπó ê›íË
}
