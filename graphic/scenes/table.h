#ifndef GRAPHIC_SCENES_TABLE_H
#define GRAPHIC_SCENES_TABLE_H

#include "proto.h"

class GameTableScreen : public Scene {
private:
	LPDIRECT3DTEXTURE9 tBorder; // 枠テクスチャ
	LPD3DXSPRITE sBorder; // 枠スプライト
public:
	GameTableScreen(ScreenManipulator* const manipulator);
	~GameTableScreen();
	void Render();
};

#endif
