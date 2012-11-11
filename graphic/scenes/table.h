#ifndef GRAPHIC_SCENES_TABLE_H
#define GRAPHIC_SCENES_TABLE_H

#include "proto.h"

class GameTableScreen : public Scene {
private:
	LPDIRECT3DTEXTURE9 tBorder; // �g�e�N�X�`��
	LPD3DXSPRITE sBorder; // �g�X�v���C�g
public:
	GameTableScreen(ScreenManipulator* const manipulator);
	~GameTableScreen();
	void Render();
};

#endif
