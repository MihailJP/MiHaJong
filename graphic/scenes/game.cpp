#include "game.h"
#include "../scrmanip.h"
#include "../resource.h"
#include "../init.h"
#include "../loadtex.h"

void TableProtoScene::LoadTexture(LPDIRECT3DTEXTURE9* texture, LPCTSTR resource, unsigned width, unsigned height) {
	::LoadTexture(caller->getDevice(), texture, resource, width, height);
	TileTexture = new ShowTile(caller->getDevice());
}
void TableProtoScene::InitSprite(LPD3DXSPRITE* sprite) {
	if (FAILED(D3DXCreateSprite(caller->getDevice(), sprite)))
		throw _T("�X�v���C�g�̐����Ɏ��s���܂���");
}

TableProtoScene::TableProtoScene(ScreenManipulator* const manipulator) : Scene(manipulator) {
	LoadTexture(&tSideBar, MAKEINTRESOURCE(IDB_PNG_SDBAR), 256, 768); InitSprite(&sSideBar);
}

TableProtoScene::~TableProtoScene() {
	if (tSideBar) tSideBar->Release();
	if (sSideBar) sSideBar->Release();
	delete TileTexture;
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
