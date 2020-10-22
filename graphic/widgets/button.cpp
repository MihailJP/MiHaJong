#include "button.h"
#include "../loadtex.h"
#include "../resource.h"
#include <cassert>
#include "../sprite.h"
#include "../matrix.h"
#include "../../common/strcode.h"
#include "../except.h"

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
	const int xpos = static_cast<int>(static_cast<float>(X + (6.0f * Width / 156.0f)) / Geometry::WindowScale());
	const int ypos = static_cast<int>(static_cast<float>(Y + (6.0f * Height / 48.0f)) / Geometry::WindowScale());
	const unsigned w = myTextRenderer->strWidthByCols(caption);
	myTextRenderer->NewText(ButtonID, caption,
		(w < 8) ? xpos + static_cast<int>(9.0f * (8.0f - static_cast<float>(w)) * (static_cast<float>(Width) / 156.0f / Geometry::WindowScale())) : xpos, ypos,
		Height / 48.0f / Geometry::WindowScale(),
		((w > 8) ? 8.0f / static_cast<float>(w) : 1.0f) * (adjustWidth ? Scene::WidthRate() : 1.0f),
		(std::get<0>(mySprites[ButtonID]) == ButtonStat::clear) ? 0x3fffffff : 0xffffffff);
}

void ButtonPic::setButton(unsigned ButtonID, ButtonStat stat, int X, int Y, unsigned Width, unsigned Height, ArgbColor color, const CodeConv::tstring& caption, bool adjustWidth) {
	if (mySprites.size() <= ButtonID)
		mySprites.resize(static_cast<std::size_t>(ButtonID) + 1, std::make_tuple(ButtonStat::absent, 0, 0, 0, 0, 0, _T(""), false));
	assert(stat != ButtonStat::absent);
	mySprites[ButtonID] = std::make_tuple(stat, X, Y, Width, Height, color, CodeConv::tstring(caption), adjustWidth);
	setText(ButtonID);
}

void ButtonPic::setButton(unsigned ButtonID, ButtonStat stat) {
	if (mySprites.size() <= ButtonID)
		throw UninitializedObject("ボタンが初期化されていません");
	mySprites[ButtonID] = std::make_tuple(stat,
		std::get<1>(mySprites[ButtonID]), std::get<2>(mySprites[ButtonID]),
		std::get<3>(mySprites[ButtonID]), std::get<4>(mySprites[ButtonID]),
		std::get<5>(mySprites[ButtonID]), std::get<6>(mySprites[ButtonID]),
		std::get<7>(mySprites[ButtonID]));
	setText(ButtonID);
}

void ButtonPic::Render() {
	for (const auto& k : mySprites) {
		if (std::get<0>(k) == ButtonStat::absent) continue;
		int X = std::get<1>(k), Y = std::get<2>(k);
		unsigned width = std::get<3>(k), height = std::get<4>(k);
		const TransformMatrix mat(getMatrix(
#ifdef WITH_DIRECTX
			static_cast<float>(X),
			static_cast<float>(Y),
			static_cast<float>(width) / 156.0f,
			static_cast<float>(height) / 48.0f,
#else
			// DirectXとは基準が異なる？　OpenGLの場合ここは単位行列のままでよい
			0.0f,
			0.0f,
			1.0f,
			1.0f,
#endif
			0.0f,
			0.0f,
			0.0f,
			1.0f / Geometry::WindowScale(),
			1.0f / Geometry::WindowScale()
		));
		const RECT rect = {0, 52 * (static_cast<int>(std::get<0>(k)) - 1), 156, 52 * (static_cast<int>(std::get<0>(k)) - 1) + 48};
		SpriteRenderer::instantiate(myDevice)->ShowSprite(myTexture, X, Y, width, height, std::get<5>(k) | 0xff000000, &rect, 0, 0, &mat);
	}
	myTextRenderer->Render();
}

}
