#pragma once

#include "../table.h"
#include <bitset>
#include "tehai0.h"

namespace mihajong_graphic {

class GameTableScreen::TehaiReconst : public ShowTehai {
private:
	GameTableScreen* caller;
private:
	static const unsigned int HandPosH = (TableSize - ShowTile::VertTileWidth * (HandLength - 1)) / 2;
	static const unsigned int HandPosV = DeckPosV - 144;
	int tileCursor; // è”vƒJ[ƒ\ƒ‹
	std::bitset<NumOfTilesInHand> tileEnabled;
public:
	void Reconstruct(const GameTable* gameStat, PlayerID targetPlayer); // è”v‚ÌÄ\’z
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
