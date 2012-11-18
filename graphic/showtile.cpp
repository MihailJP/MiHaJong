#include "showtile.h"
#include "resource.h"
#include "loadtex.h"
#include "geometry.h"

ShowTile::ShowTile(LPDIRECT3DDEVICE9 device) {
	myDevice = device;
	LoadTexture(myDevice, &TileTexture, MAKEINTRESOURCE(IDB_PNG_TILE), TextureWidth, TextureHeight);
}
ShowTile::~ShowTile() {
	for (unsigned int i = 0; i < mySprites.size(); i++)
		if (mySprites[i] && (mySprites[i]->sprite)) DelTile(i);
	if (TileTexture) TileTexture->Release();
}

/* �V�K�̔v�I�u�W�F�N�g���쐬���� */
void ShowTile::NewTile(unsigned int ID, tileCode tile, doraCol red, int x, int y, TileDirection direction, TileSide side) {
	if (mySprites.size() <= ID) mySprites.resize(ID + 1, nullptr); // �z��̊g��
	if (!mySprites[ID]) DelTile(ID); // ���ɑ��݂����ꍇ
	mySprites[ID] = new TileDescriptor;
	mySprites[ID]->tile = tile; mySprites[ID]->red = red;
	mySprites[ID]->X = x; mySprites[ID]->Y = y;
	mySprites[ID]->direction = direction; mySprites[ID]->side = side;
	if (FAILED(D3DXCreateSprite(myDevice, &mySprites[ID]->sprite)))
		throw _T("�X�v���C�g�̐����Ɏ��s���܂���");
}

/* �v�I�u�W�F�N�g�̌�n�� */
void ShowTile::DelTile(unsigned int ID) {
	if (mySprites[ID]) {
		if (mySprites[ID]->sprite) mySprites[ID]->sprite->Release();
		delete mySprites[ID]; mySprites[ID] = nullptr;
	}
}

/* �����_�����O */
void ShowTile::RenderVert(TileDescriptor* tile, RECT* rect) {
	D3DXMATRIX matrix; D3DXMatrixIdentity(&matrix);
	D3DXMatrixScaling(&matrix, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXVECTOR3 Center(VertTileWidth/2, VertTileHeight/2, 0);
	D3DXVECTOR3 Pos((float)tile->X, (float)tile->Y, 0);
	tile->sprite->Begin(D3DXSPRITE_ALPHABLEND);
	tile->sprite->SetTransform(&matrix);
	tile->sprite->Draw(TileTexture, rect, &Center, &Pos, 0xffffffff);
	tile->sprite->End();
}
void ShowTile::RenderHori(TileDescriptor* tile, RECT* rect) {
	D3DXMATRIX matrix; D3DXMatrixIdentity(&matrix);
	D3DXMatrixScaling(&matrix, Geometry::WindowScale(), Geometry::WindowScale(), 0.0f);
	D3DXVECTOR3 Center(HoriTileWidth/2, HoriTileHeight/2, 0);
	D3DXVECTOR3 Pos((float)tile->X, (float)tile->Y, 0);
	tile->sprite->Begin(D3DXSPRITE_ALPHABLEND);
	tile->sprite->SetTransform(&matrix);
	tile->sprite->Draw(TileTexture, rect, &Center, &Pos, 0xffffffff);
	tile->sprite->End();
}
void ShowTile::Render() {
	for (auto k = mySprites.begin(); k != mySprites.end(); ++k) {
		if ((*k) && ((*k)->sprite)) {
			if (((*k)->direction == Portrait) || ((*k)->direction == UpsideDown)) {
				/* Portrait alignment */
				RECT rect = {
					((*k)->tile % 10) * (VertTileWidth + TexturePadding),
					((*k)->tile / 10) * (VertTileHeight + TexturePadding),
					((*k)->tile % 10 + 1) * (VertTileWidth + TexturePadding),
					((*k)->tile / 10 + 1) * (VertTileHeight + TexturePadding)
				};
				RECT rectrev = {
					((int)BackSide % 10) * (VertTileWidth + TexturePadding),
					((int)BackSide / 10) * (VertTileHeight + TexturePadding),
					((int)BackSide % 10 + 1) * (VertTileWidth + TexturePadding),
					((int)BackSide / 10 + 1) * (VertTileHeight + TexturePadding)
				};
				switch ((*k)->side) {
				case Obverse:
					if ((*k)->direction == UpsideDown) {
						rect.left += (VertTileWidth + TexturePadding) * TileCols;
						rect.right += (VertTileWidth + TexturePadding) * TileCols;
					}
					RenderVert(*k, &rect);
					break;
				case Upright:
					if ((*k)->direction == UpsideDown) {
						rectrev.left += (VertTileWidth + TexturePadding) * TileCols * 2;
						rectrev.right += (VertTileWidth + TexturePadding) * TileCols * 2;
						RenderVert(*k, &rectrev);
					} else {
						rect.left += (VertTileWidth + TexturePadding) * TileCols * 2;
						rect.right += (VertTileWidth + TexturePadding) * TileCols * 2;
						RenderVert(*k, &rect);
					}
					break;
				case Reverse:
					RenderVert(*k, &rectrev);
					break;
				}
			}
			else {
				/* Landscape alignment */
				RECT rect = {
					((*k)->tile % 10) * (HoriTileWidth + TexturePadding),
					((*k)->tile / 10) * (HoriTileHeight + TexturePadding) + (VertTileHeight + TexturePadding) * TileRows,
					((*k)->tile % 10 + 1) * (HoriTileWidth + TexturePadding),
					((*k)->tile / 10 + 1) * (HoriTileHeight + TexturePadding) + (VertTileHeight + TexturePadding) * TileRows
				};
				RECT rectrev = {
					((int)BackSide % 10) * (HoriTileWidth + TexturePadding),
					((int)BackSide / 10) * (HoriTileHeight + TexturePadding) + (VertTileHeight + TexturePadding) * TileRows,
					((int)BackSide % 10 + 1) * (HoriTileWidth + TexturePadding) ,
					((int)BackSide / 10 + 1) * (HoriTileHeight + TexturePadding) + (VertTileHeight + TexturePadding) * TileRows
				};
				RECT rectside = {
					2 * TileRows * (HoriTileWidth + TexturePadding),
					(VertTileHeight + TexturePadding) * TileRows,
					2 * TileRows * (HoriTileWidth + TexturePadding) + SideTileWidth+ TexturePadding,
					(VertTileWidth + TexturePadding) * TileRows + (SideTileHeight + TexturePadding)
				};
				switch ((*k)->side) {
				case Obverse:
					if ((*k)->direction == Clockwise) {
						rect.left += HoriTileWidth * TileCols;
						rect.right += HoriTileWidth * TileCols;
					}
					RenderHori(*k, &rect);
					break;
				case Upright:
					if ((*k)->direction == Clockwise) {
						rectside.left += SideTileWidth;
						rectside.right += SideTileWidth;
					}
					RenderHori(*k, &rectside);
					break;
				case Reverse:
					RenderHori(*k, &rectrev);
					break;
				}
			}
		}
	}
}
