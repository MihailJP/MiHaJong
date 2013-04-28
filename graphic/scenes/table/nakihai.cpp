#include "nakihai.h"
#include "../../utils.h"

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

GameTableScreen::NakihaiReconst::NakihaiReconst(GameTableScreen* parent) : ShowNakihai(parent->caller->getDevice()) {
	caller = parent;
}

GameTableScreen::NakihaiReconst::~NakihaiReconst() {
}

}
