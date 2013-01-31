#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::YamahaiReconst {
private:
	ShowTile* TileTexture;
	GameTableScreen* caller;
public:
	void Reconstruct(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer); // R”v‚ÌÄ\’z
	void Render();
	explicit YamahaiReconst(GameTableScreen* parent);
	~YamahaiReconst();
};

}
