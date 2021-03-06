#pragma once

#include <map>

namespace mihajong_graphic {

class FontMapClass {
protected:
	typedef std::pair<bool, unsigned short> charAttr;
	typedef std::map<wchar_t, charAttr> fMap;
	typedef fMap::value_type fMapDat;
	FontMapClass() {};
	FontMapClass(const FontMapClass&) = delete; // Delete unexpected copy constructor
	FontMapClass& operator= (const FontMapClass&) = delete; // Delete unexpected assign operator
	virtual ~FontMapClass() = 0;
	fMap m;
	virtual const unsigned short Default_Chr() = 0;
	void readCharMap(int ResourceNum);
public:
	static FontMapClass* instantiate() {return nullptr;} // This class is an abstract class which cannot be instantiated
	charAttr map(wchar_t c);
};

class FontMap : public FontMapClass {
private:
	FontMap();
	FontMap(const FontMap&) = delete; // Delete unexpected copy constructor
	FontMap& operator= (const FontMap&) = delete; // Delete unexpected assign operator
	virtual ~FontMap() {}
	const unsigned short Default_Chr() {return 0;}
public:
	static FontMap* instantiate();
};

class FontMapLargeChr : public FontMapClass {
private:
	FontMapLargeChr();
	FontMapLargeChr(const FontMapLargeChr&) = delete; // Delete unexpected copy constructor
	FontMapLargeChr& operator= (const FontMapLargeChr&) = delete; // Delete unexpected assign operator
	virtual ~FontMapLargeChr() {}
	const unsigned short Default_Chr() {return 89;}
public:
	static FontMapLargeChr* instantiate();
};

class FontMapSmallChr : public FontMapClass {
private:
	FontMapSmallChr();
	FontMapSmallChr(const FontMapSmallChr&) = delete; // Delete unexpected copy constructor
	FontMapSmallChr& operator= (const FontMapSmallChr&) = delete; // Delete unexpected assign operator
	virtual ~FontMapSmallChr() {}
	const unsigned short Default_Chr() {return 187;}
public:
	static FontMapSmallChr* instantiate();
};

class FontMapCallDigits : public FontMapClass {
private:
	FontMapCallDigits();
	FontMapCallDigits(const FontMapCallDigits&) = delete; // Delete unexpected copy constructor
	FontMapCallDigits& operator= (const FontMapCallDigits&) = delete; // Delete unexpected assign operator
	virtual ~FontMapCallDigits() {}
	const unsigned short Default_Chr() {return 12;}
public:
	static FontMapCallDigits* instantiate();
};

class FontMapScoreDigits : public FontMapClass {
private:
	FontMapScoreDigits();
	FontMapScoreDigits(const FontMapScoreDigits&) = delete; // Delete unexpected copy constructor
	FontMapScoreDigits& operator= (const FontMapScoreDigits&) = delete; // Delete unexpected assign operator
	virtual ~FontMapScoreDigits() {}
	const unsigned short Default_Chr() {return 39;}
public:
	static FontMapScoreDigits* instantiate();
};

}
