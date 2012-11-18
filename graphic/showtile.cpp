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

/* 新規の牌オブジェクトを作成する */
void ShowTile::NewTile(unsigned int ID, tileCode tile, doraCol red, int x, int y, TileDirection direction, TileSide side) {
	if (mySprites.size() <= ID) mySprites.resize(ID + 1, nullptr); // 配列の拡張
	if (!mySprites[ID]) DelTile(ID); // 既に存在した場合
	mySprites[ID] = new TileDescriptor;
	mySprites[ID]->tile = tile; mySprites[ID]->red = red;
	mySprites[ID]->X = x; mySprites[ID]->Y = y;
	mySprites[ID]->direction = direction; mySprites[ID]->side = side;
	if (FAILED(D3DXCreateSprite(myDevice, &mySprites[ID]->sprite)))
		throw _T("スプライトの生成に失敗しました");
}

/* 牌オブジェクトの後始末 */
void ShowTile::DelTile(unsigned int ID) {
	if (mySprites[ID]) {
		if (mySprites[ID]->sprite) mySprites[ID]->sprite->Release();
		delete mySprites[ID]; mySprites[ID] = nullptr;
	}
}

/* レンダリング */
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
					((*k)->tile % 10) * VertTileWidth, ((*k)->tile / 10) * VertTileHeight,
					((*k)->tile % 10 + 1) * VertTileWidth, ((*k)->tile / 10 + 1) * VertTileHeight};
				RECT rectrev = {
					((int)BackSide % 10) * VertTileWidth, ((int)BackSide / 10) * VertTileHeight,
					((int)BackSide % 10 + 1) * VertTileWidth, ((int)BackSide / 10 + 1) * VertTileHeight};
				switch ((*k)->side) {
				case Obverse:
					if ((*k)->direction == UpsideDown) {
						rect.left += VertTileWidth * TileCols;
						rect.right += VertTileWidth * TileCols;
					}
					RenderVert(*k, &rect);
					break;
				case Upright:
					if ((*k)->direction == UpsideDown) {
						rectrev.left += VertTileWidth * TileCols * 2;
						rectrev.right += VertTileWidth * TileCols * 2;
						RenderVert(*k, &rectrev);
					} else {
						rect.left += VertTileWidth * TileCols * 2;
						rect.right += VertTileWidth * TileCols * 2;
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
					((*k)->tile % 10) * HoriTileWidth,
					((*k)->tile / 10) * HoriTileHeight + VertTileHeight * TileRows,
					((*k)->tile % 10 + 1) * HoriTileWidth,
					((*k)->tile / 10 + 1) * HoriTileHeight + VertTileHeight * TileRows};
				RECT rectrev = {
					((int)BackSide % 10) * HoriTileWidth,
					((int)BackSide / 10) * HoriTileHeight + VertTileHeight * TileRows,
					((int)BackSide % 10 + 1) * HoriTileWidth ,
					((int)BackSide / 10 + 1) * HoriTileHeight + VertTileHeight * TileRows};
				RECT rectside = {
					2 * TileRows * HoriTileWidth,
					VertTileHeight * TileRows,
					2 * TileRows * HoriTileWidth + SideTileWidth,
					VertTileWidth * TileRows + SideTileHeight};
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
