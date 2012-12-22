#pragma once

#include "proto.h"
#include "../showtile.h"
#include <array>
#include <tuple>

namespace mihajong_graphic {

class TableProtoScene : public Scene {
protected:
	ShowTile* TileTexture;
	LPDIRECT3DTEXTURE9 tSideBar; LPD3DXSPRITE sSideBar; // �T�C�h�o�[
	void LoadTexture(LPDIRECT3DTEXTURE9* texture, LPCTSTR resource, unsigned width, unsigned height); // �e�N�X�`���ǂݍ���
	void InitSprite(LPD3DXSPRITE* sprite); // �X�v���C�g������
	void ShowSprite(LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height); // �X�v���C�g�\��
	void ShowSidebar();
public:
	TableProtoScene(ScreenManipulator* const manipulator);
	virtual ~TableProtoScene();
};

}
