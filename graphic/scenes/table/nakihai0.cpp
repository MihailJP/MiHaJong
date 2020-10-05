#include "nakihai0.h"
//#include "../../gametbl.h"
#include "../../rule.h"
#include "../../utils.h"
#include <cassert>

namespace mihajong_graphic {

using utils::playerRelative;

/* 鳴いた牌を表示する */
void ShowNakihai::NakihaiAnkan(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert(tile->mstat == MeldStat::quadConcealed);
	const TileSide AnkanExpose =
		((gameStat->chkGameType(GameTypeID::guobiaoMJ) || rules::chkRule("ankan_conceal", "closed")) && (gameStat->Player[targetPlayer].HandStat != HandStatCode::exposed)) ?
		TileSide::reverse : TileSide::obverse;
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert;
	std::tie(num, x, y, std::ignore, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHVertR(2), MPosHVertR(3), MPosHVertR(4), MPosVVert(), MPosVVert(), MPosVVert(), MPosVVert(),
		false, false, false, false);
	TileTexture->NewTile(num(0), Tile(tile->tile, tile->red[2]), x(0), y(0), vert, TileSide::reverse);
	TileTexture->NewTile(num(1), Tile(tile->tile, tile->red[0]), x(1), y(1), vert, AnkanExpose);
	TileTexture->NewTile(num(2), Tile(tile->tile, tile->red[1]), x(2), y(2), vert, AnkanExpose);
	TileTexture->NewTile(num(3), Tile(tile->tile, tile->red[3]), x(3), y(3), vert, TileSide::reverse);
}
void ShowNakihai::NakihaiKamicha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert((tile->mstat == MeldStat::tripletExposedLeft) || (tile->mstat == MeldStat::quadAddedLeft) ||
		(tile->mstat == MeldStat::sequenceExposedLower) || (tile->mstat == MeldStat::sequenceExposedMiddle) ||
		(tile->mstat == MeldStat::sequenceExposedUpper));
	const TileCode tileL = (tile->mstat == MeldStat::sequenceExposedMiddle) ? static_cast<TileCode>(tile->tile + 1) : ((tile->mstat == MeldStat::sequenceExposedUpper) ? static_cast<TileCode>(tile->tile + 2) : tile->tile);
	const TileCode tileC = (tile->mstat == MeldStat::sequenceExposedLower) ? static_cast<TileCode>(tile->tile + 1) : tile->tile;
	const TileCode tileR = (tile->mstat == MeldStat::sequenceExposedUpper) ? static_cast<TileCode>(tile->tile + 1) : ((tile->mstat == MeldStat::sequenceExposedLower) || (tile->mstat == MeldStat::sequenceExposedMiddle) ? static_cast<TileCode>(tile->tile + 2) : tile->tile);
	const DoraCol redL = (tile->mstat == MeldStat::sequenceExposedMiddle) ? tile->red[1] : ((tile->mstat == MeldStat::sequenceExposedUpper) ? tile->red[2] : tile->red[0]);
	const DoraCol redC = ((tile->mstat == MeldStat::sequenceExposedMiddle) || (tile->mstat == MeldStat::sequenceExposedUpper)) ? tile->red[0] : tile->red[1];
	const DoraCol redR = (tile->mstat == MeldStat::sequenceExposedUpper) ? tile->red[1] : tile->red[2];
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, hor, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHVertR(2), MPosHHor(3), MPosHHor(3), MPosVVert(), MPosVVert(), MPosVHorU(), MPosVHorL(),
		false, false, true, true);
	TileTexture->NewTile(num(0), Tile(tileR, redR), x(0), y(0), vert, TileSide::obverse);
	TileTexture->NewTile(num(1), Tile(tileC, redC), x(1), y(1), vert, TileSide::obverse);
	if (tile->mstat == MeldStat::quadAddedLeft)
		TileTexture->NewTile(num(2), Tile(tileL, tile->red[3]), x(2), y(2), hor, TileSide::obverse);
	else TileTexture->DelTile(num(2));
	TileTexture->NewTile(num(3), Tile(tileL, redL), x(3), y(3), hor, TileSide::obverse);
}
void ShowNakihai::NakihaiToimen(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert((tile->mstat == MeldStat::tripletExposedCenter) || (tile->mstat == MeldStat::quadAddedCenter));
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, hor, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHHor(2), MPosHHor(2), MPosHVertL(3), MPosVVert(), MPosVHorU(), MPosVHorL(), MPosVVert(),
		false, true, true, false);
	TileTexture->NewTile(num(0), Tile(tile->tile, tile->red[2]), x(0), y(0), vert, TileSide::obverse);
	if (tile->mstat == MeldStat::quadAddedCenter)
		TileTexture->NewTile(num(1), Tile(tile->tile, tile->red[3]), x(1), y(1), hor, TileSide::obverse);
	else TileTexture->DelTile(num(1));
	TileTexture->NewTile(num(2), Tile(tile->tile, tile->red[0]), x(2), y(2), hor, TileSide::obverse);
	TileTexture->NewTile(num(3), Tile(tile->tile, tile->red[1]), x(3), y(3), vert, TileSide::obverse);
}
void ShowNakihai::NakihaiShimocha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert((tile->mstat == MeldStat::tripletExposedRight) || (tile->mstat == MeldStat::quadAddedRight));
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, std::ignore, vert, hor) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHHor(1), MPosHHor(1), MPosHVertL(2), MPosHVertL(3), MPosVHorU(), MPosVHorL(), MPosVVert(), MPosVVert(),
		true, true, false, false);
	if (tile->mstat == MeldStat::quadAddedRight)
		TileTexture->NewTile(num(0), Tile(tile->tile, tile->red[3]), x(0), y(0), hor, TileSide::obverse);
	else TileTexture->DelTile(num(0));
	TileTexture->NewTile(num(1), Tile(tile->tile, tile->red[0]), x(1), y(1), hor, TileSide::obverse);
	TileTexture->NewTile(num(2), Tile(tile->tile, tile->red[2]), x(2), y(2), vert, TileSide::obverse);
	TileTexture->NewTile(num(3), Tile(tile->tile, tile->red[1]), x(3), y(3), vert, TileSide::obverse);
}
void ShowNakihai::MinkanKamicha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert(tile->mstat == MeldStat::quadExposedLeft);
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, hor, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHVertR(2), MPosHVertR(3), MPosHHor(4), MPosVVert(), MPosVVert(), MPosVVert(), MPosVHorL(),
		false, false, false, true);
	TileTexture->NewTile(num(0), Tile(tile->tile, tile->red[3]), x(0), y(0), vert, TileSide::obverse);
	TileTexture->NewTile(num(1), Tile(tile->tile, tile->red[2]), x(1), y(1), vert, TileSide::obverse);
	TileTexture->NewTile(num(2), Tile(tile->tile, tile->red[1]), x(2), y(2), vert, TileSide::obverse);
	TileTexture->NewTile(num(3), Tile(tile->tile, tile->red[0]), x(3), y(3), hor, TileSide::obverse);
}
void ShowNakihai::MinkanToimen(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert(tile->mstat == MeldStat::quadExposedCenter);
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, hor, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHVertR(2), MPosHHor(3), MPosHVertL(4), MPosVVert(), MPosVVert(), MPosVHorL(), MPosVVert(),
		false, false, true, false);
	TileTexture->NewTile(num(0), Tile(tile->tile, tile->red[3]), x(0), y(0), vert, TileSide::obverse);
	TileTexture->NewTile(num(1), Tile(tile->tile, tile->red[2]), x(1), y(1), vert, TileSide::obverse);
	TileTexture->NewTile(num(2), Tile(tile->tile, tile->red[0]), x(2), y(2), hor, TileSide::obverse);
	TileTexture->NewTile(num(3), Tile(tile->tile, tile->red[1]), x(3), y(3), vert, TileSide::obverse);
}
void ShowNakihai::MinkanShimocha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert(tile->mstat == MeldStat::quadExposedRight);
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, std::ignore, vert, hor) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHHor(1), MPosHVertL(2), MPosHVertL(3), MPosHVertL(4), MPosVHorL(), MPosVVert(), MPosVVert(), MPosVVert(),
		true, false, false, false);
	TileTexture->NewTile(num(0), Tile(tile->tile, tile->red[0]), x(0), y(0), hor, TileSide::obverse);
	TileTexture->NewTile(num(1), Tile(tile->tile, tile->red[3]), x(1), y(1), vert, TileSide::obverse);
	TileTexture->NewTile(num(2), Tile(tile->tile, tile->red[2]), x(2), y(2), vert, TileSide::obverse);
	TileTexture->NewTile(num(3), Tile(tile->tile, tile->red[1]), x(3), y(3), vert, TileSide::obverse);
}
void ShowNakihai::NakihaiSelRoutine(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	switch (gameStat->Player[targetPlayer].Meld[meldID].mstat) {
	case MeldStat::sequenceExposedLower: case MeldStat::sequenceExposedMiddle: case MeldStat::sequenceExposedUpper:
	case MeldStat::tripletExposedLeft: case MeldStat::quadAddedLeft:
		NakihaiKamicha(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	case MeldStat::tripletExposedCenter: case MeldStat::quadAddedCenter:
		NakihaiToimen(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	case MeldStat::tripletExposedRight: case MeldStat::quadAddedRight:
		NakihaiShimocha(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	case MeldStat::quadConcealed:
		NakihaiAnkan(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	case MeldStat::quadExposedLeft:
		MinkanKamicha(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	case MeldStat::quadExposedCenter:
		MinkanToimen(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	case MeldStat::quadExposedRight:
		MinkanShimocha(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	}
}

ShowNakihai::ShowNakihai(DevicePtr device) {
	myDevice = device;
	TileTexture = new ShowTile(device);
}

ShowNakihai::~ShowNakihai() {
	delete TileTexture;
}

}
