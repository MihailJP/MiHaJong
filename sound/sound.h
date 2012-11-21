#ifndef SOUND_DLL_SOUND_H
#define SOUND_DLL_SOUND_H

#include <Windows.h>
#include <dsound.h>
#include "soundobj.h"

#ifdef SOUND_EXPORTS
#define SOUNDDLL_EXPORT __declspec(dllexport)
#else
#define SOUNDDLL_EXPORT __declspec(dllimport)
#endif

namespace sound {

#ifdef SOUND_EXPORTS
	extern SoundManipulator* soundManipulator;
#endif

	SOUNDDLL_EXPORT int Initialize();
	SOUNDDLL_EXPORT void Cleanup();
}

#endif
