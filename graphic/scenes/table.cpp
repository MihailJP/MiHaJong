#include "table.h"
#include "../scrmanip.h"
#include "../resource.h"

GameTableScreen::GameTableScreen(ScreenManipulator* const manipulator) : TableProtoScene(manipulator) {
	LoadTexture(&tBorder, MAKEINTRESOURCE(IDB_PNG_TBLBORDER), 768, 768); InitSprite(&sBorder);
	LoadTexture(&tBaize, MAKEINTRESOURCE(IDB_PNG_TBLBAIZE), 674, 674); InitSprite(&sBaize);
	for (int i = 2; i < 36; i += 2) {
		TileTexture->NewTile(i  , BambooOne, Normal, 96 + 32*(i/2), 88, UpsideDown, Reverse);
		TileTexture->NewTile(i+1, BambooOne, Normal, 96 + 32*(i/2), 77, UpsideDown, Reverse);
	}
	for (int i = 0; i < 34; i += 2) {
		TileTexture->NewTile(36+i  , BambooOne, Normal, 103, 127 + 32*(i/2), Clockwise, Reverse);
		TileTexture->NewTile(36+i+1, BambooOne, Normal, 103, 116 + 32*(i/2), Clockwise, Reverse);
	}
	for (int i = 2; i < 36; i += 2) {
		TileTexture->NewTile(72+i  , BambooOne, Normal, 679, 81 + 32*(i/2), Clockwise, Reverse);
		TileTexture->NewTile(72+i+1, BambooOne, Normal, 679, 70 + 32*(i/2), Clockwise, Reverse);
	}
	for (int i = 0; i < 34; i += 2) {
		TileTexture->NewTile(108+i  , BambooOne, Normal, 142 + 32*(i/2), 665, UpsideDown, Reverse);
		TileTexture->NewTile(108+i+1, BambooOne, Normal, 142 + 32*(i/2), 654, UpsideDown, Reverse);
	}
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
