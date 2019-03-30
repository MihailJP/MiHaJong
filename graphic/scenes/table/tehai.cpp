#include <cmath>
#include "../../pi.h"

#include "tehai.h"
#include "../../utils.h"

#include "color.h"

namespace mihajong_graphic {

using utils::playerRelative;

/* 手牌を表示する */
void GameTableScreen::TehaiReconst::Reconstruct(const GameTable* gameStat, PlayerID targetPlayer) {
	const unsigned int h = HandPosH, v = HandPosV;
	ShowTehai::Reconstruct(gameStat, targetPlayer,
		[h, v](seatRelative seat) -> std::tuple<int, int> {
			switch (seat) {
				case sOpposite: return std::make_tuple(            h,             v);
				case sLeft:     return std::make_tuple(            v,             h);
				case sRight:    return std::make_tuple(TableSize - v,             h);
				case sSelf:     return std::make_tuple(            h, TableSize - v);
				default:        return std::make_tuple(0            , 0            );
			}
		}, playerRelative(targetPlayer, gameStat->PlayerID),
		[this](int i) -> ArgbColor {
			Color tileColor; tileColor.rgbaAsOneValue = 0xffffffff;
			if (firstChosenTile == i) {
				if (tileCursor == i) {
					const double Zeit = static_cast<double>(myTimer.currTime() % 9000000ULL);
					tileColor.rgbaAsStruct.r = static_cast<unsigned>(static_cast<double>(tileColor.rgbaAsStruct.g) * (sin(Zeit / 450000.0 * M_PI) / 5.0 + 0.75));
					tileColor.rgbaAsStruct.b = static_cast<unsigned>(static_cast<double>(tileColor.rgbaAsStruct.b) * (sin(Zeit / 450000.0 * M_PI) / 5.0 + 0.75));
				} else {
					tileColor.rgbaAsStruct.r /= 2;
					tileColor.rgbaAsStruct.b /= 2;
				}
			} else {
				if (tileCursor == i) {
					const double Zeit = static_cast<double>(myTimer.currTime() % 9000000ULL);
					tileColor.rgbaAsStruct.g = static_cast<unsigned>(static_cast<double>(tileColor.rgbaAsStruct.g) * (sin(Zeit / 450000.0 * M_PI) / 5.0 + 0.75));
					tileColor.rgbaAsStruct.b = static_cast<unsigned>(static_cast<double>(tileColor.rgbaAsStruct.b) * (sin(Zeit / 450000.0 * M_PI) / 5.0 + 0.75));
				}
				if (!tileEnabled[i]) { // 暗転処理
					tileColor.rgbaAsStruct.r /= 3;
					tileColor.rgbaAsStruct.g /= 3;
					tileColor.rgbaAsStruct.b /= 3;
				}
			}
			return tileColor.rgbaAsOneValue;
		}, [this](const int* x, const int* y, int i) -> void {
			if ((x != nullptr) && (y != nullptr))
				caller->setRegion(i,
				(*x) - ShowTile::VertTileWidth / 2, (*y) - ShowTile::VertTileHeight / 2,
				(*x) + ShowTile::VertTileWidth / 2, (*y) + ShowTile::VertTileHeight / 2);
			else
				caller->setRegion(i, NullRegion);
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
	firstChosenTile = tileCursorOff;
	tileEnabled.set();
}

GameTableScreen::TehaiReconst::~TehaiReconst() {
}

}
