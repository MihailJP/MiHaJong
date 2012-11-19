#include "text.h"
#include "resource.h"
#include "font.h"
#include "../mjcore/strcode.h"

TextRenderer::TextRenderer(LPDIRECT3DDEVICE9 device) {
	myDevice = device;
	LoadTexture(device, &font, MAKEINTRESOURCE(IDB_PNG_FONT), 1280, 1280);
}

TextRenderer::~TextRenderer() {
	for (auto k = StringData.begin(); k != StringData.end(); ++k) {
		if (*k) {
			delete (*k); (*k) = nullptr;
		}
	}
	if (font) font->Release();
}

/* 新規の文字列オブジェクトを作成する */
void TextRenderer::NewText(unsigned int ID, const std::wstring& str, int x, int y, float scale = 1.0, float width = 1.0) {
	if (StringData.size() <= ID) StringData.resize(ID + 1, nullptr); // 配列の拡張
	if (!StringData[ID]) delete StringData[ID]; // 既に存在した場合
	StringData[ID] = new StringAttr;
	StringData[ID]->str = std::wstring(str);
	StringData[ID]->X = x; StringData[ID]->Y = y;
	StringData[ID]->scale = scale; StringData[ID]->width = width;
	reconstruct();
}
void TextRenderer::NewText(unsigned int ID, const std::string& str, int x, int y, float scale = 1.0, float width = 1.0) {
	NewText(ID, CodeConv::ANSItoWIDE(str), x, y, scale, width);
}

/* 文字列オブジェクトの後始末 */
void TextRenderer::DelText(unsigned int ID) {
	deleteSprite();
	delete StringData[ID]; StringData[ID] = nullptr;
	reconstruct();
}

/* スプライト再構築 */
void TextRenderer::reconstruct() {
	deleteSprite();
	for (auto k = StringData.begin(); k != StringData.end(); ++k) {
		
	}
}

/* スプライトを削除する */
void TextRenderer::deleteSprite() {
	for (auto k = SpriteData.begin(); k != SpriteData.end(); ++k) {
		if ((*k)->sprite) {
			(*k)->sprite->Release(); (*k)->sprite = nullptr;
		}
	}
	SpriteData.clear();
}
