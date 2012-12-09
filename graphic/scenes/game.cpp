#include "game.h"
#include "../scrmanip.h"
#include "../resource.h"
#include "../init.h"
#include "../loadtex.h"
#include "../sprite.h"
#include "../gametbl.h"
#include <cassert>

namespace mihajong_graphic {

// -------------------------------------------------------------------------

void TableProtoScene::LoadTexture(LPDIRECT3DTEXTURE9* texture, LPCTSTR resource, unsigned width, unsigned height) {
	mihajong_graphic::LoadTexture(caller->getDevice(), texture, resource, width, height);
	TileTexture = new ShowTile(caller->getDevice());
}
void TableProtoScene::InitSprite(LPD3DXSPRITE* sprite) {
	if (FAILED(D3DXCreateSprite(caller->getDevice(), sprite)))
		throw _T("スプライトの生成に失敗しました");
}

TableProtoScene::TableProtoScene(ScreenManipulator* const manipulator) : Scene(manipulator) {
	LoadTexture(&tSideBar, MAKEINTRESOURCE(IDB_PNG_SDBAR), 960, 1080); InitSprite(&sSideBar);
	myDiceRenderer = new DiceRenderer(manipulator->getDevice());
}

TableProtoScene::~TableProtoScene() {
	delete myDiceRenderer;
	if (tSideBar) tSideBar->Release();
	if (sSideBar) sSideBar->Release();
	delete TileTexture;
}

void TableProtoScene::ShowSprite(LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height) {
	SpriteRenderer::ShowSprite(sprite, texture, X, Y, Width, Height);
}

void TableProtoScene::ShowSidebar() {
	ShowSprite(sSideBar, tSideBar, Geometry::BaseSize, 0, Geometry::SidebarWidth(), 1080);
}

// -------------------------------------------------------------------------

TableProtoScene::DiceRenderer::DiceRenderer(LPDIRECT3DDEVICE9 device) {
	myDevice = device;
	mihajong_graphic::LoadTexture(myDevice, &DiceTexture, MAKEINTRESOURCE(IDB_PNG_DICE), TextureWidth, TextureHeight);
	for (auto k = mySprite.begin(); k != mySprite.end(); ++k) {
		LPD3DXSPRITE mySprite;
		if (FAILED(D3DXCreateSprite(myDevice, &mySprite)))
			throw _T("スプライトの生成に失敗しました");
		*k = std::make_tuple(mySprite, 0, 0);
	}
}

TableProtoScene::DiceRenderer::~DiceRenderer() {
	for (auto k = mySprite.begin(); k != mySprite.end(); ++k)
		if (std::get<0>(*k)) std::get<0>(*k)->Release();
	if (DiceTexture) DiceTexture->Release();
}

void TableProtoScene::DiceRenderer::Render() {
	unsigned i = 0;
	for (auto k = mySprite.begin(); k != mySprite.end(); ++k) {
		assert(std::get<0>(*k) != nullptr);
		if (std::get<0>(*k)) {
			assert((GameStatus::gameStat()->Dice[i].Number >= 1) && (GameStatus::gameStat()->Dice[i].Number <= 6));
			RECT rect = {
				(GameStatus::gameStat()->Dice[i].Number - 1) * (TextureCharWidth + TextureCharPadding),
				(GameStatus::gameStat()->Dice[i].Direction) * (TextureCharHeight + TextureCharPadding),
				(GameStatus::gameStat()->Dice[i].Number - 1) * (TextureCharWidth + TextureCharPadding) + TextureCharWidth,
				(GameStatus::gameStat()->Dice[i].Direction) * (TextureCharHeight + TextureCharPadding) + TextureCharHeight,
			};
			SpriteRenderer::ShowSprite(std::get<0>(*k), DiceTexture, std::get<1>(*k), std::get<2>(*k),
				TextureCharWidth, TextureCharHeight, 0xffffffff, &rect, TextureCharCenterX, TextureCharCenterY);
		}
		++i;
	}
}

// -------------------------------------------------------------------------

}
