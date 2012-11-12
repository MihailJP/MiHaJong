#ifndef GRAPHIC_SCENES_GAME_H
#define GRAPHIC_SCENES_GAME_H

#include "proto.h"

class TableProtoScene : public Scene {
protected:
	LPDIRECT3DTEXTURE9 tSideBar; LPD3DXSPRITE sSideBar; // サイドバー
	void LoadTexture(LPDIRECT3DTEXTURE9* texture, LPTSTR filename, unsigned width, unsigned height); // テクスチャ読み込み
	void InitSprite(LPD3DXSPRITE* sprite); // スプライト初期化
	void ShowSprite(LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height); // スプライト表示
	void ShowSidebar();
public:
	TableProtoScene(ScreenManipulator* const manipulator);
	virtual ~TableProtoScene();
};

#endif
