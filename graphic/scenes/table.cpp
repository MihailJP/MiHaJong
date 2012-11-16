#include "table.h"
#include "../scrmanip.h"
#include "../resource.h"

GameTableScreen::GameTableScreen(ScreenManipulator* const manipulator) : TableProtoScene(manipulator) {
	LoadTexture(&tBorder, MAKEINTRESOURCE(IDB_PNG_TBLBORDER), 768, 768); InitSprite(&sBorder);
	LoadTexture(&tBaize, MAKEINTRESOURCE(IDB_PNG_TBLBAIZE), 674, 674); InitSprite(&sBaize);
	TileTexture->NewTile(0, BambooOne, Normal, 96, 123, Portrait, Obverse);
	TileTexture->NewTile(1, BambooOne, Normal, 128, 123, Portrait, Upright);
	TileTexture->NewTile(2, BambooOne, Normal, 160, 123, Portrait, Reverse);
	TileTexture->NewTile(3, BambooOne, Normal, 192, 123, UpsideDown, Obverse);
}

GameTableScreen::~GameTableScreen() {
	if (tBorder) tBorder->Release();
	if (sBorder) sBorder->Release();
	if (tBaize) tBaize->Release();
	if (sBaize) sBaize->Release();
}

void GameTableScreen::Render() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0); // バッファクリア
	ShowSidebar();
	ShowSprite(sBaize, tBaize, 47, 47, 674, 674);
	ShowSprite(sBorder, tBorder, 0, 0, 768, 768);
	TileTexture->Render();
}
