#include "button.h"
#include "../loadtex.h"
#include "../resource.h"
#include <cassert>
#include "../sprite.h"
#include "../../common/strcode.h"

namespace mihajong_graphic {

ButtonPic::ButtonPic(DevicePtr device) {
	myDevice = device;
	LoadTexture(myDevice, &myTexture, MAKEINTRESOURCE(IDB_PNG_BUTTON));
	myTextRenderer = new TextRenderer(myDevice);
}

ButtonPic::~ButtonPic() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	myTexture->Release();
#endif
	delete myTextRenderer;
}

void ButtonPic::setText(unsigned ButtonID) {
	const int X = std::get<1>(mySprites[ButtonID]), Y = std::get<2>(mySprites[ButtonID]);
	const unsigned Width = std::get<3>(mySprites[ButtonID]), Height = std::get<4>(mySprites[ButtonID]);
	const CodeConv::tstring& caption = std::get<6>(mySprites[ButtonID]);
	const bool adjustWidth = std::get<7>(mySprites[ButtonID]);
	const int xpos = (int)((float)(X + (6.0f * Width / 156.0f)) / Geometry::WindowScale());
	const int ypos = (int)((float)(Y + (6.0f * Height / 48.0f)) / Geometry::WindowScale());
	const unsigned w = myTextRenderer->strWidthByCols(caption);
	myTextRenderer->NewText(ButtonID, caption,
		(w < 8) ? xpos + static_cast<int>(9.0f * (8.0f - static_cast<float>(w)) * (static_cast<float>(Width) / 156.0f / Geometry::WindowScale())) : xpos, ypos,
		Height / 48.0f / Geometry::WindowScale(),
		((w > 8) ? 8.0f / (float)w : 1.0f) * (adjustWidth ? Scene::WidthRate() : 1.0f),
		(std::get<0>(mySprites[ButtonID]) == clear) ? 0x3fffffff : 0xffffffff);
}

void ButtonPic::setButton(unsigned ButtonID, ButtonStat stat, int X, int Y, unsigned Width, unsigned Height, ArgbColor color, const CodeConv::tstring& caption, bool adjustWidth) {
	if (mySprites.size() <= ButtonID)
		mySprites.resize(ButtonID + 1, std::make_tuple(absent, 0, 0, 0, 0, 0, _T(""), false));
	assert(stat != absent);
	mySprites[ButtonID] = std::make_tuple(stat, X, Y, Width, Height, color, CodeConv::tstring(caption), adjustWidth);
	setText(ButtonID);
}

void ButtonPic::setButton(unsigned ButtonID, ButtonStat stat) {
	if (mySprites.size() <= ButtonID)
		throw _T("ボタンが初期化されていません");
	mySprites[ButtonID] = std::make_tuple(stat,
		std::get<1>(mySprites[ButtonID]), std::get<2>(mySprites[ButtonID]),
		std::get<3>(mySprites[ButtonID]), std::get<4>(mySprites[ButtonID]),
		std::get<5>(mySprites[ButtonID]), std::get<6>(mySprites[ButtonID]),
		std::get<7>(mySprites[ButtonID]));
	setText(ButtonID);
}

void ButtonPic::Render() {
	for (const auto& k : mySprites) {
		if (std::get<0>(k) == absent) continue;
		int X = std::get<1>(k), Y = std::get<2>(k);
		unsigned width = std::get<3>(k), height = std::get<4>(k);
#if defined(_WIN32) && defined(WITH_DIRECTX)
		TransformMatrix mat, mat2; D3DXMatrixIdentity(&mat); D3DXMatrixIdentity(&mat2);
		D3DXMatrixTranslation(&mat2, (float)(-X), (float)(-Y), 0.0f); D3DXMatrixMultiply(&mat, &mat, &mat2);
		D3DXMatrixScaling(&mat2, (float)width / 156.0f, (float)height / 48.0f, 0.0f); D3DXMatrixMultiply(&mat, &mat, &mat2);
		D3DXMatrixTranslation(&mat2, (float)X, (float)Y, 0.0f); D3DXMatrixMultiply(&mat, &mat, &mat2);
#else
		glPushMatrix(); glLoadIdentity();
		// DirectXとは基準が異なる？　OpenGLの場合ここは単位行列のままでよい
		TransformMatrix mat; glGetFloatv(GL_MODELVIEW_MATRIX, &mat[0]);
		glPopMatrix();
#endif
		RECT rect = {0, 52 * (std::get<0>(k) - 1), 156, 52 * (std::get<0>(k) - 1) + 48};
		SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X, Y, width, height, std::get<5>(k) | 0xff000000, &rect, 0, 0, &mat);
	}
	myTextRenderer->Render();
}

}
