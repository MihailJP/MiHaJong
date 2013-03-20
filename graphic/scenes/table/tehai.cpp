#include <cmath>
#include "../../pi.h"

#include "tehai.h"
#include "../../utils.h"

#include "color.h"

namespace mihajong_graphic {

using utils::playerRelative;

/* Žè”v‚ð•\Ž¦‚·‚é */
void GameTableScreen::TehaiReconst::Reconstruct(const GameTable* gameStat, PlayerID targetPlayer) {
	ShowTehai::Reconstruct(gameStat, targetPlayer,
		[](seatRelative seat) -> std::tuple<int, int> {
			switch (seat) {
				case sOpposite: return std::make_tuple(            HandPosH,             HandPosV);
				case sLeft:     return std::make_tuple(            HandPosV,             HandPosH);
				case sRight:    return std::make_tuple(TableSize - HandPosV,             HandPosH);
				case sSelf:     return std::make_tuple(            HandPosH, TableSize - HandPosV);
				default:        return std::make_tuple(0                   , 0                   );
			}
		}, playerRelative(targetPlayer, gameStat->PlayerID),
		[this](int i) -> D3DCOLOR {
			Color tileColor; tileColor.rgbaAsOneValue = 0xffffffff;
			if (tileCursor == i) {
				const double Zeit = (double)(myTimer.currTime() % 9000000ULL);
				tileColor.rgbaAsStruct.g = (unsigned)((double)tileColor.rgbaAsStruct.g * (sin(Zeit / 450000.0 * M_PI) / 5.0 + 0.75));
				tileColor.rgbaAsStruct.b = (unsigned)((double)tileColor.rgbaAsStruct.b * (sin(Zeit / 450000.0 * M_PI) / 5.0 + 0.75));
			}
			if (!tileEnabled[i]) { // ˆÃ“]ˆ—
				tileColor.rgbaAsStruct.r /= 3;
				tileColor.rgbaAsStruct.g /= 3;
				tileColor.rgbaAsStruct.b /= 3;
			}
			return tileColor.rgbaAsOneValue;
		}, [this](const int* x, const int* y, int i) -> void {
			const Region nullRegion = {0, 0, -1, -1};
			if (caller->regions.size() <= i) caller->regions.resize(i + 1, nullRegion);
			if ((x != nullptr) && (y != nullptr)) {
				const Region newRegion = {
					(*x) - ShowTile::VertTileWidth / 2, (*y) - ShowTile::VertTileHeight / 2,
					(*x) + ShowTile::VertTileWidth / 2, (*y) + ShowTile::VertTileHeight / 2,
				};
				caller->regions[i] = newRegion;
			} else {
				caller->regions[i] = nullRegion;
			}
		});
}

void GameTableScreen::TehaiReconst::Render() {
	if (tileCursor != tileCursorOff)
		Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
	TileTexture->Render();
}

GameTableScreen::TehaiReconst::TehaiReconst(GameTableScreen* parent) : ShowTehai(parent->caller->getDevice()) {
	caller = parent;
	tileCursor = tileCursorOff;
	tileEnabled.set();
}

GameTableScreen::TehaiReconst::~TehaiReconst() {
}

}
