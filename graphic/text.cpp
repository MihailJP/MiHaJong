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

/* �V�K�̕�����I�u�W�F�N�g���쐬���� */
void TextRenderer::NewText(unsigned int ID, const std::wstring& str, int x, int y, float scale = 1.0, float width = 1.0) {
	if (StringData.size() <= ID) StringData.resize(ID + 1, nullptr); // �z��̊g��
	if (!StringData[ID]) delete StringData[ID]; // ���ɑ��݂����ꍇ
	StringData[ID] = new StringAttr;
	StringData[ID]->str = std::wstring(str);
	StringData[ID]->X = x; StringData[ID]->Y = y;
	StringData[ID]->scale = scale; StringData[ID]->width = width;
	reconstruct();
}
void TextRenderer::NewText(unsigned int ID, const std::string& str, int x, int y, float scale = 1.0, float width = 1.0) {
	NewText(ID, CodeConv::ANSItoWIDE(str), x, y, scale, width);
}

/* ������I�u�W�F�N�g�̌�n�� */
void TextRenderer::DelText(unsigned int ID) {
	deleteSprite();
	delete StringData[ID]; StringData[ID] = nullptr;
	reconstruct();
}

/* �X�v���C�g�č\�z */
void TextRenderer::reconstruct() {
	deleteSprite();
	for (auto k = StringData.begin(); k != StringData.end(); ++k) {
		
	}
}

/* �X�v���C�g���폜���� */
void TextRenderer::deleteSprite() {
	for (auto k = SpriteData.begin(); k != SpriteData.end(); ++k) {
		if ((*k)->sprite) {
			(*k)->sprite->Release(); (*k)->sprite = nullptr;
		}
	}
	SpriteData.clear();
}
