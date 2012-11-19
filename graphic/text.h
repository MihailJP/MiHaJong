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
	struct StringAttr;
	struct SpriteAttr;
	LPDIRECT3DDEVICE9 myDevice;
	LPDIRECT3DTEXTURE9 font;
	std::vector<StringAttr*> StringData;
	std::vector<SpriteAttr*> SpriteData;
	void reconstruct();
	void deleteSprite();
public:
	TextRenderer(LPDIRECT3DDEVICE9 device);
	~TextRenderer();
	void NewText(unsigned int ID, const std::wstring& str, int x, int y, float scale, float width);
	void NewText(unsigned int ID, const std::string& str, int x, int y, float scale, float width);
	void DelText(unsigned int ID);
	void Render();
};

struct TextRenderer::StringAttr {
	std::wstring str;
	int X, Y;
	float scale, width;
};

struct TextRenderer::SpriteAttr {
	LPD3DXSPRITE sprite;
	int X, Y;
	float widthScale, heightScale;
};
#endif
