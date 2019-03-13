#include "sound.h"
#include "logger.h"
#include <string>
#include <sstream>
#include "../common/strcode.h"
#include "GuruGuruSMF/GuruGuruSMF4_Cpp.h"

sound::SoundManipulator* sound::soundManipulator = nullptr;

namespace {

#if defined(_WIN32) && defined(MIDI_SUPPORT)
int deviceID(const CodeConv::tstring& deviceName) {
	const UINT numOfDevs(midiOutGetNumDevs());
	MIDIOUTCAPS midiOutCaps{};

	for (int i = 0; i < numOfDevs; ++i) {
		if (midiOutGetDevCaps(i, &midiOutCaps, sizeof midiOutCaps) == MMSYSERR_NOERROR) {
			if (deviceName == CodeConv::lower(midiOutCaps.szPname)) return i;
		}
	}

	return GuruGuruSmf::Device::DirectMusic;
}
#endif /* defined(_WIN32) && defined(MIDI_SUPPORT) */

}

SOUNDDLL_EXPORT int sound::Initialize(LPCTSTR midiDev) try {
	const CodeConv::tstring devName(midiDev ? midiDev : _T(""));
	const auto dev(deviceID(devName));
	soundManipulator = new SoundManipulator(dev);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}

#ifdef _WIN32
SOUNDDLL_EXPORT int sound::Initialize(HWND hWnd, LPCTSTR midiDev)
#else /* _WIN32 */
SOUNDDLL_EXPORT int sound::Initialize(Window hWnd, LPCTSTR midiDev)
#endif /* _WIN32 */
try {
	const CodeConv::tstring devName(midiDev ? midiDev : _T(""));
	const auto dev(deviceID(devName));
	soundManipulator = new SoundManipulator(dev, hWnd);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}

SOUNDDLL_EXPORT void sound::Cleanup() {
	delete soundManipulator;
	soundManipulator = nullptr;
}

/* サウンドデータ読み込み */
SOUNDDLL_EXPORT int sound::LoadWave(unsigned ID, const char* filename, int looped) try {
	{
		CodeConv::tostringstream o; o << _T("WAVE ファイル読み込み ID [") << ID << _T("] ファイル名 [") << filename << _T("]");
		debug(o.str().c_str());
	}
	if (!soundManipulator) throw CodeConv::tstring(_T("初期化されていません！！！"));
	soundManipulator->readWaveData(ID, filename, looped);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}
SOUNDDLL_EXPORT int sound::LoadVorbis(unsigned ID, const char* filename, int looped) try {
	{
		CodeConv::tostringstream o; o << _T("Ogg Vorbis ファイル読み込み ID [") << ID << _T("] ファイル名 [") << filename << _T("]");
		debug(o.str().c_str());
	}
	if (!soundManipulator) throw CodeConv::tstring(_T("初期化されていません！！！"));
	soundManipulator->readVorbisData(ID, filename, looped);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}
SOUNDDLL_EXPORT int sound::LoadMidi(unsigned ID, const char* filename, int looped) try {
	{
		CodeConv::tostringstream o; o << _T("MIDI ファイル読み込み ID [") << ID << _T("] ファイル名 [") << filename << _T("]");
		debug(o.str().c_str());
	}
	if (!soundManipulator) throw CodeConv::tstring(_T("初期化されていません！！！"));
	soundManipulator->readMidiData(ID, filename, looped);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}

/* サウンド再生 */
SOUNDDLL_EXPORT int sound::Play(unsigned ID) try {
	{
		CodeConv::tostringstream o; o << _T("サウンド再生 ID [") << ID << _T("]");
		debug(o.str().c_str());
	}
	if (!soundManipulator) throw CodeConv::tstring(_T("初期化されていません！！！"));
	soundManipulator->play(ID);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}

/* サウンド停止 */
SOUNDDLL_EXPORT int sound::Stop(unsigned ID) try {
	if (!soundManipulator) throw CodeConv::tstring(_T("初期化されていません！！！"));
	soundManipulator->stop(ID);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}

/* 音量設定 */
SOUNDDLL_EXPORT int sound::SetVolume(unsigned ID, double volume) try {
	if (!soundManipulator) throw CodeConv::tstring(_T("初期化されていません！！！"));
	soundManipulator->setVolume(ID, volume);
	return 0;
} catch (CodeConv::tstring& e) {
	error(e.c_str());
	return -1;
}