#include "text.h"
#include "resource.h"
#include "font.h"
#include "geometry.h"
#include "../mjcore/strcode.h"

TextRenderer::TextRenderer(LPDIRECT3DDEVICE9 device) {
	myDevice = device;
	LoadTexture(device, &font, MAKEINTRESOURCE(IDB_PNG_FONT), 1280, 1280);
}

TextRenderer::~TextRenderer() {
	deleteSprite();
	for (auto k = StringData.begin(); k != StringData.end(); ++k) {
		if (*k) {
			delete (*k); (*k) = nullptr;
		}
	}
	if (font) font->Release();
}

/* 新規の文字列オブジェクトを作成する */
void TextRenderer::NewText(unsigned int ID, const std::wstring& str, int x, int y, float scale, float width, D3DCOLOR color) {
	if (StringData.size() <= ID) StringData.resize(ID + 1, nullptr); // 配列の拡張
	if (!StringData[ID]) delete StringData[ID]; // 既に存在した場合
	StringData[ID] = new StringAttr;
	StringData[ID]->str = std::wstring(str);
	StringData[ID]->X = x; StringData[ID]->Y = y;
	StringData[ID]->scale = scale; StringData[ID]->width = width;
	StringData[ID]->color = color;
	reconstruct();
}
void TextRenderer::NewText(unsigned int ID, const std::string& str, int x, int y, float scale, float width, D3DCOLOR color) {
	NewText(ID, CodeConv::ANSItoWIDE(str), x, y, scale, width, color);
}

/* 文字列オブジェクトの後始末 */
void TextRenderer::DelText(unsigned int ID) {
	delete StringData[ID]; StringData[ID] = nullptr;
	reconstruct();
}

/* スプライト再構築 */
void TextRenderer::reconstruct() {
	deleteSprite();
	for (auto k = StringData.begin(); k != StringData.end(); ++k) {
		float chrAdvance = (FontBaseSize - FontPadding * 2) * (*k)->scale * (*k)->width; float cursorPos = 0;
		for (auto c = (*k)->str.begin(); c != (*k)->str.end(); ++c) {
			SpriteData.push_back(new SpriteAttr);
			SpriteData.back()->sprite = nullptr;
			if (FAILED(D3DXCreateSprite(myDevice, &SpriteData.back()->sprite)))
				throw _T("スプライトの生成に失敗しました");
			SpriteData.back()->chr = *c;
			SpriteData.back()->X = (*k)->X + chrAdvance * cursorPos - FontPadding;
			SpriteData.back()->Y = (*k)->Y - FontPadding;
			SpriteData.back()->widthScale = (*k)->scale * (*k)->width;
			SpriteData.back()->heightScale = (*k)->scale;
			SpriteData.back()->color = (*k)->color;
			if (*c <= L'\x7f') cursorPos += .5f;
			else cursorPos += 1.0f;
		}
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

/* レンダリング */
void TextRenderer::Render() {
	for (auto k = SpriteData.begin(); k != SpriteData.end(); ++k) {
		/* 行列を計算する */
		D3DXMATRIX matrix1; D3DXMatrixIdentity(&matrix1);
		D3DXMatrixTranslation(&matrix1, (float)-((*k)->X), (float)-((*k)->Y), 0);
		D3DXMATRIX matrix2; D3DXMatrixIdentity(&matrix2);
		D3DXMatrixScaling(&matrix2, (float)(*k)->widthScale, (float)(*k)->heightScale, 0.0f);
		D3DXMATRIX matrix3; D3DXMatrixIdentity(&matrix3);
		D3DXMatrixTranslation(&matrix3, (float)(*k)->X, (float)(*k)->Y, 0);
		D3DXMATRIX matrix4; D3DXMatrixIdentity(&matrix4);
		D3DXMatrixScaling(&matrix4, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
		D3DXMATRIX matrix; D3DXMatrixIdentity(&matrix);
		::D3DXMatrixMultiply(&matrix, &matrix1, &matrix2);
		::D3DXMatrixMultiply(&matrix, &matrix, &matrix3);
		::D3DXMatrixMultiply(&matrix, &matrix, &matrix4);
		/* ここまで */
		RECT rect = {
			(FontMap::instantiate()->map((*k)->chr) % FontCols) * FontBaseSize,
			(FontMap::instantiate()->map((*k)->chr) / FontCols) * FontBaseSize,
			(FontMap::instantiate()->map((*k)->chr) % FontCols + 1) * FontBaseSize,
			(FontMap::instantiate()->map((*k)->chr) / FontCols + 1) * FontBaseSize,
		};
		D3DXVECTOR3 Center(0, 0, 0);
		D3DXVECTOR3 Pos((float)(*k)->X, (float)(*k)->Y, 0);
		(*k)->sprite->Begin(D3DXSPRITE_ALPHABLEND);
		(*k)->sprite->SetTransform(&matrix);
		(*k)->sprite->Draw(font, &rect, &Center, &Pos, (*k)->color);
		(*k)->sprite->End();
	}
}
