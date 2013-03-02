#pragma once

#include "../table.h"
#include "nakihai0.h"

namespace mihajong_graphic {

class GameTableScreen::NakihaiReconst : public ShowNakihai {
private:
	GameTableScreen* caller;
private:
	unsigned int MeldPosH() {return TableSize - 31;}
	unsigned int MeldPosV() {return DeckPosV - 166;}
	unsigned int MPosVVert() {return TableSize - MeldPosV();}
	unsigned int MPosVHorU() {return TableSize - MeldPosV() - ShowTile::VertTileWidth + 6;}
	unsigned int MPosVHorL() {return TableSize - MeldPosV() + 6;}
	std::tuple<std::function<unsigned (unsigned)>, std::function<int (unsigned)>, std::function<int (unsigned)>, TileDirection, TileDirection, TileDirection>
		playerPosition(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID,
		unsigned h1, unsigned h2, unsigned h3, unsigned h4, unsigned v1, unsigned v2, unsigned v3, unsigned v4,
		bool r1, bool r2, bool r3, bool r4);
public:
	void Reconstruct(const GameTable* gameStat, PlayerID targetPlayer); // ñ¬Ç¢ÇΩîvÇÃçƒç\íz
	void Render();
	explicit NakihaiReconst(GameTableScreen* parent);
	~NakihaiReconst();
};

}
