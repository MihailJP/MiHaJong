#include "test.h"
#include "../scrmanip.h"

TestScene::TestScene(ScreenManipulator* const manipulator) : Scene(manipulator) {
	D3DXCreateFont(caller->getDevice(), 40, 0, FW_HEAVY, 1, false, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, _T("‚l‚r ƒSƒVƒbƒN"), &pFont);
}

TestScene::~TestScene() {
	if (pFont) pFont->Release();
}

void TestScene::Render() {
	RECT rect; SetRect(&rect, 400, 300, 50, 50);
	pFont->DrawText(nullptr, _T("Hello World!"), -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
}
