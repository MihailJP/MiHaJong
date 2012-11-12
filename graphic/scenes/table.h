#ifndef GRAPHIC_SCENES_TABLE_H
#define GRAPHIC_SCENES_TABLE_H

#include "game.h"

class GameTableScreen : public TableProtoScene {
private:
	LPDIRECT3DTEXTURE9 tBorder; LPD3DXSPRITE sBorder; // ��̘g
	LPDIRECT3DTEXTURE9 tBaize; LPD3DXSPRITE sBaize; // ���ђn
public:
	GameTableScreen(ScreenManipulator* const manipulator);
	~GameTableScreen();
	void Render();
};

#endif
