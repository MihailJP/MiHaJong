#include "table.h"
#include "../scrmanip.h"
#include "../resource.h"
#include "../gametbl.h"
#include "../rule.h"
#include <tuple>
#include <cassert>

namespace mihajong_graphic {

namespace {
	seatRelative inline playerRelative(PLAYER_ID targetPlayer, PLAYER_ID basePlayer) {
		return (seatRelative)((PLAYERS + targetPlayer - basePlayer) % PLAYERS);
	}
}
	
GameTableScreen::GameTableScreen(ScreenManipulator* const manipulator) : TableProtoScene(manipulator) {
	{ /* Debug status */
		GameTable* gstat = GameStatus::gameStat();
		for (PLAYER_ID i = 0; i < PLAYERS; ++i) {
			gstat->Player.val[i].MeldPointer = 4;
			for (PLAYER_ID j = 1; j <= 4; ++j) {
				gstat->Player.val[i].Meld[j].mstat = meldQuadAddedCenter;
				gstat->Player.val[i].Meld[j].tile = (tileCode)(BambooOne + (j - 1));
			}
		}
		GameStatus::updateGameStat(gstat);
	}
	LoadTexture(&tBorder, MAKEINTRESOURCE(IDB_PNG_TBLBORDER), 1080, 1080); InitSprite(&sBorder);
	LoadTexture(&tBaize, MAKEINTRESOURCE(IDB_PNG_TBLBAIZE), 1080, 1080); InitSprite(&sBaize);
	yamahaiReconst = new YamahaiReconst(this);
	Reconstruct(GameStatus::gameStat());
}

GameTableScreen::~GameTableScreen() {
	delete yamahaiReconst;
	if (tBorder) tBorder->Release();
	if (sBorder) sBorder->Release();
	if (tBaize) tBaize->Release();
	if (sBaize) sBaize->Release();
}

/* éRîvÇÃï\é¶ */
void GameTableScreen::ReconstructYamahai(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer) {
	std::tuple<unsigned, unsigned, unsigned, unsigned> yamahaiAttr;
	if (gameStat->gameType & AllSanma)
		yamahaiAttr = std::make_tuple(3, 108, 18, 102 - gameStat->ExtraRinshan);
	else if (rules::chkRule("flower_tiles", "no"))
		yamahaiAttr = std::make_tuple(0, 136, 17, 130);
	else if (rules::chkRule("flower_tiles", "8tiles"))
		yamahaiAttr = std::make_tuple(1, 144, 18, 130);
	else
		yamahaiAttr = std::make_tuple(2, 140, 18, 130);
	seatRelative tmpPlayerCode = playerRelative((gameStat->gameType & Sanma4) ? trueTargetPlayer : targetPlayer, gameStat->PlayerID);
	auto calcTileNum = [&gameStat, &targetPlayer, &yamahaiAttr](unsigned j) -> unsigned {
		const unsigned yamahaiMode = std::get<0>(yamahaiAttr);
		const unsigned dice = gameStat->Dice[0].Number + gameStat->Dice[1].Number;
		if ((gameStat->Dice[0].Number == 0) && (gameStat->Dice[1].Number == 0)) return 0; // ñ¢ê›íË
		assert((dice >= 2) && (dice <= 12));
		switch (yamahaiMode) {
		case 0:
			return
				((((targetPlayer + 3) % PLAYERS) * 34) + j * 2 + (104 * dice + 34) +
				((gameStat->gameType % PLAYERS)                       * 102)) % 136;
		case 1:
			return
				((((targetPlayer + 3) % PLAYERS) * 36) + j * 2 + (110 * dice + 36) +
				((gameStat->gameType % PLAYERS)                       * 108)) % 144;
		case 2:
			return (
				((((targetPlayer + 3) % PLAYERS) * 35) + j * 2 + (107 * dice + 35) +
				((gameStat->gameType % PLAYERS)                       * 105)) % 140
				) / 2 * 2;
		case 3:
			return
				((((targetPlayer + 2) % 3      ) * 36) + j * 2 + (74 * dice + 36) +
				(((gameStat->gameType - (gameStat->gameType / 4)) % 3)*  72)) % 108;
		default:
			throw _T("calcTileNum: Invalid mode!");
		}
	};
	auto getRinshanFlag = [&gameStat, &yamahaiAttr](unsigned ActiveTileNum) -> bool {
		const unsigned NumberOfTiles = std::get<1>(yamahaiAttr);
		bool RinshanFlag = false;
		for (unsigned i = 0; i <= 10; i += 2)
			if ((ActiveTileNum == i) && (gameStat->RinshanPointer < NumberOfTiles - i - 2)) RinshanFlag = true;
		return RinshanFlag;
	};
	switch (tmpPlayerCode) {
	case sOpposite:
		for (int i = (18 - std::get<2>(yamahaiAttr)) * 2; i < 36; i += 2) { /* ëŒñ ë§ÇÃéR */
			unsigned tileNum = calcTileNum((std::get<2>(yamahaiAttr) * 2 - i) / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i); TileTexture->DelTile(i + 1);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile(      i    , gameStat->Deck[k + 1].tile, Normal, DeckPosH + ShowTile::VertTileWidth * (i / 2 - 1),
				DeckPosV,                 UpsideDown, Reverse);
			if ((gameStat->TilePointer <= k)     && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile(      i + 1, gameStat->Deck[k    ].tile, Normal, DeckPosH + ShowTile::VertTileWidth * (i / 2 - 1),
				DeckPosV - TileThickness, UpsideDown, dora ? Obverse : Reverse);
		}
		break;
	case sLeft:
		for (int i = 0; i < std::get<2>(yamahaiAttr) * 2; i += 2) { /* è„â∆ë§ÇÃéR */
			unsigned tileNum = calcTileNum(i / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i + 36); TileTexture->DelTile(i + 37);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile( 36 + i    , gameStat->Deck[k + 1].tile, Normal, DeckPosV,
				DeckPosH                 + ShowTile::VertTileWidth * (i / 2), Clockwise, Reverse);
			if ((gameStat->TilePointer <= k)     && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile( 36 + i + 1, gameStat->Deck[k    ].tile, Normal, DeckPosV,
				DeckPosH - TileThickness + ShowTile::VertTileWidth * (i / 2), Clockwise, dora ? Obverse : Reverse);
		}
		break;
	case sRight:
		for (int i = (18 - std::get<2>(yamahaiAttr)) * 2; i < 36; i += 2) { /* â∫â∆ë§ÇÃéR */
			unsigned tileNum = calcTileNum((std::get<2>(yamahaiAttr) * 2 - i) / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i + 72); TileTexture->DelTile(i + 73);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile( 72 + i    , gameStat->Deck[k + 1].tile, Normal, TableSize - DeckPosV,
				DeckPosH                 + ShowTile::VertTileWidth * (i / 2 - 1), Withershins, Reverse);
			if ((gameStat->TilePointer <= k) && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile( 72 + i + 1, gameStat->Deck[k    ].tile, Normal, TableSize - DeckPosV,
				DeckPosH - TileThickness + ShowTile::VertTileWidth * (i / 2 - 1), Withershins, dora ? Obverse : Reverse);
		}
		break;
	case sSelf:
		for (int i = 0; i < std::get<2>(yamahaiAttr) * 2; i += 2) { /* é©ï™ÇÃéR */
			unsigned tileNum = calcTileNum(i / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k  >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i + 108); TileTexture->DelTile(i + 109);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile(108 + i    , gameStat->Deck[k + 1].tile, Normal, DeckPosH + ShowTile::VertTileWidth * (i / 2),
				TableSize - DeckPosV,                 Portrait, Reverse);
			if ((gameStat->TilePointer <= k)     && (!getRinshanFlag(tileNum)))
				TileTexture->NewTile(108 + i + 1, gameStat->Deck[k    ].tile, Normal, DeckPosH + ShowTile::VertTileWidth * (i / 2),
				TableSize - DeckPosV - TileThickness, Portrait, dora ? Obverse : Reverse);
		}
		break;
	}
	return;
}

/* éËîvÇï\é¶Ç∑ÇÈ */
void GameTableScreen::ReconstructTehai(const GameTable* gameStat, PLAYER_ID targetPlayer) {
	int tilePos;
	/* éËîv */
	switch (playerRelative(targetPlayer, gameStat->PlayerID)) {
	case sOpposite: /* ëŒñ ÇÃéËîv */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i)
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile)
				++tilePos;
		for (int i = 0; i <= HandLength; ++i)
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile)
				TileTexture->NewTile(144+i,
				gameStat->Player.val[targetPlayer].Hand[i].tile,
				gameStat->Player.val[targetPlayer].Hand[i].red,
				HandPosH + ShowTile::VertTileWidth * (--tilePos) - ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				HandPosV, UpsideDown, Obverse);
		break;
	case sLeft: /* è„â∆ÇÃéËîv */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i)
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile)
				TileTexture->NewTile(144+14+i,
				gameStat->Player.val[targetPlayer].Hand[i].tile,
				gameStat->Player.val[targetPlayer].Hand[i].red,
				HandPosV,
				HandPosH + ShowTile::VertTileWidth * (tilePos++) + ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				Clockwise, Obverse);
		break;
	case sRight: /* â∫â∆ÇÃéËîv */
		tilePos = 0;
		for (int i = HandLength; i >= 0; --i)
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile)
				TileTexture->NewTile(144+28+(13-i),
				gameStat->Player.val[targetPlayer].Hand[i].tile,
				gameStat->Player.val[targetPlayer].Hand[i].red,
				TableSize - HandPosV,
				HandPosH + ShowTile::VertTileWidth * (tilePos++) - ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				Withershins, Obverse);
		break;
	case sSelf: /* é©ï™ÇÃéËîv */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i)
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile)
				TileTexture->NewTile(144+42+i,
				gameStat->Player.val[targetPlayer].Hand[i].tile,
				gameStat->Player.val[targetPlayer].Hand[i].red,
				HandPosH + ShowTile::VertTileWidth * (tilePos++) + ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				TableSize - HandPosV, Portrait, Obverse);
		break;
	}
}

