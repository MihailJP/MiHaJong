#ifndef GRAPHIC_SCENES_TABLE_H
#define GRAPHIC_SCENES_TABLE_H

#include "proto.h"

class GameTableScreen : public Scene {
private:
	LPDIRECT3DTEXTURE9 tBorder; LPD3DXSPRITE sBorder; // 卓の枠
	LPDIRECT3DTEXTURE9 tBaize; LPD3DXSPRITE sBaize; // 羅紗地
	void LoadTexture(LPDIRECT3DTEXTURE9* texture, LPTSTR filename, unsigned width, unsigned height); // テクスチャ読み込み
	void InitSprite(LPD3DXSPRITE* sprite); // スプライト初期化
	void ShowSprite(LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height); // スプライト表示
public:
	GameTableScreen(ScreenManipulator* const manipulator);
	~GameTableScreen();
	void Render();
};

#endif
