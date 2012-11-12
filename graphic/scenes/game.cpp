#include "game.h"
#include "../scrmanip.h"

void TableProtoScene::LoadTexture(LPDIRECT3DTEXTURE9* texture, LPTSTR filename, unsigned width, unsigned height) {
	if (FAILED(D3DXCreateTextureFromFileEx(caller->getDevice(), filename, width, height, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0x00000000, nullptr, nullptr, texture)))
		throw _T("テクスチャの生成に失敗しました");
}
void TableProtoScene::InitSprite(LPD3DXSPRITE* sprite) {
	if (FAILED(D3DXCreateSprite(caller->getDevice(), sprite)))
		throw _T("スプライトの生成に失敗しました");
}

TableProtoScene::TableProtoScene(ScreenManipulator* const manipulator) : Scene(manipulator) {
	LoadTexture(&tSideBar, _T("data\\sidebar.png"), 256, 768); InitSprite(&sSideBar);
}

TableProtoScene::~TableProtoScene() {
	if (tSideBar) tSideBar->Release();
	if (sSideBar) sSideBar->Release();
}

void TableProtoScene::ShowSprite(LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height) {
	RECT rect = {0, 0, Width, Height};
	D3DXVECTOR3 Center(0, 0, 0);
	D3DXVECTOR3 Pos((float)X, (float)Y, 0);
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(texture, &rect, &Center, &Pos, 0xffffffff);
	sprite->End();
}

void TableProtoScene::ShowSidebar() {
	ShowSprite(sSideBar, tSideBar, 768, 0, 256, 768);
}
