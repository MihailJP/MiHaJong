#include "conffile.h"

#include <string>
#include "../../common/strcode.h"
#include "../../common/scrmode.h"
#include <fstream>
#include <regex>
#include <cassert>
#include <locale>

namespace ConfigFile {

typedef std::basic_regex<TCHAR> tregex;
CodeConv::tstring lower(const CodeConv::tstring& str) {
	CodeConv::tstring result;
	for (auto i = str.begin(); i != str.end(); ++i) {
		result += std::tolower(*i, std::locale::classic());
	}
	return result;
}

/* コンフィグのパス */
std::string ConfigFile::confPath() {
	std::string configpath = "";
	char* cur = new char[1024];
	GetCurrentDirectoryA(1024, cur);
	char* progfiles = new char[1024];
	char* appdata = new char[1024];
#if defined(_MSC_VER)
	size_t* sz = new size_t;
	getenv_s(sz, progfiles, 1024, "ProgramFiles");
	getenv_s(sz, appdata, 1024, "APPDATA");
#else
	strncpy(progfiles, getenv("ProgramFiles"), 1023);
	strncpy(appdata, getenv("APPDATA"), 1023);
	progfiles[1023] = appdata[1023] = '\0';
#endif

	if (strstr(cur, progfiles) == cur)
		configpath = std::string(appdata) + std::string("\\MiHaJong\\");

	delete[] cur; delete[] appdata; delete[] progfiles;
#if defined(_MSC_VER)
	delete sz;
#endif
	return configpath;
}

void ConfigFile::load() {
	/* 設定ファイル読み込み */
	{
		bool prefFileExists = exist(preferenceFile.c_str()); // 設定ファイルがあるかどうか調べる
		if (prefFileExists) { // 設定ファイル読み込み
			CodeConv::tifstream file(preferenceFile.c_str());
#ifdef UNICODE
			(void)file.imbue(std::locale(".65001"));
#else /* UNICODE */
			(void)file.imbue(std::locale(".932"));
#endif /* UNICODE */
			CodeConv::tstring line, text;
			while (std::getline(file, line)) text += line + _T("\n");
			INIParser::parseini(configMap, text.c_str(), false);
		}
	}
}

ConfigFile::ConfigFile() : preferenceFile(confPath() + "config.ini") {
	load();
}

void ConfigFile::save() {
	CodeConv::tofstream file(preferenceFile);
#ifdef UNICODE
	(void)file.imbue(std::locale(".65001"));
#else /* UNICODE */
	(void)file.imbue(std::locale(".932"));
#endif /* UNICODE */
	file << _T("MiHaJong Configuration File\n\n");
	file << _T("[preferences]\n\n");
	file << _T("; プレイヤー名\n");
	file << _T("name=") << configMap[_T("preferences")][_T("name")] << _T("\n\n");
	file << _T("; サーバーのアドレス\n");
	file << _T("; IPv4形式で入力してください。IPv6には現在対応していません。\n");
	file << _T("server=") << configMap[_T("preferences")][_T("server")] << _T("\n\n");
	file << _T("; フルスクリーン/ウィンドウの別\n");
	file << _T("; Windows版のみ。Linux版では無視されます。\n");
	file << _T("; フルスクリーンにするには、\"fullscreen\"を、\n");
	file << _T("; ウィンドウモードにするには、\"windowed\"を指定します。\n");
	file << _T("; なお、\"borderless\"を指定すると、ウィンドウを全画面に表示します。\n");
	file << _T("; デフォルトではウィンドウモードです。\n");
	file << _T("screen=") << configMap[_T("preferences")][_T("screen")] << _T("\n\n");
	file << _T("; ウィンドウサイズ/解像度\n");
	file << _T("; ウィンドウモードではウィンドウサイズ、フルスクリーンでは解像度を設定します。\n");
	file << _T("; 次の値が指定できます: svga, xga, fwxga, sxga, uxga, fullhd, wuxga\n");
	file << _T("; screenが\"borderless\"のときは無視されます。\n");
	file << _T("scrsize=") << configMap[_T("preferences")][_T("scrsize")] << _T("\n\n");
	file << _T("; モニタ番号\n");
	file << _T("; Windows版のみ。Linux版では無視されます。\n");
	file << _T("; 使用するモニタの番号を指定します。\n");
	file << _T("; 番号は1から始まります。\n");
	file << _T("; screenが\"fullscreen\"のときは無視されます。\n");
	file << _T("monitor=") << configMap[_T("preferences")][_T("monitor")] << _T("\n\n");
	file << _T("; BGM音量\n");
	file << _T("; 0～100の数値を指定してください。\n");
	file << _T("bgmvolume=") << configMap[_T("preferences")][_T("bgmvolume")] << _T("\n\n");
	file << _T("; 効果音音量\n");
	file << _T("; 0～100の数値を指定してください。\n");
	file << _T("sndvolume=") << configMap[_T("preferences")][_T("sndvolume")] << _T("\n\n");
	file << _T("; 牌の種類\n");
	file << _T("; デフォルトの牌を使用する場合は\"normal\"を、\n");
	file << _T("; ブラック牌を使用するには\"black\"を指定します。\n");
	file << _T("tile=") << configMap[_T("preferences")][_T("tile")] << _T("\n\n");
}

/* プレイヤー名 */
CodeConv::tstring ConfigFile::playerName() {
	return configMap[_T("preferences")][_T("name")];
}
void ConfigFile::playerName(const CodeConv::tstring& val) {
	configMap[_T("preferences")][_T("name")] = val;
}

/* フルスクリーン/ウィンドウ設定 */
ScreenMode::ScreenMode ConfigFile::scrMode() {
	using namespace ScreenMode;
	return configMap[_T("preferences")][_T("screen")] == _T("borderless") ? scrModeBorderless
		: configMap[_T("preferences")][_T("screen")] == _T("fullscreen") ? scrModeFullscreen : scrModeWindowed;
}
void ConfigFile::scrMode(ScreenMode::ScreenMode val) {
	using namespace ScreenMode;
	switch (val) {
	case scrModeBorderless:
		configMap[_T("preferences")][_T("screen")] = _T("borderless");
		break;
	case scrModeFullscreen:
		configMap[_T("preferences")][_T("screen")] = _T("fullscreen");
		break;
	case scrModeWindowed:
		configMap[_T("preferences")][_T("screen")] = _T("windowed");
		break;
	}
}

/* 解像度設定 */
ScreenConfig ConfigFile::screenResolution() {
	const std::map<CodeConv::tstring, ScreenConfig> scrConfList = {
		{_T("scr_800_600"),   screenSVGA},   {_T("svga"),   screenSVGA},
		{_T("scr_1024_768"),  screenXGA},    {_T("xga"),    screenXGA},
		{_T("scr_1366_768"),  screenFWXGA},  {_T("fwxga"),  screenFWXGA},
		{_T("scr_1280_1024"), screenSXGA},   {_T("sxga"),   screenSXGA},
		{_T("scr_1600_1200"), screenUXGA},   {_T("uxga"),   screenUXGA},
		{_T("scr_1920_1080"), screenFullHD}, {_T("fullhd"), screenFullHD},
		{_T("scr_1920_1200"), screenWUXGA},  {_T("wuxga"),  screenWUXGA},
	};
	try {
		return scrConfList.at(configMap[_T("preferences")][_T("scrsize")]);
	} catch (std::out_of_range&) {
		return screenDefaultRes;
	}
}
void ConfigFile::screenResolution(ScreenConfig val) {
	switch (val) {
	case screenSVGA:
		configMap[_T("preferences")][_T("scrsize")] = _T("svga");
		break;
	case screenXGA:
		configMap[_T("preferences")][_T("scrsize")] = _T("xga");
		break;
	case screenFWXGA:
		configMap[_T("preferences")][_T("scrsize")] = _T("fwxga");
		break;
	case screenSXGA:
		configMap[_T("preferences")][_T("scrsize")] = _T("sxga");
		break;
	case screenUXGA:
		configMap[_T("preferences")][_T("scrsize")] = _T("uxga");
		break;
	case screenFullHD:
		configMap[_T("preferences")][_T("scrsize")] = _T("fullhd");
		break;
	case screenWUXGA:
		configMap[_T("preferences")][_T("scrsize")] = _T("wuxga");
		break;
	default:
		throw std::out_of_range("invalid value for screen resolution");
	}
}

unsigned int ConfigFile::screenResolutionX() {
	if (scrMode() == ScreenMode::scrModeBorderless) {
		DISPLAY_DEVICE device; memset(&device, 0, sizeof device), device.cb = sizeof(DISPLAY_DEVICE);
		EnumDisplayDevices(nullptr, monitorNumber() - 1u, &device, 0);
		HDC hdc = CreateDC(device.DeviceName, device.DeviceName, nullptr, nullptr);
		int width = 0;
		if (hdc) {
			width = GetDeviceCaps(hdc, HORZRES);
			DeleteDC(hdc);
		}
		assert(monitorNumber() != 1 || width > 0);
		if (width == 0) {
			monitorNumber(1);
			return screenResolutionX();
		} else {
			return width;
		}
	} else {
		switch (screenResolution()) {
		case screenSVGA:   return  800u;
		case screenXGA:    return 1024u;
		case screenFWXGA:  return 1366u;
		case screenSXGA:   return 1280u;
		case screenUXGA:   return 1600u;
		case screenFullHD: return 1920u;
		case screenWUXGA:  return 1920u;
		default:           return    0u;
		}
	}
}
unsigned int ConfigFile::screenResolutionY() {
	if (scrMode() == ScreenMode::scrModeBorderless) {
		DISPLAY_DEVICE device; memset(&device, 0, sizeof device), device.cb = sizeof(DISPLAY_DEVICE);
		EnumDisplayDevices(nullptr, monitorNumber() - 1u, &device, 0);
		HDC hdc = CreateDC(device.DeviceName, device.DeviceName, nullptr, nullptr);
		int height = 0;
		if (hdc) {
			height = GetDeviceCaps(hdc, VERTRES);
			DeleteDC(hdc);
		}
		assert(monitorNumber() != 1 || height > 0);
		if (height == 0) {
			monitorNumber(1);
			return screenResolutionY();
		} else {
			return height;
		}
	} else {
		switch (screenResolution()) {
		case screenSVGA:   return  600u;
		case screenXGA:    return  768u;
		case screenFWXGA:  return  768u;
		case screenSXGA:   return 1024u;
		case screenUXGA:   return 1200u;
		case screenFullHD: return 1080u;
		case screenWUXGA:  return 1200u;
		default:           return    0u;
		}
	}
}

/* モニタ番号 */
unsigned int ConfigFile::monitorNumber() {
	const auto valTxt(configMap[_T("preferences")][_T("monitor")]);
	try {
		return std::stoul(valTxt);
	} catch (std::invalid_argument&) {
		return 1u;
	}
}
void ConfigFile::monitorNumber(unsigned int val) {
	configMap[_T("preferences")][_T("monitor")] = to_tstring(val);
}

/* BGM音量 */
unsigned int ConfigFile::bgmVolume() {
	const tregex r(_T("vol_")); // 互換用設定
	const auto valTxt(std::regex_replace(configMap[_T("preferences")][_T("bgmvolume")], r, _T("")));
	try {
		return std::stoul(valTxt);
	} catch (std::invalid_argument&) {
		return 100u;
	}
}
void ConfigFile::bgmVolume(unsigned int val) {
	configMap[_T("preferences")][_T("bgmvolume")] = to_tstring(val);
}

/* 効果音音量 */
unsigned int ConfigFile::soundVolume() {
	const tregex r(_T("vol_")); // 互換用設定
	const auto valTxt(std::regex_replace(configMap[_T("preferences")][_T("sndvolume")], r, _T("")));
	try {
		return std::stoul(valTxt);
	} catch (std::invalid_argument&) {
		return 100u;
	}
}
void ConfigFile::soundVolume(unsigned int val) {
	configMap[_T("preferences")][_T("sndvolume")] = to_tstring(val);
}

/* ブラック牌 */
bool ConfigFile::blackTile() {
	const tregex r(_T("_tile")); // 互換用設定
	return std::regex_replace(configMap[_T("preferences")][_T("tile")], r, _T("")) == _T("black");
}
void ConfigFile::blackTile(bool val) {
	configMap[_T("preferences")][_T("tile")] = val ? _T("black") : _T("normal");
}

/* サーバーのアドレス */
CodeConv::tstring ConfigFile::serverAddress() {
	return configMap[_T("preferences")][_T("server")];
}
void ConfigFile::serverAddress(const CodeConv::tstring& addr) {
	configMap[_T("preferences")][_T("server")] = addr;
}

}
