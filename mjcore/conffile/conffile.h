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

enum ScreenConfig : int {
	screenInvalid = -1,
	screenSVGA,   //  800 x  600
	screenXGA,    // 1024 x  768
	screenFWXGA,  // 1366 x  768
	screenSXGA,   // 1280 x 1024
	screenUXGA,   // 1600 x 1200
	screenFullHD, // 1920 x 1080
	screenWUXGA,  // 1920 x 1200
	screenDefaultRes = screenXGA,
};

class ConfigFile {
private:
	INIParser::IniMapMap configMap;

	std::string confPath();
	void load();
public:
	const std::string preferenceFile;
	ConfigFile();
	ConfigFile(const ConfigFile&) = delete;
	ConfigFile& operator=(const ConfigFile&) = delete;
	void save();
public:
	CodeConv::tstring playerName(); void playerName(const CodeConv::tstring&);
	ScreenMode::ScreenMode scrMode(); void scrMode(ScreenMode::ScreenMode);
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
