#include "font.h"
#include "init.h"
#include "resource.h"
#include <windows.h>
#include <cstdint>
#include <sstream>
#include <cassert>

using std::uint8_t;

namespace mihajong_graphic {

namespace {
	void LoadFileInResource(int name, int type, DWORD& size, const uint8_t*& data) {
		HRSRC rc = FindResource(GraphicDLL, MAKEINTRESOURCE(name), MAKEINTRESOURCE(type));
		DWORD err = GetLastError();
		assert(rc != nullptr);
		HGLOBAL rcData = LoadResource(GraphicDLL, rc);
		assert(rcData != nullptr);
		size = SizeofResource(GraphicDLL, rc);
		assert(size != 0);
		data = static_cast<const uint8_t*>(::LockResource(rcData));
	}
}

FontMap* FontMap::instantiate() {
	static FontMap myInstance;
	return &myInstance;
}

FontMapLargeChr* FontMapLargeChr::instantiate() {
	static FontMapLargeChr myInstance;
	return &myInstance;
}

FontMapSmallChr* FontMapSmallChr::instantiate() {
	static FontMapSmallChr myInstance;
	return &myInstance;
}

FontMapCallDigits* FontMapCallDigits::instantiate() {
	static FontMapCallDigits myInstance;
	return &myInstance;
}

FontMapScoreDigits* FontMapScoreDigits::instantiate() {
	static FontMapScoreDigits myInstance;
	return &myInstance;
}

void FontMapClass::readCharMap(int ResourceNum) {
	DWORD size; const uint8_t* data;
	LoadFileInResource(ResourceNum, CHARMAP_FILE, size, data);
	std::istringstream charmap(std::string(reinterpret_cast<const char*>(data), static_cast<size_t>(size)));
	while (true) {
		unsigned chrnum, mapped_to; char flag;
		charmap >> std::hex >> chrnum >> flag >> std::dec >> mapped_to;
		if ((chrnum == 0) || (flag == '!')) break; // sentinel
		m.insert(fMapDat(static_cast<wchar_t>(chrnum), std::make_pair((flag == '*'), mapped_to)));
	}
}

FontMapClass::charAttr FontMapClass::map(wchar_t c) {
	if (m.find(c) != m.end()) return m[c];
	else return std::make_pair(true, Default_Chr());
}

FontMapClass::~FontMapClass() {
}

FontMap::FontMap() {
	readCharMap(IDB_CHARMAP_FONT);
}

FontMapLargeChr::FontMapLargeChr() {
	readCharMap(IDB_CHARMAP_FONT_HUGE);
}

FontMapSmallChr::FontMapSmallChr() {
	readCharMap(IDB_CHARMAP_FONT_SMALL);
}

FontMapCallDigits::FontMapCallDigits() {
	readCharMap(IDB_CHARMAP_CALL_DIGITS);
}

FontMapScoreDigits::FontMapScoreDigits() {
	readCharMap(IDB_CHARMAP_SCORE_DIGITS);
}

}
