#pragma once

#include <map>

namespace mihajong_graphic {

class FontMapClass {
protected:
	typedef std::map<wchar_t, unsigned short> fMap;
	typedef std::map<wchar_t, unsigned short>::value_type fMapDat;
	FontMapClass() {};
	FontMapClass(const FontMapClass&) {}
	virtual ~FontMapClass() = 0 {};
	FontMapClass& operator=(const FontMapClass&) {return *this;}
	fMap m;
	virtual const unsigned short Default_Chr() = 0;
public:
	static FontMapClass* instantiate() {return nullptr;} // This class is an abstract class which cannot be instantiated
	unsigned short map(wchar_t c);
};

class FontMap : public FontMapClass {
private:
	FontMap();
	FontMap(const FontMap&) {}
	virtual ~FontMap() {}
	const unsigned short Default_Chr() {return 0;}
public:
	static FontMap* instantiate();
};

class FontMapLargeChr : public FontMapClass {
private:
	FontMapLargeChr();
	FontMapLargeChr(const FontMapLargeChr&) {}
	virtual ~FontMapLargeChr() {}
	const unsigned short Default_Chr() {return 43;}
public:
	static FontMapLargeChr* instantiate();
};

class FontMapSmallChr : public FontMapClass {
private:
	FontMapSmallChr();
	FontMapSmallChr(const FontMapSmallChr&) {}
	virtual ~FontMapSmallChr() {}
	const unsigned short Default_Chr() {return 187;}
public:
	static FontMapSmallChr* instantiate();
};

}
