#pragma once

#include "proto.h"
#include "../showtile.h"
#include <array>
#include <tuple>

namespace mihajong_graphic {

class TableProtoScene : public Scene {
protected:
	ShowTile* TileTexture;
	class DiceRenderer;
	DiceRenderer* myDiceRenderer;
	LPDIRECT3DTEXTURE9 tSideBar; LPD3DXSPRITE sSideBar; // �T�C�h�o�[
	void LoadTexture(LPDIRECT3DTEXTURE9* texture, LPCTSTR resource, unsigned width, unsigned height); // �e�N�X�`���ǂݍ���
	void InitSprite(LPD3DXSPRITE* sprite); // �X�v���C�g������
	void ShowSprite(LPD3DXSPRITE sprite, LPDIRECT3DTEXTURE9 texture, int X, int Y, int Width, int Height); // �X�v���C�g�\��
	void ShowSidebar();
public:
	TableProtoScene(ScreenManipulator* const manipulator);
	virtual ~TableProtoScene();
};

class TableProtoScene::DiceRenderer {
private:
	static const unsigned TextureWidth = 156u;
	static const unsigned TextureHeight = 144u;
	static const unsigned TextureCharWidth = 24u;
	static const unsigned TextureCharHeight = 34u;
	static const unsigned TextureCharPadding = 2u;
	static const signed TextureCharCenterX = 12;
	static const signed TextureCharCenterY = 12;
	LPDIRECT3DDEVICE9 myDevice;
	LPDIRECT3DTEXTURE9 DiceTexture;
	std::array<std::tuple<LPD3DXSPRITE, int, int>, 2> mySprite;
public:
	void Render();
	explicit DiceRenderer(LPDIRECT3DDEVICE9 device);
	~DiceRenderer();
};

}
