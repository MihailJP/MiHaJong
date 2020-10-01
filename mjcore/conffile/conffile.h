#pragma once

#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#else /* _WIN32 */
#include <sys/stat.h>
#endif /* _WIN32 */
#include "../reader/ini2map.h"
#include "../../common/strcode.h"
#include "../../common/scrmode.h"

namespace ConfigFile {

using std::uint8_t;
using std::uint32_t;

#ifdef _WIN32
inline bool exist(LPCSTR filename) {
	return (GetFileAttributesA(filename) != -1);
}
#else /* _WIN32 */
inline bool exist(const char* filename) {
	struct stat s;
	return (stat(filename, &s) != -1);
}
#endif /* _WIN32 */

enum class ScreenConfig : int {
	invalid = -1,
	svga,   //  800 x  600
	xga,    // 1024 x  768
	fwxga,  // 1366 x  768
	sxga,   // 1280 x 1024
	uxga,   // 1600 x 1200
	fullHD, // 1920 x 1080
	wuxga,  // 1920 x 1200
#ifdef WITH_4K_PICS
	wqhd,   // 2560 x 1440
	uhd4K,  // 3840 x 2160
#endif /* WITH_4K_PICS */
	defaultRes = xga,
};

class ConfigFile {
private:
	INIParser::IniMapMap configMap;

	void load();
public:
	static std::string confPath();
	const std::string preferenceFile;
	ConfigFile();
	ConfigFile(const ConfigFile&) = delete;
	ConfigFile& operator=(const ConfigFile&) = delete;
	void save();
public:
	CodeConv::tstring playerName(); void playerName(const CodeConv::tstring&);
	Screen_Mode::ScreenMode scrMode(); void scrMode(Screen_Mode::ScreenMode);
	bool blackTile(); void blackTile(bool);
	unsigned int bgmVolume(); void bgmVolume(unsigned int);
	unsigned int soundVolume(); void soundVolume(unsigned int);
	unsigned int monitorNumber(); void monitorNumber(unsigned int);
	CodeConv::tstring midiDevice(); void midiDevice(const CodeConv::tstring&);
	ScreenConfig screenResolution(); void screenResolution(ScreenConfig);
	unsigned int screenResolutionX(); unsigned int screenResolutionY();
	CodeConv::tstring serverAddress(); void serverAddress(const CodeConv::tstring&);
};

}
