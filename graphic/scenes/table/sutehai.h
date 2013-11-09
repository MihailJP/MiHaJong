#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::SutehaiReconst {
private:
	ShowTile* TileTexture;
	GameTableScreen* caller;
private:
	static unsigned int DiscardLineLength() {return GameStatus::gameStat()->chkGameType(GuobiaoMJ) ? 7 : 6;}
	static unsigned int DiscardPosH() {return (TableSize - ShowTile::VertTileWidth * (DiscardLineLength() - 1)) / 2;}
	static const unsigned int DiscardPosV = (TableSize / 2) - 130;
	void ReconstructSutehai_portrait(const GameTable* gameStat, PlayerID targetPlayer,
		unsigned tileID, unsigned& tilePosCol, unsigned& tilePosRow, bool& shiftPos);
	void ReconstructSutehai_rotated(const GameTable* gameStat, PlayerID targetPlayer,
		unsigned tileID, unsigned& tilePosCol, unsigned& tilePosRow, bool& shiftPos);
public:
	void Reconstruct(const GameTable* gameStat, PlayerID targetPlayer); // éÃîvÇÃçƒç\íz
	void Render();
	explicit SutehaiReconst(GameTableScreen* parent);
	SutehaiReconst(const SutehaiReconst&) = delete; // Delete unexpected copy constructor
	SutehaiReconst& operator= (const SutehaiReconst&) = delete; // Delete unexpected assign operator
	~SutehaiReconst();
};

}
