#include "table.h"
#include "../scrmanip.h"

GameTableScreen::GameTableScreen(ScreenManipulator* const manipulator) : Scene(manipulator) {
	if (FAILED(D3DXCreateTextureFromFileEx(caller->getDevice(), _T("data\\frame.png"), 768, 768, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0x00000000, nullptr, nullptr, &tBorder)))
		throw _T("テクスチャの生成に失敗しました");
	if (FAILED(D3DXCreateSprite(caller->getDevice(), &sBorder)))
		throw _T("スプライトの生成に失敗しました");
}

GameTableScreen::~GameTableScreen() {
	if (tBorder) tBorder->Release();
	if (sBorder) sBorder->Release();
}

void GameTableScreen::Render() {
	RECT rect = {0, 0, 768, 768};
	D3DXVECTOR3 Center(0, 0, 0);
	D3DXVECTOR3 Pos(0.0f, 0.0f, 0);
	sBorder->Begin(D3DXSPRITE_ALPHABLEND);
	sBorder->Draw(tBorder, &rect, &Center, &Pos, 0xffffffff);
	sBorder->End();
}
