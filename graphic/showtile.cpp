#include "showtile.h"
#include "resource.h"
#include "loadtex.h"
#include "geometry.h"
#include "sprite.h"
#include "rule.h"

namespace mihajong_graphic {

ShowTile::ShowTile(DevicePtr device) {
	myDevice = device;
	LoadTexture(myDevice, &TileTexture,
		preferences::blackTile ? MAKEINTRESOURCE(IDB_PNG_TILE_BLACK) : MAKEINTRESOURCE(IDB_PNG_TILE));
}
ShowTile::~ShowTile() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (TileTexture) TileTexture->Release();
#endif
}

/* 新規の牌オブジェクトを作成する */
void ShowTile::NewTile(unsigned int ID, Tile tile, int x, int y, TileDirection direction, TileSide side, ArgbColor filterCol) {
	constexpr TileDescriptor empty = {false, Tile(), 0, 0, TileDirection::portrait, TileSide::obverse, 0xffffffff};
	if (mySprites.size() <= ID) mySprites.resize(static_cast<std::size_t>(ID) + 1, empty); // 配列の拡張
	mySprites[ID].exist = true;
	mySprites[ID].tile = tile;
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
void ShowTile::RenderTile(const TileDescriptor* tile, const RECT* rect, int CenterX, int CenterY) {
	SpriteRenderer::instantiate(myDevice)->ShowSprite(TileTexture, tile->X, tile->Y,
		CenterX*2, CenterY*2, tile->color, rect, CenterX, CenterY);
}
void ShowTile::RenderVert(const TileDescriptor* tile, const RECT* rect) {
	RenderTile(tile, rect, VertTileWidth/2, VertTileHeight/2);
}
void ShowTile::RenderHori(const TileDescriptor* tile, const RECT* rect) {
	RenderTile(tile, rect, HoriTileWidth/2, HoriTileHeight/2);
}
void ShowTile::RenderSide(const TileDescriptor* tile, const RECT* rect) {
	RenderTile(tile, rect, SideTileWidth/2, SideTileHeight/2);
}
void ShowTile::Render() {
	for (auto& k : mySprites) {
		const int32_t kVal = k.tile.tile + static_cast<int>(k.tile.red) * TileNonflowerMax;
		if (k.exist) {
			if ((k.direction == TileDirection::portrait) || (k.direction == TileDirection::upsideDown)) {
				/* TileDirection::portrait alignment */
				RECT rect = {
					static_cast<int32_t>((kVal % 10) * (VertTileWidth + TexturePadding)),
					static_cast<int32_t>((kVal / 10) * (VertTileHeight + TexturePadding)),
					static_cast<int32_t>((kVal % 10 + 1) * (VertTileWidth + TexturePadding) - TexturePadding),
					static_cast<int32_t>((kVal / 10 + 1) * (VertTileHeight + TexturePadding) - TexturePadding),
				};
				RECT rectrev = {
					(static_cast<int>(BackSide) % 10) * (VertTileWidth + TexturePadding),
					(static_cast<int>(BackSide) / 10) * (VertTileHeight + TexturePadding),
					(static_cast<int>(BackSide) % 10 + 1) * (VertTileWidth + TexturePadding) - TexturePadding,
					(static_cast<int>(BackSide) / 10 + 1) * (VertTileHeight + TexturePadding) - TexturePadding,
				};
				switch (k.side) {
				case TileSide::obverse:
					if (k.direction == TileDirection::upsideDown) {
						rect.left += (VertTileWidth + TexturePadding) * TileCols;
						rect.right += (VertTileWidth + TexturePadding) * TileCols;
					}
					RenderVert(&k, &rect);
					break;
				case TileSide::upright:
					if (k.direction == TileDirection::upsideDown) {
						rectrev.left += (VertTileWidth + TexturePadding) * TileCols * 2;
						rectrev.right += (VertTileWidth + TexturePadding) * TileCols * 2;
						RenderVert(&k, &rectrev);
					} else {
						rect.left += (VertTileWidth + TexturePadding) * TileCols * 2;
						rect.right += (VertTileWidth + TexturePadding) * TileCols * 2;
						RenderVert(&k, &rect);
					}
					break;
				case TileSide::reverse:
					RenderVert(&k, &rectrev);
					break;
				}
			}
			else {
				/* Landscape alignment */
				RECT rect = {
					static_cast<int32_t>((kVal % 10) * (HoriTileWidth + TexturePadding)),
					static_cast<int32_t>((kVal / 10) * (HoriTileHeight + TexturePadding) + (VertTileHeight + TexturePadding) * TileRows),
					static_cast<int32_t>((kVal % 10 + 1) * (HoriTileWidth + TexturePadding) - TexturePadding),
					static_cast<int32_t>((kVal / 10 + 1) * (HoriTileHeight + TexturePadding) + (VertTileHeight + TexturePadding) * TileRows - TexturePadding),
				};
				RECT rectrev = {
					(static_cast<int>(BackSide) % 10) * (HoriTileWidth + TexturePadding),
					(static_cast<int>(BackSide) / 10) * (HoriTileHeight + TexturePadding) + (VertTileHeight + TexturePadding) * TileRows,
					(static_cast<int>(BackSide) % 10 + 1) * (HoriTileWidth + TexturePadding) - TexturePadding,
					(static_cast<int>(BackSide) / 10 + 1) * (HoriTileHeight + TexturePadding) + (VertTileHeight + TexturePadding) * TileRows - TexturePadding,
				};
				RECT rectside = {
					2 * TileCols * (HoriTileWidth + TexturePadding),
					(VertTileHeight + TexturePadding) * TileRows,
					2 * TileCols * (HoriTileWidth + TexturePadding) + SideTileWidth,
					(VertTileHeight + TexturePadding) * TileRows + SideTileHeight,
				};
				switch (k.side) {
				case TileSide::obverse:
					if (k.direction == TileDirection::clockwise) {
						rect.left += (HoriTileWidth + TexturePadding) * TileCols;
						rect.right += (HoriTileWidth + TexturePadding) * TileCols;
					}
					RenderHori(&k, &rect);
					break;
				case TileSide::upright:
					if (k.direction == TileDirection::clockwise) {
						rectside.left += SideTileWidth + TexturePadding;
						rectside.right += SideTileWidth + TexturePadding;
					}
					RenderSide(&k, &rectside);
					break;
				case TileSide::reverse:
					RenderHori(&k, &rectrev);
					break;
				}
			}
		}
	}
}

}