void GameTableScreen::ReconstructPlayer(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer) {
	ReconstructYamahai(gameStat, targetPlayer, trueTargetPlayer);
	ReconstructTehai(gameStat, targetPlayer);
	yamahaiReconst->ReconstructNakihai(gameStat, targetPlayer);
}

void GameTableScreen::Reconstruct(const GameTable* gameStat) {
	if (gameStat->gameType & Yonma) {
		for (PLAYER_ID i = 0; i < 4; i++)
			ReconstructPlayer(gameStat, i, i);
	} else if (gameStat->gameType & Sanma4) {
		PLAYER_ID tobePlayed[4][4] = {
			{0, 1, 2, 3}, {3, 1, 2, 0}, {1, 3, 2, 0}, {1, 2, 3, 0},
		};
		for (PLAYER_ID i = 0; i < 4; i++) {
			PLAYER_ID j(tobePlayed[gameStat->GameRound % PLAYERS][i]);
			if (j < 3) ReconstructPlayer(gameStat, j, i);
		}
	} else {
		for (PLAYER_ID i = 0; i < 3; i++)
			ReconstructPlayer(gameStat, i, i);
	}
}

void GameTableScreen::Render() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0); // ÉoÉbÉtÉ@ÉNÉäÉA
	ShowSidebar();
	ShowSprite(sBaize, tBaize, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	ShowSprite(sBorder, tBorder, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	if (GameStatus::isModified())
		Reconstruct(GameStatus::gameStat());
	TileTexture->Render();
}

// -------------------------------------------------------------------------

/* ñ¬Ç¢ÇΩîvÇï\é¶Ç∑ÇÈ */
std::tuple<std::function<unsigned (unsigned)>, std::function<int (unsigned)>, std::function<int (unsigned)>, TileDirection, TileDirection, TileDirection>
	GameTableScreen::YamahaiReconst::playerPosition(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID,
	unsigned h1, unsigned h2, unsigned h3, unsigned h4, unsigned v1, unsigned v2, unsigned v3, unsigned v4,
	bool r1, bool r2, bool r3, bool r4)
{
	switch (playerRelative(targetPlayer, gameStat->PlayerID)) {
	case sOpposite:
		return std::make_tuple(
			[=](unsigned i) -> unsigned {return 215 - i - IDOffset;},
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
			[=](unsigned i) -> unsigned {return 231 - i - IDOffset;},
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
			[=](unsigned i) -> unsigned {return 232 + i + IDOffset;},
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
			[=](unsigned i) -> unsigned {return 248 + i + IDOffset;},
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
	}
}
void GameTableScreen::YamahaiReconst::NakihaiAnkan(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const meldCode* const tile = &(gameStat->Player.val[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player.val[targetPlayer].MeldPointer));
	assert(tile->mstat == meldQuadConcealed);
	const TileSide AnkanExpose =
		(rules::chkRule("ankan_conceal", "closed") && (gameStat->Player.val[targetPlayer].HandStat != handExposed)) ?
		Reverse : Obverse;
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert;
	std::tie(num, x, y, std::ignore, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHVertR(2), MPosHVertR(3), MPosHVertR(4), MPosVVert, MPosVVert, MPosVVert, MPosVVert,
		false, false, false, false);
	caller->TileTexture->NewTile(num(0), tile->tile, tile->red[2], x(0), y(0), vert, Reverse);
	caller->TileTexture->NewTile(num(1), tile->tile, tile->red[0], x(1), y(1), vert, AnkanExpose);
	caller->TileTexture->NewTile(num(2), tile->tile, tile->red[1], x(2), y(2), vert, AnkanExpose);
	caller->TileTexture->NewTile(num(3), tile->tile, tile->red[3], x(3), y(3), vert, Reverse);
}
void GameTableScreen::YamahaiReconst::NakihaiKamicha(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const meldCode* const tile = &(gameStat->Player.val[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player.val[targetPlayer].MeldPointer));
	assert((tile->mstat == meldTripletExposedLeft) || (tile->mstat == meldQuadAddedLeft) ||
		(tile->mstat == meldSequenceExposedLower) || (tile->mstat == meldSequenceExposedMiddle) ||
		(tile->mstat == meldSequenceExposedUpper));
	const tileCode tileL = (tile->mstat == meldSequenceExposedMiddle) ? (tileCode)(tile->tile + 1) : ((tile->mstat == meldSequenceExposedUpper) ? (tileCode)(tile->tile + 2) : tile->tile);
	const tileCode tileC = (tile->mstat == meldSequenceExposedLower) ? (tileCode)(tile->tile + 1) : tile->tile;
	const tileCode tileR = (tile->mstat == meldSequenceExposedUpper) ? (tileCode)(tile->tile + 1) : ((tile->mstat == meldSequenceExposedLower) || (tile->mstat == meldSequenceExposedMiddle) ? (tileCode)(tile->tile + 2) : tile->tile);
	const doraCol redL = (tile->mstat == meldSequenceExposedMiddle) ? tile->red[1] : ((tile->mstat == meldSequenceExposedUpper) ? tile->red[2] : tile->red[0]);
	const doraCol redC = ((tile->mstat == meldSequenceExposedMiddle) || (tile->mstat == meldSequenceExposedUpper)) ? tile->red[0] : tile->red[1];
	const doraCol redR = (tile->mstat == meldSequenceExposedUpper) ? tile->red[1] : tile->red[2];
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, hor, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHVertR(2), MPosHHor(3), MPosHHor(3), MPosVVert, MPosVVert, MPosVHorU, MPosVHorL,
		false, false, true, true);
	caller->TileTexture->NewTile(num(0), tileR, redR, x(0), y(0), vert, Obverse);
	caller->TileTexture->NewTile(num(1), tileC, redC, x(1), y(1), vert, Obverse);
	if (tile->mstat == meldQuadAddedLeft)
		caller->TileTexture->NewTile(num(2), tileL, tile->red[3], x(2), y(2), hor, Obverse);
	caller->TileTexture->NewTile(num(3), tileL, redL, x(3), y(3), hor, Obverse);
}
void GameTableScreen::YamahaiReconst::NakihaiToimen(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const meldCode* const tile = &(gameStat->Player.val[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player.val[targetPlayer].MeldPointer));
	assert((tile->mstat == meldTripletExposedCenter) || (tile->mstat == meldQuadAddedCenter));
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, hor, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHHor(2), MPosHHor(2), MPosHVertL(3), MPosVVert, MPosVHorU, MPosVHorL, MPosVVert,
		false, true, true, false);
	caller->TileTexture->NewTile(num(0), tile->tile, tile->red[2], x(0), y(0), vert, Obverse);
	if (tile->mstat == meldQuadAddedCenter)
		caller->TileTexture->NewTile(num(1), tile->tile, tile->red[3], x(1), y(1), hor, Obverse);
	caller->TileTexture->NewTile(num(2), tile->tile, tile->red[0], x(2), y(2), hor, Obverse);
	caller->TileTexture->NewTile(num(3), tile->tile, tile->red[1], x(3), y(3), vert, Obverse);
}
void GameTableScreen::YamahaiReconst::NakihaiShimocha(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const meldCode* const tile = &(gameStat->Player.val[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player.val[targetPlayer].MeldPointer));
	assert((tile->mstat == meldTripletExposedRight) || (tile->mstat == meldQuadAddedRight));
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, std::ignore, vert, hor) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHHor(1), MPosHHor(1), MPosHVertL(2), MPosHVertL(3), MPosVHorU, MPosVHorL, MPosVVert, MPosVVert,
		true, true, false, false);
	if (tile->mstat == meldQuadAddedRight)
		caller->TileTexture->NewTile(num(0), tile->tile, tile->red[3], x(0), y(0), hor, Obverse);
	caller->TileTexture->NewTile(num(1), tile->tile, tile->red[0], x(1), y(1), hor, Obverse);
	caller->TileTexture->NewTile(num(2), tile->tile, tile->red[2], x(2), y(2), vert, Obverse);
	caller->TileTexture->NewTile(num(3), tile->tile, tile->red[1], x(3), y(3), vert, Obverse);
}
void GameTableScreen::YamahaiReconst::MinkanKamicha(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const meldCode* const tile = &(gameStat->Player.val[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player.val[targetPlayer].MeldPointer));
	assert(tile->mstat == meldQuadExposedLeft);
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, hor, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHVertR(2), MPosHVertR(3), MPosHHor(4), MPosVVert, MPosVVert, MPosVVert, MPosVHorL,
		false, false, false, true);
	caller->TileTexture->NewTile(num(0), tile->tile, tile->red[3], x(0), y(0), vert, Obverse);
	caller->TileTexture->NewTile(num(1), tile->tile, tile->red[2], x(1), y(1), vert, Obverse);
	caller->TileTexture->NewTile(num(2), tile->tile, tile->red[1], x(2), y(2), vert, Obverse);
	caller->TileTexture->NewTile(num(3), tile->tile, tile->red[0], x(3), y(3), hor, Obverse);
}
void GameTableScreen::YamahaiReconst::MinkanToimen(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const meldCode* const tile = &(gameStat->Player.val[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player.val[targetPlayer].MeldPointer));
	assert(tile->mstat == meldQuadExposedCenter);
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, hor, vert, std::ignore) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHVertR(1), MPosHVertR(2), MPosHHor(3), MPosHVertL(4), MPosVVert, MPosVVert, MPosVHorL, MPosVVert,
		false, false, true, false);
	caller->TileTexture->NewTile(num(0), tile->tile, tile->red[3], x(0), y(0), vert, Obverse);
	caller->TileTexture->NewTile(num(1), tile->tile, tile->red[2], x(1), y(1), vert, Obverse);
	caller->TileTexture->NewTile(num(2), tile->tile, tile->red[0], x(2), y(2), hor, Obverse);
	caller->TileTexture->NewTile(num(3), tile->tile, tile->red[1], x(3), y(3), vert, Obverse);
}
void GameTableScreen::YamahaiReconst::MinkanShimocha(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	const meldCode* const tile = &(gameStat->Player.val[targetPlayer].Meld[meldID]);
	assert((meldID > 0) && (meldID <= gameStat->Player.val[targetPlayer].MeldPointer));
	assert(tile->mstat == meldQuadExposedRight);
	std::function<unsigned (unsigned)> num; std::function<int (unsigned)>x, y; TileDirection vert, hor;
	std::tie(num, x, y, std::ignore, vert, hor) = playerPosition(gameStat, targetPlayer, PositionOffset, IDOffset, meldID,
		MPosHHor(1), MPosHVertL(2), MPosHVertL(3), MPosHVertL(4), MPosVHorL, MPosVVert, MPosVVert, MPosVVert,
		true, false, false, false);
	caller->TileTexture->NewTile(num(0), tile->tile, tile->red[0], x(0), y(0), hor, Obverse);
	caller->TileTexture->NewTile(num(1), tile->tile, tile->red[3], x(1), y(1), vert, Obverse);
	caller->TileTexture->NewTile(num(2), tile->tile, tile->red[2], x(2), y(2), vert, Obverse);
	caller->TileTexture->NewTile(num(3), tile->tile, tile->red[1], x(3), y(3), vert, Obverse);
}
void GameTableScreen::YamahaiReconst::NakihaiSelRoutine(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
	switch (gameStat->Player.val[targetPlayer].Meld[meldID].mstat) {
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
		NakihaiKamicha(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	case meldQuadExposedCenter:
		NakihaiToimen(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	case meldQuadExposedRight:
		NakihaiShimocha(gameStat, targetPlayer, PositionOffset, IDOffset, meldID);
		break;
	}
}
void GameTableScreen::YamahaiReconst::ReconstructNakihai(const GameTable* gameStat, PLAYER_ID targetPlayer) {
	unsigned posOffset[5] = {0,};
	for (int i = 1; i <= gameStat->Player.val[targetPlayer].MeldPointer; ++i) {
		switch (gameStat->Player.val[targetPlayer].Meld[i].mstat) {
		case meldQuadAddedLeft: case meldQuadAddedCenter: case meldQuadAddedRight:
			posOffset[i] = posOffset[i - 1] + ShowTile::VertTileWidth * 2;
			break;
		default:
			posOffset[i] = posOffset[i - 1] + ShowTile::HoriTileHeight;
			break;
		}
	}
	for (int i = 1; i <= gameStat->Player.val[targetPlayer].MeldPointer; ++i)
		NakihaiSelRoutine(gameStat, targetPlayer, posOffset[i - 1],
			(gameStat->Player.val[targetPlayer].MeldPointer - i) * 4, i);
}

}
