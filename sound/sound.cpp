#include "sound.h"
#include "logger.h"
#include <string>
#include <sstream>

sound::SoundManipulator* sound::soundManipulator = nullptr;

SOUNDDLL_EXPORT int sound::Initialize() try {
	soundManipulator = new SoundManipulator();
	return 0;
} catch (std::string& e) {
	error(e.c_str());
	return -1;
}

SOUNDDLL_EXPORT int sound::Initialize(HWND hWnd) try {
	soundManipulator = new SoundManipulator(hWnd);
	return 0;
} catch (std::string& e) {
	error(e.c_str());
	return -1;
}

SOUNDDLL_EXPORT void sound::Cleanup() {
	delete soundManipulator;
	soundManipulator = nullptr;
}

/* サウンドデータ読み込み */
SOUNDDLL_EXPORT int sound::LoadWave(unsigned ID, LPCSTR filename, int looped) try {
	{
		std::ostringstream o; o << "WAVE ファイル読み込み ID [" << ID << "] ファイル名 [" << filename << "]";
		debug(o.str().c_str());
	}
	if (!soundManipulator) throw std::string("初期化されていません！！！");
	soundManipulator->readWaveData(ID, filename, looped);
	return 0;
} catch (std::string& e) {
	error(e.c_str());
	return -1;
}
SOUNDDLL_EXPORT int sound::LoadVorbis(unsigned ID, LPCSTR filename, int looped) try {
	{
		std::ostringstream o; o << "Ogg Vorbis ファイル読み込み ID [" << ID << "] ファイル名 [" << filename << "]";
		debug(o.str().c_str());
	}
	if (!soundManipulator) throw std::string("初期化されていません！！！");
	soundManipulator->readVorbisData(ID, filename, looped);
	return 0;
} catch (std::string& e) {
	error(e.c_str());
	return -1;
}
SOUNDDLL_EXPORT int sound::LoadMidi(unsigned ID, LPCSTR filename, int looped) try {
	{
		std::ostringstream o; o << "MIDI ファイル読み込み ID [" << ID << "] ファイル名 [" << filename << "]";
		debug(o.str().c_str());
	}
	if (!soundManipulator) throw std::string("初期化されていません！！！");
	soundManipulator->readMidiData(ID, filename, looped);
	return 0;
} catch (std::string& e) {
	error(e.c_str());
	return -1;
}

/* サウンド再生 */
SOUNDDLL_EXPORT int sound::Play(unsigned ID) try {
	{
		std::ostringstream o; o << "サウンド再生 ID [" << ID << "]";
		debug(o.str().c_str());
	}
	if (!soundManipulator) throw std::string("初期化されていません！！！");
	soundManipulator->play(ID);
	return 0;
} catch (std::string& e) {
	error(e.c_str());
	return -1;
}

/* サウンド停止 */
SOUNDDLL_EXPORT int sound::Stop(unsigned ID) try {
	if (!soundManipulator) throw std::string("初期化されていません！！！");
	soundManipulator->stop(ID);
	return 0;
} catch (std::string& e) {
	error(e.c_str());
	return -1;
}
