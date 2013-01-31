#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::TehaiReconst {
private:
	ShowTile* TileTexture;
	GameTableScreen* caller;
private:
	static const unsigned int HandLength = 13;
	static const unsigned int HandPosH = (TableSize - ShowTile::VertTileWidth * (HandLength - 1)) / 2;
	static const unsigned int HandPosV = DeckPosV - 144;
	static const int tileCursorOff = -3; // 手牌カーソル無効時
	int tileCursor; // 手牌カーソル
public:
	void Reconstruct(const GameTable* gameStat, PLAYER_ID targetPlayer); // 手牌の再構築
	void Render();
	explicit TehaiReconst(GameTableScreen* parent);
	~TehaiReconst();
public:
	int getTileCursor() {return tileCursor;}
	void setTileCursor(int val = tileCursorOff) {tileCursor = val;}
	bool isCursorEnabled() {return tileCursor != tileCursorOff;}
	int incrTileCursor() {return ++tileCursor;}
	int decrTileCursor() {return --tileCursor;}
};

}
