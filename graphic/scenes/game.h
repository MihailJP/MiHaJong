#pragma once

#include "proto.h"
#include "../showtile.h"
#include <array>
#include <tuple>

namespace mihajong_graphic {

class TableProtoScene : public Scene {
protected:
	ShowTile* TileTexture;
	LPDIRECT3DTEXTURE9 tSideBar; LPD3DXSPRITE sSideBar; // サイドバー
	void LoadTexture(LPDIRECT3DTEXTURE9* texture, LPCTSTR resource, unsigned width, unsigned height); // テクスチャ読み込み
	void InitSprite(LPD3DXSPRITE* sprite); // スプライト初期化
	void ShowSprite(LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height); // スプライト表示
	void ShowSidebar();
public:
	TableProtoScene(ScreenManipulator* const manipulator);
	virtual ~TableProtoScene();
};

}
