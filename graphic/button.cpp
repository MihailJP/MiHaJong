#include "button.h"
#include "loadtex.h"
#include "resource.h"
#include <cassert>
#include "sprite.h"
#include "../common/strcode.h"

namespace mihajong_graphic {

ButtonPic::ButtonPic(LPDIRECT3DDEVICE9 device) {
	myDevice = device;
	LoadTexture(myDevice, &myTexture, MAKEINTRESOURCE(IDB_PNG_BUTTON), 160, 156);
	myTextRenderer = new TextRenderer(myDevice);
}

ButtonPic::~ButtonPic() {
	myTexture->Release();
	delete myTextRenderer;
}

void ButtonPic::setText(unsigned ButtonID) {
	const int X = std::get<1>(mySprites[ButtonID]), Y = std::get<2>(mySprites[ButtonID]);
	const unsigned Width = std::get<3>(mySprites[ButtonID]), Height = std::get<4>(mySprites[ButtonID]);
	const CodeConv::tstring& caption = std::get<6>(mySprites[ButtonID]);
	const int xpos = (int)((float)(X + (6.0f * Width / 156.0f)) / Geometry::WindowScale());
	const int ypos = (int)((float)(Y + (6.0f * Height / 48.0f)) / Geometry::WindowScale());
#ifdef _UNICODE
	unsigned w = 0;
	for (auto k = caption.begin(); k != caption.end(); ++k) {
		if (*k <= _T('\x7f')) w += 1;
		else w += 2;
	}
#else
	unsigned w = caption.size();
#endif
	myTextRenderer->NewText(ButtonID, caption,
		(w < 8) ? xpos + (9 * (8 - w) * (Width / 156.0f / Geometry::WindowScale())) : xpos, ypos,
		Height / 48.0f / Geometry::WindowScale(), (w > 8) ? 8.0f / (float)w : 1.0f,
		(std::get<0>(mySprites[ButtonID]) == clear) ? 0x3fffffff : 0xffffffff);
}

void ButtonPic::setButton(unsigned ButtonID, ButtonStat stat, int X, int Y, unsigned Width, unsigned Height, D3DCOLOR color, const CodeConv::tstring& caption) {
	if (mySprites.size() <= ButtonID)
		mySprites.resize(ButtonID + 1, std::make_tuple(absent, 0, 0, 0, 0, 0, _T("")));
	assert(stat != absent);
	mySprites[ButtonID] = std::make_tuple(stat, X, Y, Width, Height, color, CodeConv::tstring(caption));
	setText(ButtonID);
}

void ButtonPic::setButton(unsigned ButtonID, ButtonStat stat) {
	if (mySprites.size() <= ButtonID)
		throw _T("ƒ{ƒ^ƒ“‚ª‰Šú‰»‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
	mySprites[ButtonID] = std::make_tuple(stat,
		std::get<1>(mySprites[ButtonID]), std::get<2>(mySprites[ButtonID]),
		std::get<3>(mySprites[ButtonID]), std::get<4>(mySprites[ButtonID]),
		std::get<5>(mySprites[ButtonID]), std::get<6>(mySprites[ButtonID]));
	setText(ButtonID);
}

void ButtonPic::Render() {
	for (auto k = mySprites.begin(); k != mySprites.end(); ++k) {
		if (std::get<0>(*k) == absent) continue;
		int X = std::get<1>(*k), Y = std::get<2>(*k);
		unsigned width = std::get<3>(*k), height = std::get<4>(*k);
		D3DXMATRIX mat, mat2; D3DXMatrixIdentity(&mat); D3DXMatrixIdentity(&mat2);
		D3DXMatrixTranslation(&mat2, (float)(-X), (float)(-Y), 0.0f); D3DXMatrixMultiply(&mat, &mat, &mat2);
		D3DXMatrixScaling(&mat2, (float)width / 156.0f, (float)height / 48.0f, 0.0f); D3DXMatrixMultiply(&mat, &mat, &mat2);
		D3DXMatrixTranslation(&mat2, (float)X, (float)Y, 0.0f); D3DXMatrixMultiply(&mat, &mat, &mat2);
		RECT rect = {0, 52 * (std::get<0>(*k) - 1), 156, 52 * (std::get<0>(*k) - 1) + 48};
		SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X, Y, width, height, std::get<5>(*k) | 0xff000000, &rect, 0, 0, &mat);
	}
	myTextRenderer->Render();
}

}
