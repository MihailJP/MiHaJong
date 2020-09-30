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
	assert(tile->mstat == meldQuadConcealed);
	const TileSide AnkanExpose =
		(((gameStat->gameType & GuobiaoMJ) || rules::chkRule("ankan_conceal", "closed")) && (gameStat->Player[targetPlayer].HandStat != handExposed)) ?
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
	assert((tile->mstat == meldTripletExposedLeft) || (tile->mstat == meldQuadAddedLeft) ||
		(tile->mstat == meldSequenceExposedLower) || (tile->mstat == meldSequenceExposedMiddle) ||
		(tile->mstat == meldSequenceExposedUpper));
	const TileCode tileL = (tile->mstat == meldSequenceExposedMiddle) ? static_cast<TileCode>(tile->tile + 1) : ((tile->mstat == meldSequenceExposedUpper) ? static_cast<TileCode>(tile->tile + 2) : tile->tile);
	const TileCode tileC = (tile->mstat == meldSequenceExposedLower) ? static_cast<TileCode>(tile->tile + 1) : tile->tile;
	const TileCode tileR = (tile->mstat == meldSequenceExposedUpper) ? static_cast<TileCode>(tile->tile + 1) : ((tile->mstat == meldSequenceExposedLower) || (tile->mstat == meldSequenceExposedMiddle) ? static_cast<TileCode>(tile->tile + 2) : tile->tile);
	const doraCol redL = (tile->mstat == meldSequenceExposedMiddle) ? tile->red[1] : ((tile->mstat == meldSequenceExposedUpper) ? tile->red[2] : tile->red[0]);
	const doraCol redC = ((tile->mstat == meldSequenceExposedMiddle) || (tile->mstat == meldSequenceExposedUpper)) ? tile->red[0] : tile->red[1];
	const doraCol redR = (tile->mstat == meldSequenceExposedUpper) ? tile->red[1] : tile->red[2];
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, hor, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHVertR(2), MPosHHor(3), MPosHHor(3), MPosVVert(), MPosVVert(), MPosVHorU(), MPosVHorL(),
		false, false, true, true);
	TileTexture->NewTile(num(0), Tile(tileR, redR), x(0), y(0), vert, TileSide::obverse);
	TileTexture->NewTile(num(1), Tile(tileC, redC), x(1), y(1), vert, TileSide::obverse);
	if (tile->mstat == meldQuadAddedLeft)
		TileTexture->NewTile(num(2), Tile(tileL, tile->red[3]), x(2), y(2), hor, TileSide::obverse);
	else TileTexture->DelTile(num(2));
	TileTexture->NewTile(num(3), Tile(tileL, redL), x(3), y(3), hor, TileSide::obverse);
}
void ShowNakihai::NakihaiToimen(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert((tile->mstat == meldTripletExposedCenter) || (tile->mstat == meldQuadAddedCenter));
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, hor, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHHor(2), MPosHHor(2), MPosHVertL(3), MPosVVert(), MPosVHorU(), MPosVHorL(), MPosVVert(),
		false, true, true, false);
	TileTexture->NewTile(num(0), Tile(tile->tile, tile->red[2]), x(0), y(0), vert, TileSide::obverse);
	if (tile->mstat == meldQuadAddedCenter)
		TileTexture->NewTile(num(1), Tile(tile->tile, tile->red[3]), x(1), y(1), hor, TileSide::obverse);
	else TileTexture->DelTile(num(1));
	TileTexture->NewTile(num(2), Tile(tile->tile, tile->red[0]), x(2), y(2), hor, TileSide::obverse);
	TileTexture->NewTile(num(3), Tile(tile->tile, tile->red[1]), x(3), y(3), vert, TileSide::obverse);
}
void ShowNakihai::NakihaiShimocha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert((tile->mstat == meldTripletExposedRight) || (tile->mstat == meldQuadAddedRight));
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, std::ignore, vert, hor) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHHor(1), MPosHHor(1), MPosHVertL(2), MPosHVertL(3), MPosVHorU(), MPosVHorL(), MPosVVert(), MPosVVert(),
		true, true, false, false);
	if (tile->mstat == meldQuadAddedRight)
		TileTexture->NewTile(num(0), Tile(tile->tile, tile->red[3]), x(0), y(0), hor, TileSide::obverse);
	else TileTexture->DelTile(num(0));
	TileTexture->NewTile(num(1), Tile(tile->tile, tile->red[0]), x(1), y(1), hor, TileSide::obverse);
	TileTexture->NewTile(num(2), Tile(tile->tile, tile->red[2]), x(2), y(2), vert, TileSide::obverse);
	TileTexture->NewTile(num(3), Tile(tile->tile, tile->red[1]), x(3), y(3), vert, TileSide::obverse);
}
void ShowNakihai::MinkanKamicha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert(tile->mstat == meldQuadExposedLeft);
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
	assert(tile->mstat == meldQuadExposedCenter);
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
	assert(tile->mstat == meldQuadExposedRight);
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
	case meldSequenceExposedLower: case meldSequenceExposedMiddle: case meldSequenceExposedUpper:
	case meldTripletExposedLeft: case meldQuadAddedLeft:
		NakihaiKamicha(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	case meldTripletExposedCenter: case meldQuadAddedCenter:
		NakihaiToimen(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	case meldTripletExposedRight: case meldQuadAddedRight:
		NakihaiShimocha(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	case meldQuadConcealed:
		NakihaiAnkan(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	case meldQuadExposedLeft:
		MinkanKamicha(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	case meldQuadExposedCenter:
		MinkanToimen(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	case meldQuadExposedRight:
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
