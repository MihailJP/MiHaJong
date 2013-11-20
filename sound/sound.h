#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <X11/Xlib.h>
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

	SOUNDDLL_EXPORT int Initialize(); // 初期化
#ifdef _WIN32
	SOUNDDLL_EXPORT int Initialize(HWND hWnd); // 初期化
#else /* _WIN32 */
	SOUNDDLL_EXPORT int Initialize(Window hWnd); // 初期化
#endif /* _WIN32 */
	SOUNDDLL_EXPORT void Cleanup(); // 後始末

	SOUNDDLL_EXPORT int LoadWave(unsigned ID, const char* filename, int looped); // WAVE読み込み
	SOUNDDLL_EXPORT int LoadVorbis(unsigned ID, const char* filename, int looped); // Vorbis読み込み
	SOUNDDLL_EXPORT int LoadMidi(unsigned ID, const char* filename, int looped); // MIDI読み込み

	SOUNDDLL_EXPORT int Play(unsigned ID); // 再生
	SOUNDDLL_EXPORT int Stop(unsigned ID); // 停止

	SOUNDDLL_EXPORT int SetVolume(unsigned ID, double volume); // 音量設定
}
