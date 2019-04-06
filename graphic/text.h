#pragma once

#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include "directx.h"
#include <string>
#include <vector>
#include "loadtex.h"
#include "font.h"
#include <mutex>

namespace mihajong_graphic {

class ITextRenderer {
protected:
	virtual const unsigned int FontBaseSize() = 0;
	virtual const unsigned int FontWidth() = 0;
	virtual const unsigned int FontCols() = 0;
	virtual const unsigned int FontPadding() = 0;
	struct StringAttr;
	struct SpriteAttr;
	FontMapClass* fontmap;
	DevicePtr myDevice;
	TexturePtr font;
	std::vector<StringAttr*> StringData;
	std::vector<std::vector<SpriteAttr*> > SpriteData;
	void spriteRecalc(unsigned int ID, SpriteAttr* sprite, float chrAdvance, float cursorPos);
	void reconstruct();
	void reconstruct(unsigned int ID, bool rescanStr = true);
	void deleteSprite();
	void deleteSprite(unsigned int ID);
	std::recursive_mutex SpriteMutex;
public:
	explicit ITextRenderer(DevicePtr device);
	ITextRenderer(const ITextRenderer&) = delete; // Delete unexpected copy constructor
	ITextRenderer& operator= (const ITextRenderer&) = delete; // Delete unexpected assign operator
	virtual ~ITextRenderer() = 0;
	void NewText(unsigned int ID, const std::wstring& str, int x, int y,
		float scale = 1.0, float width = 1.0, ArgbColor color = 0xffffffff);
	void NewText(unsigned int ID, const std::string& str, int x, int y,
		float scale = 1.0, float width = 1.0, ArgbColor color = 0xffffffff);
	void DelText(unsigned int ID);
	void Render();
	virtual unsigned strWidthByCols(const std::wstring& str);
	virtual unsigned strWidthByCols(const std::string& str);
	virtual unsigned strWidthByPix(const std::wstring& str);
	virtual unsigned strWidthByPix(const std::string& str);
};

class TextRenderer : public ITextRenderer {
private:
	const unsigned int FontBaseSize() {return 40;}
	const unsigned int FontWidth() {return FontBaseSize();}
	const unsigned int FontCols() {return 48;}
	const unsigned int FontPadding() {return 2;}
public:
	explicit TextRenderer(DevicePtr device);
	TextRenderer(const TextRenderer&) = delete; // Delete unexpected copy constructor
	TextRenderer& operator= (const TextRenderer&) = delete; // Delete unexpected assign operator
	~TextRenderer();
};

class HugeTextRenderer : public ITextRenderer {
private:
	const unsigned int FontBaseSize() {return 224;}
	const unsigned int FontWidth() {return FontBaseSize();}
	const unsigned int FontCols() {return 9;}
	const unsigned int FontPadding() {return 0;}
public:
	explicit HugeTextRenderer(DevicePtr device);
	HugeTextRenderer(const HugeTextRenderer&) = delete; // Delete unexpected copy constructor
	HugeTextRenderer& operator= (const HugeTextRenderer&) = delete; // Delete unexpected assign operator
	~HugeTextRenderer();
};

class SmallTextRenderer : public ITextRenderer {
private:
	const unsigned int FontBaseSize() {return 20;}
	const unsigned int FontWidth() {return FontBaseSize();}
	const unsigned int FontCols() {return 94;}
	const unsigned int FontPadding() {return 1;}
public:
	explicit SmallTextRenderer(DevicePtr device);
	SmallTextRenderer(const SmallTextRenderer&) = delete; // Delete unexpected copy constructor
	SmallTextRenderer& operator= (const SmallTextRenderer&) = delete; // Delete unexpected assign operator
	~SmallTextRenderer();
};

class CallDigitRenderer : public ITextRenderer {
private:
	const unsigned int FontBaseSize() {return 96;}
	const unsigned int FontWidth() {return 48;}
	const unsigned int FontCols() {return 39;}
	const unsigned int FontPadding() {return 0;}
public:
	explicit CallDigitRenderer(DevicePtr device);
	CallDigitRenderer(const CallDigitRenderer&) = delete; // Delete unexpected copy constructor
	CallDigitRenderer& operator= (const CallDigitRenderer&) = delete; // Delete unexpected assign operator
	~CallDigitRenderer();
};

class ScoreDigitRenderer : public ITextRenderer {
private:
	const unsigned int FontBaseSize() {return 128;}
	const unsigned int FontWidth() {return 96;}
	const unsigned int FontCols() {return 10;}
	const unsigned int FontPadding() {return 0;}
public:
	explicit ScoreDigitRenderer(DevicePtr device);
	ScoreDigitRenderer(const ScoreDigitRenderer&) = delete; // Delete unexpected copy constructor
	ScoreDigitRenderer& operator= (const ScoreDigitRenderer&) = delete; // Delete unexpected assign operator
	~ScoreDigitRenderer();
};

struct ITextRenderer::StringAttr {
	std::wstring str;
	int X, Y;
	float scale, width;
	ArgbColor color;
	StringAttr();
};

struct ITextRenderer::SpriteAttr {
	unsigned short chr_id;
	bool isFullWidth;
	int X, Y;
	float widthScale, heightScale;
	ArgbColor color;
	TransformMatrix matrix;
	SpriteAttr();
};

}
