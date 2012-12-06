#pragma once

#include <map>

namespace mihajong_graphic {

class FontMap {
private:
	typedef std::map<wchar_t, unsigned short> fMap;
	typedef std::map<wchar_t, unsigned short>::value_type fMapDat;
	FontMap();
	FontMap(const FontMap&) {}
	virtual ~FontMap() {}
	FontMap& operator=(const FontMap&) {return *this;}
	fMap m;
	static const unsigned short Default_Chr = 0;
public:
	static FontMap* instantiate();
	unsigned short map(wchar_t c);
};

}
