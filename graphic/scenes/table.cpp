#include "table.h"
#include "../scrmanip.h"
#include "../resource.h"

GameTableScreen::GameTableScreen(ScreenManipulator* const manipulator) : TableProtoScene(manipulator) {
	LoadTexture(&tBorder, MAKEINTRESOURCE(IDB_PNG_TBLBORDER), 1080, 1080); InitSprite(&sBorder);
	LoadTexture(&tBaize, MAKEINTRESOURCE(IDB_PNG_TBLBAIZE), 1080, 1080); InitSprite(&sBaize);
	/* �R�v */
	for (int i = 2; i < 36; i += 2) { /* �Ζʑ��̎R */
		TileTexture->NewTile(i  , BambooOne, Normal, DeckPosH + ShowTile::VertTileWidth*(i/2-1), DeckPosV, UpsideDown, Reverse);
		TileTexture->NewTile(i+1, BambooOne, Normal, DeckPosH + ShowTile::VertTileWidth*(i/2-1), DeckPosV - TileThickness, UpsideDown, Reverse);
	}
	for (int i = 0; i < 34; i += 2) { /* ��Ƒ��̎R */
		TileTexture->NewTile(36+i  , BambooOne, Normal, DeckPosV, DeckPosH + ShowTile::VertTileWidth*(i/2), Clockwise, Reverse);
		TileTexture->NewTile(36+i+1, BambooOne, Normal, DeckPosV, DeckPosH - TileThickness + ShowTile::VertTileWidth*(i/2), Clockwise, Reverse);
	}
	for (int i = 2; i < 36; i += 2) { /* ���Ƒ��̎R */
		TileTexture->NewTile(72+i  , BambooOne, Normal, TableSize - DeckPosV, DeckPosH + ShowTile::VertTileWidth*(i/2-1), Withershins, Reverse);
		TileTexture->NewTile(72+i+1, BambooOne, Normal, TableSize - DeckPosV, DeckPosH - TileThickness + ShowTile::VertTileWidth*(i/2-1), Withershins, Reverse);
	}
	for (int i = 0; i < 34; i += 2) { /* �����̎R */
		TileTexture->NewTile(108+i  , BambooOne, Normal, DeckPosH + ShowTile::VertTileWidth*(i/2), TableSize - DeckPosV, Portrait, Reverse);
		TileTexture->NewTile(108+i+1, BambooOne, Normal, DeckPosH + ShowTile::VertTileWidth*(i/2), TableSize - DeckPosV - TileThickness, Portrait, Reverse);
	}
	/* ��v */
	for (int i = 0; i < HandLength; i++) { /* �����̎R */
		TileTexture->NewTile(144+i , WhiteDragon, Normal, HandPosH + ShowTile::VertTileWidth*i, TableSize - HandPosV, Portrait, Upright);
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
		D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0); // �o�b�t�@�N���A
	ShowSidebar();
	ShowSprite(sBaize, tBaize, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	ShowSprite(sBorder, tBorder, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	TileTexture->Render();
}
