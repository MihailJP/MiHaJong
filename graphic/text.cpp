#include "text.h"
#include "resource.h"
#include "geometry.h"
#include "../mjcore/strcode.h"
#include "sprite.h"

namespace mihajong_graphic {

ITextRenderer::ITextRenderer(LPDIRECT3DDEVICE9 device) {
	myDevice = device;
}
TextRenderer::TextRenderer(LPDIRECT3DDEVICE9 device) : ITextRenderer(device) {
	fontmap = FontMap::instantiate();
	LoadTexture(device, &font, MAKEINTRESOURCE(IDB_PNG_FONT), 1280, 3240);
}
HugeTextRenderer::HugeTextRenderer(LPDIRECT3DDEVICE9 device) : ITextRenderer(device) {
	fontmap = FontMapLargeChr::instantiate();
	LoadTexture(device, &font, MAKEINTRESOURCE(IDB_PNG_FONT_HUGE), 1792, 2240);
}
SmallTextRenderer::SmallTextRenderer(LPDIRECT3DDEVICE9 device) : ITextRenderer(device) {
	fontmap = FontMapSmallChr::instantiate();
	LoadTexture(device, &font, MAKEINTRESOURCE(IDB_PNG_FONT_SMALL), 1880, 1920);
}
CallDigitRenderer::CallDigitRenderer(LPDIRECT3DDEVICE9 device) : ITextRenderer(device) {
	fontmap = FontMapCallDigits::instantiate();
	LoadTexture(device, &font, MAKEINTRESOURCE(IDB_PNG_CALL_DIGITS), 1872, 96);
}

ITextRenderer::~ITextRenderer() {
	deleteSprite();
	for (auto k = StringData.begin(); k != StringData.end(); ++k) {
		if (*k) {
			delete (*k); (*k) = nullptr;
		}
	}
	if (font) font->Release();
}
TextRenderer::~TextRenderer() {
}
HugeTextRenderer::~HugeTextRenderer() {
}
SmallTextRenderer::~SmallTextRenderer() {
}
CallDigitRenderer::~CallDigitRenderer() {
}

/* 新規の文字列オブジェクトを作成する */
void ITextRenderer::NewText(unsigned int ID, const std::wstring& str, int x, int y, float scale, float width, D3DCOLOR color) {
	if (StringData.size() <= ID) StringData.resize(ID + 1, nullptr); // 配列の拡張
	bool TextChanged = (!StringData[ID]) || (StringData[ID]->str != str);
	if (StringData[ID] && TextChanged) delete StringData[ID]; // 既に存在した場合
	if (TextChanged) StringData[ID] = new StringAttr;
	StringData[ID]->X = x; StringData[ID]->Y = y;
	StringData[ID]->scale = scale; StringData[ID]->width = width;
	StringData[ID]->color = color;
	if (TextChanged) {
		StringData[ID]->str = std::wstring(str);
		reconstruct(ID, true);
	} else {
		reconstruct(ID, false);
	}
}
void ITextRenderer::NewText(unsigned int ID, const std::string& str, int x, int y, float scale, float width, D3DCOLOR color) {
	NewText(ID, CodeConv::ANSItoWIDE(str), x, y, scale, width, color);
}

/* 文字列オブジェクトの後始末 */
void ITextRenderer::DelText(unsigned int ID) {
	if (StringData.size() <= ID) return;
	delete StringData[ID]; StringData[ID] = nullptr;
	deleteSprite(ID);
}

/* スプライト再構築 */
void ITextRenderer::spriteRecalc(unsigned int ID, SpriteAttr* sprite, float chrAdvance, float cursorPos) {
	sprite->X = StringData[ID]->X + chrAdvance * cursorPos - FontPadding();
	sprite->Y = StringData[ID]->Y - FontPadding();
	sprite->widthScale = StringData[ID]->scale * StringData[ID]->width;
	sprite->heightScale = StringData[ID]->scale;
	sprite->color = StringData[ID]->color;
	/* 行列を計算する */
	D3DXMATRIX m; D3DXMatrixIdentity(&m);
	D3DXMatrixIdentity(&sprite->matrix);
	D3DXMatrixTranslation(&m, (float)-(sprite->X), (float)-(sprite->Y), 0);
	D3DXMatrixMultiply(&sprite->matrix, &sprite->matrix, &m);
	D3DXMatrixScaling(&m, sprite->widthScale, sprite->heightScale, 0.0f);
	D3DXMatrixMultiply(&sprite->matrix, &sprite->matrix, &m);
	D3DXMatrixTranslation(&m, (float)sprite->X, (float)sprite->Y, 0);
	D3DXMatrixMultiply(&sprite->matrix, &sprite->matrix, &m);
	D3DXMatrixScaling(&m, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXMatrixMultiply(&sprite->matrix, &sprite->matrix, &m);
	/* ここまで */
}
void ITextRenderer::reconstruct(unsigned int ID, bool rescanStr) {
	if (SpriteData.size() <= ID) SpriteData.resize(ID + 1, std::vector<SpriteAttr*>()); // 配列の拡張
	if ((!SpriteData[ID].empty()) && rescanStr) deleteSprite(ID); // 既に存在した場合
	if (!StringData[ID]) /* ぬるぽ */
		return; /* ガッ */
	float chrAdvance = (FontWidth() - FontPadding() * 2) * StringData[ID]->scale * StringData[ID]->width;
	float cursorPos = 0;
	if (rescanStr) {
		for (auto k = StringData[ID]->str.begin(); k != StringData[ID]->str.end(); ++k) {
			SpriteData[ID].push_back(new SpriteAttr);
			SpriteData[ID].back()->isFullWidth = fontmap->map(*k).first;
			SpriteData[ID].back()->chr_id = fontmap->map(*k).second;
			spriteRecalc(ID, SpriteData[ID].back(), chrAdvance, cursorPos);
			if (SpriteData[ID].back()->isFullWidth) cursorPos += 1.0f;
			else cursorPos += .5f;
		}
	} else {
		for (auto k = SpriteData[ID].begin(); k != SpriteData[ID].end(); ++k) {
			spriteRecalc(ID, *k, chrAdvance, cursorPos);
			if ((*k)->isFullWidth) cursorPos += 1.0f;
			else cursorPos += .5f;
		}
	}
}
void ITextRenderer::reconstruct() {
	// VERY SLOW. DO NOT USE.
	deleteSprite();
	for (unsigned i = 0; i < StringData.size(); i++)
		reconstruct(i);
}

/* スプライトを削除する */
void ITextRenderer::deleteSprite(unsigned int ID) {
	for (auto k = SpriteData[ID].begin(); k != SpriteData[ID].end(); ++k)
		delete (*k);
	SpriteData[ID].clear();
}
void ITextRenderer::deleteSprite() {
	for (unsigned int i = 0; i < SpriteData.size(); i++)
		deleteSprite(i);
	SpriteData.clear();
}

/* レンダリング */
void ITextRenderer::Render() {
	for (auto i = SpriteData.begin(); i != SpriteData.end(); ++i) {
		for (auto k = (*i).begin(); k != (*i).end(); ++k) {
			if (!(*k)) continue;
			RECT rect = {
				((*k)->chr_id % FontCols()) * FontWidth(),
				((*k)->chr_id / FontCols()) * FontBaseSize(),
				((*k)->chr_id % FontCols() + 1) * FontWidth(),
				((*k)->chr_id / FontCols() + 1) * FontBaseSize(),
			};
			SpriteRenderer::instantiate(myDevice)->ShowSprite(
				font, (*k)->X, (*k)->Y, FontWidth(), FontBaseSize(),
				(*k)->color, &rect, 0, 0, &((*k)->matrix));
		}
	}
}

}
