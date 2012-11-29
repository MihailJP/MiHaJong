#ifndef GRAPHIC_TEXT_H
#define GRAPHIC_TEXT_H

#include <Windows.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <string>
#include <vector>
#include "loadtex.h"

class TextRenderer {
private:
	static const unsigned int FontBaseSize = 40;
	static const unsigned int FontCols = 32;
	static const unsigned int FontPadding = 2;
	struct StringAttr;
	struct SpriteAttr;
	LPDIRECT3DDEVICE9 myDevice;
	LPDIRECT3DTEXTURE9 font;
	std::vector<StringAttr*> StringData;
	std::vector<std::vector<SpriteAttr*> > SpriteData;
	void spriteRecalc(unsigned int ID, SpriteAttr* sprite, float chrAdvance, float cursorPos);
	void reconstruct();
	void reconstruct(unsigned int ID, bool rescanStr = true);
	void deleteSprite();
	void deleteSprite(unsigned int ID);
public:
	TextRenderer(LPDIRECT3DDEVICE9 device);
	~TextRenderer();
	void NewText(unsigned int ID, const std::wstring& str, int x, int y,
		float scale = 1.0, float width = 1.0, D3DCOLOR color = 0xffffffff);
	void NewText(unsigned int ID, const std::string& str, int x, int y,
		float scale = 1.0, float width = 1.0, D3DCOLOR color = 0xffffffff);
	void DelText(unsigned int ID);
	void Render();
};

struct TextRenderer::StringAttr {
	std::wstring str;
	int X, Y;
	float scale, width;
	D3DCOLOR color;
};

struct TextRenderer::SpriteAttr {
	LPD3DXSPRITE sprite;
	unsigned short chr_id;
	int X, Y;
	float widthScale, heightScale;
	D3DCOLOR color;
	D3DXMATRIX matrix;
};
#endif
