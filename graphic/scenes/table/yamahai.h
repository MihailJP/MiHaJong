#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::YamahaiReconst {
private:
	ShowTile* TileTexture;
	GameTableScreen* caller;
public:
	void Reconstruct(const GameTable* gameStat, PlayerID targetPlayer, PlayerID trueTargetPlayer); // 山牌の再構築
	void Render();
	explicit YamahaiReconst(GameTableScreen* parent);
	YamahaiReconst(const YamahaiReconst&) = delete; // Delete unexpected copy constructor
	YamahaiReconst& operator= (const YamahaiReconst&) = delete; // Delete unexpected assign operator
	~YamahaiReconst();
};

}
