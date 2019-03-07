#pragma once

#include "../table.h"
#include "nakihai0.h"

namespace mihajong_graphic {

class GameTableScreen::NakihaiReconst : public ShowNakihai {
private:
	GameTableScreen* caller;
private:
	const unsigned int MeldPosH() override {return TableSize - 31;}
	const unsigned int MeldPosV() override {return DeckPosV - 166;}
	const unsigned int MPosVVert() override {return TableSize - MeldPosV();}
	const unsigned int MPosVHorU() override {return TableSize - MeldPosV() - ShowTile::VertTileWidth + 6;}
	const unsigned int MPosVHorL() override {return TableSize - MeldPosV() + 6;}
	std::tuple<std::function<unsigned (unsigned)>, std::function<int (unsigned)>, std::function<int (unsigned)>, TileDirection, TileDirection, TileDirection>
		playerPosition(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID,
		unsigned h1, unsigned h2, unsigned h3, unsigned h4, unsigned v1, unsigned v2, unsigned v3, unsigned v4,
		bool r1, bool r2, bool r3, bool r4);
public:
	void Reconstruct(const GameTable* gameStat, PlayerID targetPlayer); // 鳴いた牌の再構築
	void Render();
	explicit NakihaiReconst(GameTableScreen* parent);
	NakihaiReconst(const NakihaiReconst&) = delete; // Delete unexpected copy constructor
	NakihaiReconst& operator= (const NakihaiReconst&) = delete; // Delete unexpected assign operator
	~NakihaiReconst();
};

}
