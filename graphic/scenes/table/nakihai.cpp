#include "nakihai.h"
#include "../../gametbl.h"
#include "../../rule.h"
#include "../../utils.h"
#include <cassert>

namespace mihajong_graphic {

using utils::playerRelative;

/* –Â‚¢‚½”v‚ð•\Ž¦‚·‚é */
std::tuple<std::function<unsigned (unsigned)>, std::function<int (unsigned)>, std::function<int (unsigned)>, TileDirection, TileDirection, TileDirection>
	GameTableScreen::NakihaiReconst::playerPosition(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID,
	unsigned h1, unsigned h2, unsigned h3, unsigned h4, unsigned v1, unsigned v2, unsigned v3, unsigned v4,
	bool r1, bool r2, bool r3, bool r4)
{
	switch (playerRelative(targetPlayer, gameStat->PlayerID)) {
	case sOpposite:
		return std::make_tuple(
			[=](unsigned i) -> unsigned {return 15 - i - IDOffset;},
			[=](unsigned i) -> int {
				switch (i) {
					case 0: return TableSize - h1; break;
					case 1: return TableSize - h2; break;
					case 2: return TableSize - h3; break;
					case 3: return TableSize - h4; break;
					default: throw "Out of range";
				}
			},
			[=](unsigned i) -> int {
				switch (i) {
					case 0: return TableSize - v1 + PositionOffset - (r1 ? 5 : 4); break;
					case 1: return TableSize - v2 + PositionOffset - (r2 ? 5 : 4); break;
					case 2: return TableSize - v3 + PositionOffset - (r3 ? 5 : 4); break;
					case 3: return TableSize - v4 + PositionOffset - (r4 ? 5 : 4); break;
					default: throw "Out of range";
				}
			},
			Clockwise, UpsideDown, Withershins);
	case sLeft:
		return std::make_tuple(
			[=](unsigned i) -> unsigned {return 31 - i - IDOffset;},
			[=](unsigned i) -> int {
				switch (i) {
					case 0: return TableSize - v1 + PositionOffset - (r1 ? 5 : 4); break;
					case 1: return TableSize - v2 + PositionOffset - (r2 ? 5 : 4); break;
					case 2: return TableSize - v3 + PositionOffset - (r3 ? 5 : 4); break;
					case 3: return TableSize - v4 + PositionOffset - (r4 ? 5 : 4); break;
					default: throw "Out of range";
				}
			},
			[=](unsigned i) -> int {
				switch (i) {
					case 0: return h1 - 5; break;
					case 1: return h2 - 5; break;
					case 2: return h3 - 5; break;
					case 3: return h4 - 5; break;
					default: throw "Out of range";
				}
			},
			Portrait, Clockwise, UpsideDown);
		break;
	case sRight:
		return std::make_tuple(
			[=](unsigned i) -> unsigned {return 32 + i + IDOffset;},
			[=](unsigned i) -> int {
				switch (i) {
					case 0: return v1 - PositionOffset + (r1 ? 5 : 4); break;
					case 1: return v2 - PositionOffset + (r2 ? 5 : 4); break;
					case 2: return v3 - PositionOffset + (r3 ? 5 : 4); break;
					case 3: return v4 - PositionOffset + (r4 ? 5 : 4); break;
					default: throw "Out of range";
				}
			},
			[=](unsigned i) -> int {
				switch (i) {
					case 0: return TableSize - h1; break;
					case 1: return TableSize - h2; break;
					case 2: return TableSize - h3; break;
					case 3: return TableSize - h4; break;
					default: throw "Out of range";
				}
			},
			UpsideDown, Withershins, Portrait);
		break;
	case sSelf:
		return std::make_tuple(
			[=](unsigned i) -> unsigned {return 48 + i + IDOffset;},
			[=](unsigned i) -> int {
				switch (i) {
					case 0: return h1; break;
					case 1: return h2; break;
					case 2: return h3; break;
					case 3: return h4; break;
					default: throw "Out of range";
				}
			},
			[=](unsigned i) -> int {
				switch (i) {
					case 0: return v1 - PositionOffset; break;
					case 1: return v2 - PositionOffset; break;
					case 2: return v3 - PositionOffset; break;
					case 3: return v4 - PositionOffset; break;
					default: throw "Out of range";
				}
			},
			Withershins, Portrait, Clockwise);
		break;
	default:
		throw "Argument error";
	}
}
void GameTableScreen::NakihaiReconst::NakihaiAnkan(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert(tile->mstat == meldQuadConcealed);
	const TileSide AnkanExpose =
		(rules::chkRule("ankan_conceal", "closed") && (gameStat->Player[targetPlayer].HandStat != handExposed)) ?
		Reverse : Obverse;
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert;
	std::tie(num, x, y, std::ignore, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHVertR(2), MPosHVertR(3), MPosHVertR(4), MPosVVert, MPosVVert, MPosVVert, MPosVVert,
		false, false, false, false);
	TileTexture->NewTile(num(0), tile->tile, tile->red[2], x(0), y(0), vert, Reverse);
	TileTexture->NewTile(num(1), tile->tile, tile->red[0], x(1), y(1), vert, AnkanExpose);
	TileTexture->NewTile(num(2), tile->tile, tile->red[1], x(2), y(2), vert, AnkanExpose);
	TileTexture->NewTile(num(3), tile->tile, tile->red[3], x(3), y(3), vert, Reverse);
}
void GameTableScreen::NakihaiReconst::NakihaiKamicha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert((tile->mstat == meldTripletExposedLeft) || (tile->mstat == meldQuadAddedLeft) ||
		(tile->mstat == meldSequenceExposedLower) || (tile->mstat == meldSequenceExposedMiddle) ||
		(tile->mstat == meldSequenceExposedUpper));
	const TileCode tileL = (tile->mstat == meldSequenceExposedMiddle) ? (TileCode)(tile->tile + 1) : ((tile->mstat == meldSequenceExposedUpper) ? (TileCode)(tile->tile + 2) : tile->tile);
	const TileCode tileC = (tile->mstat == meldSequenceExposedLower) ? (TileCode)(tile->tile + 1) : tile->tile;
	const TileCode tileR = (tile->mstat == meldSequenceExposedUpper) ? (TileCode)(tile->tile + 1) : ((tile->mstat == meldSequenceExposedLower) || (tile->mstat == meldSequenceExposedMiddle) ? (TileCode)(tile->tile + 2) : tile->tile);
	const doraCol redL = (tile->mstat == meldSequenceExposedMiddle) ? tile->red[1] : ((tile->mstat == meldSequenceExposedUpper) ? tile->red[2] : tile->red[0]);
	const doraCol redC = ((tile->mstat == meldSequenceExposedMiddle) || (tile->mstat == meldSequenceExposedUpper)) ? tile->red[0] : tile->red[1];
	const doraCol redR = (tile->mstat == meldSequenceExposedUpper) ? tile->red[1] : tile->red[2];
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, hor, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHVertR(2), MPosHHor(3), MPosHHor(3), MPosVVert, MPosVVert, MPosVHorU, MPosVHorL,
		false, false, true, true);
	TileTexture->NewTile(num(0), tileR, redR, x(0), y(0), vert, Obverse);
	TileTexture->NewTile(num(1), tileC, redC, x(1), y(1), vert, Obverse);
	if (tile->mstat == meldQuadAddedLeft)
		TileTexture->NewTile(num(2), tileL, tile->red[3], x(2), y(2), hor, Obverse);
	else TileTexture->DelTile(num(2));
	TileTexture->NewTile(num(3), tileL, redL, x(3), y(3), hor, Obverse);
}
void GameTableScreen::NakihaiReconst::NakihaiToimen(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert((tile->mstat == meldTripletExposedCenter) || (tile->mstat == meldQuadAddedCenter));
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, hor, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHHor(2), MPosHHor(2), MPosHVertL(3), MPosVVert, MPosVHorU, MPosVHorL, MPosVVert,
		false, true, true, false);
	TileTexture->NewTile(num(0), tile->tile, tile->red[2], x(0), y(0), vert, Obverse);
	if (tile->mstat == meldQuadAddedCenter)
		TileTexture->NewTile(num(1), tile->tile, tile->red[3], x(1), y(1), hor, Obverse);
	else TileTexture->DelTile(num(1));
	TileTexture->NewTile(num(2), tile->tile, tile->red[0], x(2), y(2), hor, Obverse);
	TileTexture->NewTile(num(3), tile->tile, tile->red[1], x(3), y(3), vert, Obverse);
}
void GameTableScreen::NakihaiReconst::NakihaiShimocha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert((tile->mstat == meldTripletExposedRight) || (tile->mstat == meldQuadAddedRight));
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, std::ignore, vert, hor) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHHor(1), MPosHHor(1), MPosHVertL(2), MPosHVertL(3), MPosVHorU, MPosVHorL, MPosVVert, MPosVVert,
		true, true, false, false);
	if (tile->mstat == meldQuadAddedRight)
		TileTexture->NewTile(num(0), tile->tile, tile->red[3], x(0), y(0), hor, Obverse);
	else TileTexture->DelTile(num(0));
	TileTexture->NewTile(num(1), tile->tile, tile->red[0], x(1), y(1), hor, Obverse);
	TileTexture->NewTile(num(2), tile->tile, tile->red[2], x(2), y(2), vert, Obverse);
	TileTexture->NewTile(num(3), tile->tile, tile->red[1], x(3), y(3), vert, Obverse);
}
void GameTableScreen::NakihaiReconst::MinkanKamicha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert(tile->mstat == meldQuadExposedLeft);
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, hor, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHVertR(2), MPosHVertR(3), MPosHHor(4), MPosVVert, MPosVVert, MPosVVert, MPosVHorL,
		false, false, false, true);
	TileTexture->NewTile(num(0), tile->tile, tile->red[3], x(0), y(0), vert, Obverse);
	TileTexture->NewTile(num(1), tile->tile, tile->red[2], x(1), y(1), vert, Obverse);
	TileTexture->NewTile(num(2), tile->tile, tile->red[1], x(2), y(2), vert, Obverse);
	TileTexture->NewTile(num(3), tile->tile, tile->red[0], x(3), y(3), hor, Obverse);
}
void GameTableScreen::NakihaiReconst::MinkanToimen(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert(tile->mstat == meldQuadExposedCenter);
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, hor, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHVertR(2), MPosHHor(3), MPosHVertL(4), MPosVVert, MPosVVert, MPosVHorL, MPosVVert,
		false, false, true, false);
	TileTexture->NewTile(num(0), tile->tile, tile->red[3], x(0), y(0), vert, Obverse);
	TileTexture->NewTile(num(1), tile->tile, tile->red[2], x(1), y(1), vert, Obverse);
	TileTexture->NewTile(num(2), tile->tile, tile->red[0], x(2), y(2), hor, Obverse);
	TileTexture->NewTile(num(3), tile->tile, tile->red[1], x(3), y(3), vert, Obverse);
}
void GameTableScreen::NakihaiReconst::MinkanShimocha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const MeldCode* const tile = &(gameStat->Player[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player[targetPlayer].MeldPointer));
	assert(tile->mstat == meldQuadExposedRight);
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, std::ignore, vert, hor) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHHor(1), MPosHVertL(2), MPosHVertL(3), MPosHVertL(4), MPosVHorL, MPosVVert, MPosVVert, MPosVVert,
		true, false, false, false);
	TileTexture->NewTile(num(0), tile->tile, tile->red[0], x(0), y(0), hor, Obverse);
	TileTexture->NewTile(num(1), tile->tile, tile->red[3], x(1), y(1), vert, Obverse);
	TileTexture->NewTile(num(2), tile->tile, tile->red[2], x(2), y(2), vert, Obverse);
	TileTexture->NewTile(num(3), tile->tile, tile->red[1], x(3), y(3), vert, Obverse);
}
void GameTableScreen::NakihaiReconst::NakihaiSelRoutine(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
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
void GameTableScreen::NakihaiReconst::Reconstruct(const GameTable* gameStat, PlayerID targetPlayer) {
	unsigned posOffset[5] = {0,};
	for (int i = 1; i <= gameStat->Player[targetPlayer].MeldPointer; ++i) {
		switch (gameStat->Player[targetPlayer].Meld[i].mstat) {
		case meldQuadAddedLeft: case meldQuadAddedCenter: case meldQuadAddedRight:
			posOffset[i] = posOffset[i - 1] + ShowTile::VertTileWidth * 2;
			break;
		default:
			posOffset[i] = posOffset[i - 1] + ShowTile::HoriTileWidth;
			break;
		}
	}
	for (int i = 1; i <= gameStat->Player[targetPlayer].MeldPointer; ++i)
		NakihaiSelRoutine(gameStat, targetPlayer, posOffset[i - 1],
			(gameStat->Player[targetPlayer].MeldPointer - i) * 4, i);
}

void GameTableScreen::NakihaiReconst::Render() {
	TileTexture->Render();
}

GameTableScreen::NakihaiReconst::NakihaiReconst(GameTableScreen* parent) {
	caller = parent;
	TileTexture = new ShowTile(parent->caller->getDevice());
}

GameTableScreen::NakihaiReconst::~NakihaiReconst() {
	delete TileTexture;
}

}
