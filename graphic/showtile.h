#ifndef GRAPHIC_SHOWTILE_H
#define GRAPHIC_SHOWTILE_H

#include <Windows.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <tchar.h>
#include <vector>
#include "../mjcore/mjimport.h"

enum TileSide {Obverse, Upright, Reverse,};
enum TileDirection {Portrait, Withershins, UpsideDown, Clockwise,};

class ShowTile {
public:
	static const unsigned int TextureWidth   = 960;
	static const unsigned int TextureHeight  = 1235;
	static const unsigned int VertTileWidth  = 32;
	static const unsigned int VertTileHeight = 54;
	static const unsigned int SideTileWidth  = 20;
	static const unsigned int SideTileHeight = VertTileHeight;
	static const unsigned int HoriTileWidth  = 46;
	static const unsigned int HoriTileHeight = 41;
	static const unsigned int TileCols = 10;
	static const unsigned int TileRows = 13;
private:
	struct TileDescriptor;
	LPDIRECT3DDEVICE9 myDevice;
	LPDIRECT3DTEXTURE9 TileTexture;
	std::vector<TileDescriptor*> mySprites;
	void RenderVert(TileDescriptor* tile, RECT* rect);
	void RenderHori(TileDescriptor* tile, RECT* rect);
public:
	void NewTile(unsigned int ID, tileCode tile, doraCol red, int x, int y, TileDirection direction, TileSide side);
	void DelTile(unsigned int ID);
	void Render();
	ShowTile(LPDIRECT3DDEVICE9 device);
	~ShowTile();
};

struct ShowTile::TileDescriptor {
	LPD3DXSPRITE sprite;
	tileCode tile;
	doraCol red;
	int X, Y;
	TileDirection direction;
	TileSide side;
};

#endif
