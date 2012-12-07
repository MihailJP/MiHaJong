#include "text.h"
#include "resource.h"
#include "font.h"
#include "geometry.h"
#include "../mjcore/strcode.h"
#include "sprite.h"

namespace mihajong_graphic {

TextRenderer::TextRenderer(LPDIRECT3DDEVICE9 device) {
	myDevice = device;
	LoadTexture(device, &font, MAKEINTRESOURCE(IDB_PNG_FONT), 1280, 3240);
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

/* �V�K�̕�����I�u�W�F�N�g���쐬���� */
void TextRenderer::NewText(unsigned int ID, const std::wstring& str, int x, int y, float scale, float width, D3DCOLOR color) {
	if (StringData.size() <= ID) StringData.resize(ID + 1, nullptr); // �z��̊g��
	bool TextChanged = (!StringData[ID]) || (StringData[ID]->str != str);
	if (StringData[ID] && TextChanged) delete StringData[ID]; // ���ɑ��݂����ꍇ
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
void TextRenderer::NewText(unsigned int ID, const std::string& str, int x, int y, float scale, float width, D3DCOLOR color) {
	NewText(ID, CodeConv::ANSItoWIDE(str), x, y, scale, width, color);
}

/* ������I�u�W�F�N�g�̌�n�� */
void TextRenderer::DelText(unsigned int ID) {
	delete StringData[ID]; StringData[ID] = nullptr;
	deleteSprite(ID);
}

/* �X�v���C�g�č\�z */
void TextRenderer::spriteRecalc(unsigned int ID, SpriteAttr* sprite, float chrAdvance, float cursorPos) {
	sprite->X = StringData[ID]->X + chrAdvance * cursorPos - FontPadding;
	sprite->Y = StringData[ID]->Y - FontPadding;
	sprite->widthScale = StringData[ID]->scale * StringData[ID]->width;
	sprite->heightScale = StringData[ID]->scale;
	sprite->color = StringData[ID]->color;
	/* �s����v�Z���� */
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
	/* �����܂� */
}
void TextRenderer::reconstruct(unsigned int ID, bool rescanStr) {
	if (SpriteData.size() <= ID) SpriteData.resize(ID + 1, std::vector<SpriteAttr*>()); // �z��̊g��
	if ((!SpriteData[ID].empty()) && rescanStr) deleteSprite(ID); // ���ɑ��݂����ꍇ
	if (!StringData[ID]) /* �ʂ�� */
		return; /* �K�b */
	float chrAdvance = (FontBaseSize - FontPadding * 2) * StringData[ID]->scale * StringData[ID]->width;
	float cursorPos = 0;
	if (rescanStr) {
		for (auto k = StringData[ID]->str.begin(); k != StringData[ID]->str.end(); ++k) {
			SpriteData[ID].push_back(new SpriteAttr);
			SpriteData[ID].back()->sprite = nullptr;
			if (FAILED(D3DXCreateSprite(myDevice, &SpriteData[ID].back()->sprite)))
				throw _T("�X�v���C�g�̐����Ɏ��s���܂���");
			SpriteData[ID].back()->chr_id = FontMap::instantiate()->map(*k);
			spriteRecalc(ID, SpriteData[ID].back(), chrAdvance, cursorPos);
			if (*k <= L'\x7f') cursorPos += .5f;
			else cursorPos += 1.0f;
		}
	} else {
		for (auto k = SpriteData[ID].begin(); k != SpriteData[ID].end(); ++k) {
			spriteRecalc(ID, *k, chrAdvance, cursorPos);
			if (((*k)->chr_id > 0) && ((*k)->chr_id <= 96)) cursorPos += .5f;
			else cursorPos += 1.0f;
		}
	}
}
void TextRenderer::reconstruct() {
	// VERY SLOW. DO NOT USE.
	deleteSprite();
	for (unsigned i = 0; i < StringData.size(); i++)
		reconstruct(i);
}

/* �X�v���C�g���폜���� */
void TextRenderer::deleteSprite(unsigned int ID) {
	for (auto k = SpriteData[ID].begin(); k != SpriteData[ID].end(); ++k) {
		if ((*k) && ((*k)->sprite)) {
			(*k)->sprite->Release(); (*k)->sprite = nullptr;
		}
	}
	SpriteData[ID].clear();
}
void TextRenderer::deleteSprite() {
	for (unsigned int i = 0; i < SpriteData.size(); i++)
		deleteSprite(i);
	SpriteData.clear();
}

/* �����_�����O */
void TextRenderer::Render() {
	for (auto i = SpriteData.begin(); i != SpriteData.end(); ++i) {
		for (auto k = (*i).begin(); k != (*i).end(); ++k) {
			if (!(*k)) continue;
			RECT rect = {
				((*k)->chr_id % FontCols) * FontBaseSize,
				((*k)->chr_id / FontCols) * FontBaseSize,
				((*k)->chr_id % FontCols + 1) * FontBaseSize,
				((*k)->chr_id / FontCols + 1) * FontBaseSize,
			};
			SpriteRenderer::ShowSprite(
				(*k)->sprite, font, (*k)->X, (*k)->Y, FontBaseSize, FontBaseSize,
				(*k)->color, &rect, 0, 0, &((*k)->matrix));
		}
	}
}

}