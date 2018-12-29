#pragma once

#include <Windows.h>
#include <tchar.h>
#include "../reader/ini2map.h"

namespace ConfigFile {

using std::uint8_t;
using std::uint32_t;

inline bool exist(LPCSTR filename) {
	return (GetFileAttributesA(filename) != -1);
}

enum ScreenConfig : int {
	screenInvalid = -1,
	screenSVGA,   //  800 x  600
	screenXGA,    // 1024 x  768
	screenFWXGA,  // 1366 x  768
	screenSXGA,   // 1280 x 1024
	screenUXGA,   // 1600 x 1200
	screenFullHD, // 1920 x 1080
	screenWUXGA,  // 1920 x 1200
};

struct IPval {
	uint32_t val;
	IPval() : val(0u) {}
	IPval(uint32_t addressAsOneVal) : val(addressAsOneVal) {}
	IPval(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth)
		: val((uint32_t)first << 24 | (uint32_t)second << 16 |(uint32_t)third << 8 | (uint32_t)fourth) {}
	uint8_t first()  const {return (uint8_t)((val & 0xff000000) >> 24);}
	uint8_t second() const {return (uint8_t)((val & 0x00ff0000) >> 16);}
	uint8_t third()  const {return (uint8_t)((val & 0x0000ff00) >>  8);}
	uint8_t fourth() const {return (uint8_t)((val & 0x000000ff)      );}
	operator uint32_t() const {return val;}
};
static_assert(std::is_trivially_copyable<IPval>::value, "IPval is not trivially copyable");
static_assert(std::is_standard_layout<IPval>::value, "IPval is not standard layout");

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
	bool fullScreen(); void fullScreen(bool);
	bool blackTile(); void blackTile(bool);
	unsigned int bgmVolume(); void bgmVolume(unsigned int);
	unsigned int soundVolume(); void soundVolume(unsigned int);
	ScreenConfig screenResolution(); void screenResolution(ScreenConfig);
	unsigned int screenResolutionX(); unsigned int screenResolutionY();
	IPval serverAddress(); void serverAddress(IPval);
};

}
