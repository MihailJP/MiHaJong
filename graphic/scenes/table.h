#ifndef GRAPHIC_SCENES_TABLE_H
#define GRAPHIC_SCENES_TABLE_H

#include "proto.h"

class GameTableScreen : public Scene {
private:
	LPDIRECT3DTEXTURE9 tBorder; LPD3DXSPRITE sBorder; // ��̘g
	LPDIRECT3DTEXTURE9 tBaize; LPD3DXSPRITE sBaize; // ���ђn
	void LoadTexture(LPDIRECT3DTEXTURE9* texture, LPTSTR filename, unsigned width, unsigned height); // �e�N�X�`���ǂݍ���
	void InitSprite(LPD3DXSPRITE* sprite); // �X�v���C�g������
	void ShowSprite(LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height); // �X�v���C�g�\��
public:
	GameTableScreen(ScreenManipulator* const manipulator);
	~GameTableScreen();
	void Render();
};

#endif
