#pragma once

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#else /*_WIN32*/
#include "../common/strcode.h"
#endif /*_WIN32*/
#include "directx.h"
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
	static constexpr unsigned int VertTileWidth  = 32;
	static constexpr unsigned int VertTileHeight = 54;
	static constexpr unsigned int SideTileWidth  = 20;
	static constexpr unsigned int SideTileHeight = 74;
	static constexpr unsigned int HoriTileWidth  = 46;
	static constexpr unsigned int HoriTileHeight = 41;
	static constexpr unsigned int TileCols = 10;
	static constexpr unsigned int TileRows = 13;
	static constexpr unsigned int TexturePadding = 3;
private:
	struct TileDescriptor;
	DevicePtr myDevice;
	TexturePtr TileTexture;
	std::vector<TileDescriptor> mySprites;
	void RenderTile(const TileDescriptor* tile, const RECT* rect, int CenterX, int CenterY);
	void RenderVert(const TileDescriptor* tile, const RECT* rect);
	void RenderHori(const TileDescriptor* tile, const RECT* rect);
	void RenderSide(const TileDescriptor* tile, const RECT* rect);
public:
	void NewTile(unsigned int ID, Tile tile, int x, int y, TileDirection direction, TileSide side, ArgbColor filterCol = 0xffffffff);
	void DelTile(unsigned int ID);
	void Render();
	ShowTile(DevicePtr device);
	ShowTile(const ShowTile&) = delete; // Delete unexpected copy constructor
	ShowTile& operator= (const ShowTile&) = delete; // Delete unexpected assign operator
	~ShowTile();
};

struct ShowTile::TileDescriptor {
	bool exist;
	Tile tile;
	int X, Y;
	TileDirection direction;
	TileSide side;
	ArgbColor color;
};

}
