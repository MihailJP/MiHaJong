#pragma once

#include "../table.h"
#include <bitset>
#include "tehai0.h"
#include "../../timer.h"

namespace mihajong_graphic {

class GameTableScreen::TehaiReconst : public ShowTehai {
private:
	GameTableScreen* caller;
private:
	static constexpr unsigned int HandPosH = (TableSize - ShowTile::VertTileWidth * (HandLength - 1)) / 2;
	static constexpr unsigned int HandPosV = DeckPosV - 144;
	int tileCursor; // 手牌カーソル
	std::bitset<NumOfTilesInHand> tileEnabled;
public:
	void Reconstruct(const GameTable* gameStat, PlayerID targetPlayer); // 手牌の再構築
	void Render();
	explicit TehaiReconst(GameTableScreen* parent);
	TehaiReconst(const TehaiReconst&) = delete; // Delete unexpected copy constructor
	TehaiReconst& operator= (const TehaiReconst&) = delete; // Delete unexpected assign operator
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
private: // 時間関係
	Timer myTimer;
};

}
