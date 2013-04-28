#pragma once

#include <windows.h>
#include "directx.h"
#include <tchar.h>
#include <vector>
#include "../common/tilecode.h"
#include "../common/gametbl.h"
#include "../common/seatrank.h"
#include "../common/machihai.h"

namespace mihajong_graphic {

using namespace mihajong_structs;

enum TileSide {Obverse, Upright, Reverse,};
enum TileDirection {Portrait, Withershins, UpsideDown, Clockwise,};

class ShowTile {
public:
	static const unsigned int VertTileWidth  = 32;
	static const unsigned int VertTileHeight = 54;
	static const unsigned int SideTileWidth  = 20;
	static const unsigned int SideTileHeight = 74;
	static const unsigned int HoriTileWidth  = 46;
	static const unsigned int HoriTileHeight = 41;
	static const unsigned int TileCols = 10;
	static const unsigned int TileRows = 13;
	static const unsigned int TexturePadding = 3;
private:
	struct TileDescriptor;
	DevicePtr myDevice;
	TexturePtr TileTexture;
	std::vector<TileDescriptor> mySprites;
	void RenderTile(TileDescriptor* tile, RECT* rect, int CenterX, int CenterY);
	void RenderVert(TileDescriptor* tile, RECT* rect);
	void RenderHori(TileDescriptor* tile, RECT* rect);
	void RenderSide(TileDescriptor* tile, RECT* rect);
public:
	void NewTile(unsigned int ID, TileCode tile, doraCol red, int x, int y, TileDirection direction, TileSide side, ArgbColor filterCol = 0xffffffff);
	void DelTile(unsigned int ID);
	void Render();
	ShowTile(DevicePtr device);
	~ShowTile();
};

struct ShowTile::TileDescriptor {
	bool exist;
	TileCode tile;
	doraCol red;
	int X, Y;
	TileDirection direction;
	TileSide side;
	ArgbColor color;
};

}
