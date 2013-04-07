#include "showtile.h"
#include "resource.h"
#include "loadtex.h"
#include "geometry.h"
#include "sprite.h"
#include "rule.h"

namespace mihajong_graphic {

ShowTile::ShowTile(LPDIRECT3DDEVICE9 device) {
	myDevice = device;
	LoadTexture(myDevice, &TileTexture,
		rules::chkPreference("tile", "black_tile") ? MAKEINTRESOURCE(IDB_PNG_TILE_BLACK) : MAKEINTRESOURCE(IDB_PNG_TILE));
}
ShowTile::~ShowTile() {
	if (TileTexture) TileTexture->Release();
}

/* 新規の牌オブジェクトを作成する */
void ShowTile::NewTile(unsigned int ID, TileCode tile, doraCol red, int x, int y, TileDirection direction, TileSide side, D3DCOLOR filterCol) {
	const TileDescriptor empty = {false, NoTile, Normal, 0, 0, Portrait, Obverse, 0xffffffff};
	if (mySprites.size() <= ID) mySprites.resize(ID + 1, empty); // 配列の拡張
	mySprites[ID].exist = true;
	mySprites[ID].tile = tile; mySprites[ID].red = red;
	mySprites[ID].X = x; mySprites[ID].Y = y;
	mySprites[ID].direction = direction; mySprites[ID].side = side;
	mySprites[ID].color = filterCol;
}

/* 牌オブジェクトの後始末 */
void ShowTile::DelTile(unsigned int ID) {
	if (mySprites.size() > ID) {
		mySprites[ID].exist = false;
	}
}

/* レンダリング */
void ShowTile::RenderTile(TileDescriptor* tile, RECT* rect, int CenterX, int CenterY) {
	SpriteRenderer::instantiate(myDevice)->ShowSprite(TileTexture, tile->X, tile->Y,
		CenterX*2, CenterY*2, tile->color, rect, CenterX, CenterY);
}
void ShowTile::RenderVert(TileDescriptor* tile, RECT* rect) {
	RenderTile(tile, rect, VertTileWidth/2, VertTileHeight/2);
}
void ShowTile::RenderHori(TileDescriptor* tile, RECT* rect) {
	RenderTile(tile, rect, HoriTileWidth/2, HoriTileHeight/2);
}
void ShowTile::RenderSide(TileDescriptor* tile, RECT* rect) {
	RenderTile(tile, rect, SideTileWidth/2, SideTileHeight/2);
}
void ShowTile::Render() {
	for (auto k = mySprites.begin(); k != mySprites.end(); ++k) {
		if (k->exist) {
			if ((k->direction == Portrait) || (k->direction == UpsideDown)) {
				/* Portrait alignment */
				RECT rect = {
					((k->tile + k->red * TileNonflowerMax) % 10) * (VertTileWidth + TexturePadding),
					((k->tile + k->red * TileNonflowerMax) / 10) * (VertTileHeight + TexturePadding),
					((k->tile + k->red * TileNonflowerMax) % 10 + 1) * (VertTileWidth + TexturePadding) - TexturePadding,
					((k->tile + k->red * TileNonflowerMax) / 10 + 1) * (VertTileHeight + TexturePadding) - TexturePadding,
				};
				RECT rectrev = {
					((int)BackSide % 10) * (VertTileWidth + TexturePadding),
					((int)BackSide / 10) * (VertTileHeight + TexturePadding),
					((int)BackSide % 10 + 1) * (VertTileWidth + TexturePadding) - TexturePadding,
					((int)BackSide / 10 + 1) * (VertTileHeight + TexturePadding) - TexturePadding,
				};
				switch (k->side) {
				case Obverse:
					if (k->direction == UpsideDown) {
						rect.left += (VertTileWidth + TexturePadding) * TileCols;
						rect.right += (VertTileWidth + TexturePadding) * TileCols;
					}
					RenderVert(&*k, &rect);
					break;
				case Upright:
					if (k->direction == UpsideDown) {
						rectrev.left += (VertTileWidth + TexturePadding) * TileCols * 2;
						rectrev.right += (VertTileWidth + TexturePadding) * TileCols * 2;
						RenderVert(&*k, &rectrev);
					} else {
						rect.left += (VertTileWidth + TexturePadding) * TileCols * 2;
						rect.right += (VertTileWidth + TexturePadding) * TileCols * 2;
						RenderVert(&*k, &rect);
					}
					break;
				case Reverse:
					RenderVert(&*k, &rectrev);
					break;
				}
			}
			else {
				/* Landscape alignment */
				RECT rect = {
					((k->tile + k->red * TileNonflowerMax) % 10) * (HoriTileWidth + TexturePadding),
					((k->tile + k->red * TileNonflowerMax) / 10) * (HoriTileHeight + TexturePadding) + (VertTileHeight + TexturePadding) * TileRows,
					((k->tile + k->red * TileNonflowerMax) % 10 + 1) * (HoriTileWidth + TexturePadding) - TexturePadding,
					((k->tile + k->red * TileNonflowerMax) / 10 + 1) * (HoriTileHeight + TexturePadding) + (VertTileHeight + TexturePadding) * TileRows - TexturePadding,
				};
				RECT rectrev = {
					((int)BackSide % 10) * (HoriTileWidth + TexturePadding),
					((int)BackSide / 10) * (HoriTileHeight + TexturePadding) + (VertTileHeight + TexturePadding) * TileRows,
					((int)BackSide % 10 + 1) * (HoriTileWidth + TexturePadding) - TexturePadding,
					((int)BackSide / 10 + 1) * (HoriTileHeight + TexturePadding) + (VertTileHeight + TexturePadding) * TileRows - TexturePadding,
				};
				RECT rectside = {
					2 * TileCols * (HoriTileWidth + TexturePadding),
					(VertTileHeight + TexturePadding) * TileRows,
					2 * TileCols * (HoriTileWidth + TexturePadding) + SideTileWidth,
					(VertTileHeight + TexturePadding) * TileRows + SideTileHeight,
				};
				switch (k->side) {
				case Obverse:
					if (k->direction == Clockwise) {
						rect.left += (HoriTileWidth + TexturePadding) * TileCols;
						rect.right += (HoriTileWidth + TexturePadding) * TileCols;
					}
					RenderHori(&*k, &rect);
					break;
				case Upright:
					if (k->direction == Clockwise) {
						rectside.left += SideTileWidth + TexturePadding;
						rectside.right += SideTileWidth + TexturePadding;
					}
					RenderSide(&*k, &rectside);
					break;
				case Reverse:
					RenderHori(&*k, &rectrev);
					break;
				}
			}
		}
	}
}

}
