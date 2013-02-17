#pragma once

#include "../table.h"
#include <bitset>

namespace mihajong_graphic {

class GameTableScreen::TehaiReconst {
private:
	ShowTile* TileTexture;
	GameTableScreen* caller;
private:
	static const unsigned int HandLength = 13;
	static const unsigned int HandPosH = (TableSize - ShowTile::VertTileWidth * (HandLength - 1)) / 2;
	static const unsigned int HandPosV = DeckPosV - 144;
	static const int tileCursorOff = -3; // è”vƒJ[ƒ\ƒ‹–³Œø
	int tileCursor; // è”vƒJ[ƒ\ƒ‹
	std::bitset<NUM_OF_TILES_IN_HAND> tileEnabled;
public:
	void Reconstruct(const GameTable* gameStat, PLAYER_ID targetPlayer); // è”v‚ÌÄ\’z
	void Render();
	explicit TehaiReconst(GameTableScreen* parent);
	~TehaiReconst();
public:
	int getTileCursor() {return tileCursor;}
	void setTileCursor(int val = tileCursorOff) {tileCursor = val;}
	bool isCursorEnabled() {return tileCursor != tileCursorOff;}
	int incrTileCursor() {return ++tileCursor;}
	int decrTileCursor() {return --tileCursor;}
public:
	void enable() {tileEnabled.set();}
	void enable(unsigned tileID) {tileEnabled[tileID] = true;}
	void disable() {tileEnabled.reset();}
	void disable(unsigned tileID) {tileEnabled[tileID] = false;}
	bool isEnabled(unsigned tileID) {return tileEnabled[tileID];}
private: // ŠÔŠÖŒW
	std::uint64_t currTime();
};

}
