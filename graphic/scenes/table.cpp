#include "table.h"
#include "../scrmanip.h"

void GameTableScreen::LoadTexture(LPDIRECT3DTEXTURE9* texture, LPTSTR filename, unsigned width, unsigned height) {
	if (FAILED(D3DXCreateTextureFromFileEx(caller->getDevice(), filename, width, height, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0x00000000, nullptr, nullptr, texture)))
		throw _T("テクスチャの生成に失敗しました");
}
void GameTableScreen::InitSprite(LPD3DXSPRITE* sprite) {
	if (FAILED(D3DXCreateSprite(caller->getDevice(), sprite)))
		throw _T("スプライトの生成に失敗しました");
}

GameTableScreen::GameTableScreen(ScreenManipulator* const manipulator) : Scene(manipulator) {
	LoadTexture(&tBorder, _T("data\\frame.png"), 768, 768); InitSprite(&sBorder);
	LoadTexture(&tBaize, _T("data\\baize.png"), 768, 768); InitSprite(&sBaize);
}

GameTableScreen::~GameTableScreen() {
	if (tBorder) tBorder->Release();
	if (sBorder) sBorder->Release();
	if (tBaize) tBaize->Release();
	if (sBaize) sBaize->Release();
}

void GameTableScreen::ShowSprite(LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height) {
	RECT rect = {0, 0, Width, Height};
	D3DXVECTOR3 Center(0, 0, 0);
	D3DXVECTOR3 Pos((float)X, (float)Y, 0);
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(texture, &rect, &Center, &Pos, 0xffffffff);
	sprite->End();
}

void GameTableScreen::Render() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0); // バッファクリア
	ShowSprite(sBaize, tBaize, 47, 47, 674, 674);
	ShowSprite(sBorder, tBorder, 0, 0, 768, 768);
}
