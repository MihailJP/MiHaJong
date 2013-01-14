#include "table.h"
#include "../scrmanip.h"
#include "../resource.h"
#include "../gametbl.h"
#include "../rule.h"
#include "../sprite.h"
#include "../utils.h"
#include "../geometry.h"
#include <tuple>
#include <cassert>
#include <cmath>
#include "scene_id.h"

namespace mihajong_graphic {

using utils::playerRelative;
	
GameTableScreen::GameTableScreen(ScreenManipulator* const manipulator) : TableProtoScene(manipulator) {
	LoadTexture(&tBorder, MAKEINTRESOURCE(IDB_PNG_TBLBORDER), 1080, 1080);
	LoadTexture(&tBaize, MAKEINTRESOURCE(IDB_PNG_TBLBAIZE), 1080, 1080);
	LoadTexture(&tChiicha, MAKEINTRESOURCE(IDB_PNG_CHICHAMARK), 419, 174);
	LoadTexture(&tRichi, MAKEINTRESOURCE(IDB_PNG_TENBOU), 218, 148);
	LoadTexture(&tDice, MAKEINTRESOURCE(IDB_PNG_DICE), 156, 144);
	nakihaiReconst = new NakihaiReconst(this);
	Reconstruct(GameStatus::retrGameStat());
	const unsigned logWidth = (unsigned)std::floor(0.5f + // VC++2010ではround()が使えない
		(float)(((signed)Geometry::WindowWidth - (signed)Geometry::WindowHeight) / Geometry::WindowScale() - 36)) / 9u;
	logWindow = new logwnd::LogWindow(caller->getHWnd(), caller->getDevice(),
		1100, 100, logWidth, 20);
	InitializeCriticalSection(&subSceneCS);
	mySubScene = new TableSubsceneNormal(manipulator->getDevice());
}

GameTableScreen::~GameTableScreen() {
	delete mySubScene; DeleteCriticalSection(&subSceneCS);
	delete logWindow;
	delete nakihaiReconst;
	if (tDice) tDice->Release();
	if (tRichi) tRichi->Release();
	if (tChiicha) tChiicha->Release();
	if (tBorder) tBorder->Release();
	if (tBaize) tBaize->Release();
}

/* 山牌の表示 */
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
		if ((gameStat->Dice[0].Number == 0) && (gameStat->Dice[1].Number == 0)) return 0; // 未設定
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
	auto getRinshanFlag = [gameStat, yamahaiAttr](unsigned int a) -> std::function<bool (unsigned)> {
		const GameTable* const gameStat_ = gameStat;
		const std::tuple<unsigned, unsigned, unsigned, unsigned> yamahaiAttr_ = yamahaiAttr;
		return
		[gameStat_, yamahaiAttr_, a](unsigned ActiveTileNum) -> bool {
			const unsigned NumberOfTiles = std::get<1>(yamahaiAttr_);
			bool RinshanFlag = false;
			for (unsigned i = 0; i <= 10; i += 2)
				if ((ActiveTileNum == i) && (gameStat_->RinshanPointer < NumberOfTiles - i - a)) RinshanFlag = true;
			return RinshanFlag;
		};
	};
	auto getRinshanFlag2 = getRinshanFlag(2);
	auto getRinshanFlag1 = getRinshanFlag(1);
	switch (tmpPlayerCode) {
	case sOpposite:
		for (int i = (18 - std::get<2>(yamahaiAttr)) * 2; i < 36; i += 2) { /* 対面側の山 */
			unsigned tileNum = calcTileNum((std::get<2>(yamahaiAttr) * 2 - i) / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i); TileTexture->DelTile(i + 1);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag2(tileNum)))
				TileTexture->NewTile(      i    , gameStat->Deck[k + 1].tile, Normal, DeckPosH + ShowTile::VertTileWidth * (i / 2 - 1),
				DeckPosV,                 UpsideDown, Reverse);
			if ((gameStat->TilePointer <= k)     && (!getRinshanFlag1(tileNum)))
				TileTexture->NewTile(      i + 1, gameStat->Deck[k    ].tile, Normal, DeckPosH + ShowTile::VertTileWidth * (i / 2 - 1),
				DeckPosV - TileThickness, UpsideDown, dora ? Obverse : Reverse);
		}
		break;
	case sLeft:
		for (int i = 0; i < std::get<2>(yamahaiAttr) * 2; i += 2) { /* 上家側の山 */
			unsigned tileNum = calcTileNum(i / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i + 36); TileTexture->DelTile(i + 37);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag2(tileNum)))
				TileTexture->NewTile( 36 + i    , gameStat->Deck[k + 1].tile, Normal, DeckPosV,
				DeckPosH                 + ShowTile::VertTileWidth * (i / 2), Clockwise, Reverse);
			if ((gameStat->TilePointer <= k)     && (!getRinshanFlag1(tileNum)))
				TileTexture->NewTile( 36 + i + 1, gameStat->Deck[k    ].tile, Normal, DeckPosV,
				DeckPosH - TileThickness + ShowTile::VertTileWidth * (i / 2), Clockwise, dora ? Obverse : Reverse);
		}
		break;
	case sRight:
		for (int i = (18 - std::get<2>(yamahaiAttr)) * 2; i < 36; i += 2) { /* 下家側の山 */
			unsigned tileNum = calcTileNum((std::get<2>(yamahaiAttr) * 2 - i) / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i + 72); TileTexture->DelTile(i + 73);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag2(tileNum)))
				TileTexture->NewTile( 72 + i    , gameStat->Deck[k + 1].tile, Normal, TableSize - DeckPosV,
				DeckPosH                 + ShowTile::VertTileWidth * (i / 2 - 1), Withershins, Reverse);
			if ((gameStat->TilePointer <= k)     && (!getRinshanFlag1(tileNum)))
				TileTexture->NewTile( 72 + i + 1, gameStat->Deck[k    ].tile, Normal, TableSize - DeckPosV,
				DeckPosH - TileThickness + ShowTile::VertTileWidth * (i / 2 - 1), Withershins, dora ? Obverse : Reverse);
		}
		break;
	case sSelf:
		for (int i = 0; i < std::get<2>(yamahaiAttr) * 2; i += 2) { /* 自分の山 */
			unsigned tileNum = calcTileNum(i / 2);
			unsigned k = std::get<1>(yamahaiAttr) - 2 - tileNum;
			bool dora = (k  >= gameStat->DoraPointer) && (k <= std::get<3>(yamahaiAttr));
			TileTexture->DelTile(i + 108); TileTexture->DelTile(i + 109);
			if ((gameStat->TilePointer <= k + 1) && (!getRinshanFlag2(tileNum)))
				TileTexture->NewTile(108 + i    , gameStat->Deck[k + 1].tile, Normal, DeckPosH + ShowTile::VertTileWidth * (i / 2),
				TableSize - DeckPosV,                 Portrait, Reverse);
			if ((gameStat->TilePointer <= k)     && (!getRinshanFlag1(tileNum)))
				TileTexture->NewTile(108 + i + 1, gameStat->Deck[k    ].tile, Normal, DeckPosH + ShowTile::VertTileWidth * (i / 2),
				TableSize - DeckPosV - TileThickness, Portrait, dora ? Obverse : Reverse);
		}
		break;
	}
	return;
}

/* 手牌を表示する */
void GameTableScreen::ReconstructTehai(const GameTable* gameStat, PLAYER_ID targetPlayer) {
	int tilePos;
	/* 手牌 */
	switch (playerRelative(targetPlayer, gameStat->PlayerID)) {
	case sOpposite: /* 対面の手牌 */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i)
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile)
				TileTexture->NewTile(144+i,
				gameStat->Player.val[targetPlayer].Hand[i].tile,
				gameStat->Player.val[targetPlayer].Hand[i].red,
				HandPosH + ShowTile::VertTileWidth * (HandLength - (tilePos++)) - ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				HandPosV, UpsideDown, Obverse);
			else TileTexture->DelTile(144+i);
		break;
	case sLeft: /* 上家の手牌 */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i)
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile)
				TileTexture->NewTile(144+14+i,
				gameStat->Player.val[targetPlayer].Hand[i].tile,
				gameStat->Player.val[targetPlayer].Hand[i].red,
				HandPosV,
				HandPosH + ShowTile::VertTileWidth * (tilePos++) + ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				Clockwise, Obverse);
			else TileTexture->DelTile(144+14+i);
		break;
	case sRight: /* 下家の手牌 */
		tilePos = 0;
		for (int i = HandLength; i >= 0; --i)
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile)
				++tilePos;
		for (int i = HandLength; i >= 0; --i)
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile)
				TileTexture->NewTile(144+28+(13-i),
				gameStat->Player.val[targetPlayer].Hand[i].tile,
				gameStat->Player.val[targetPlayer].Hand[i].red,
				TableSize - HandPosV,
				HandPosH + ShowTile::VertTileWidth * (HandLength - (--tilePos)) - ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				Withershins, Obverse);
			else TileTexture->DelTile(144+28+(13-i));
		break;
	case sSelf: /* 自分の手牌 */
		tilePos = 0;
		for (int i = 0; i <= HandLength; ++i)
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile)
				TileTexture->NewTile(144+42+i,
				gameStat->Player.val[targetPlayer].Hand[i].tile,
				gameStat->Player.val[targetPlayer].Hand[i].red,
				HandPosH + ShowTile::VertTileWidth * (tilePos++) + ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0),
				TableSize - HandPosV, Portrait, Obverse);
			else TileTexture->DelTile(144+42+i);
		break;
	}
}

/* 捨牌の表示する */
void GameTableScreen::ReconstructSutehai_portrait(const GameTable* gameStat, PLAYER_ID targetPlayer,
	unsigned tileID, unsigned& tilePosCol, unsigned& tilePosRow, bool& shiftPos) {
		assert(gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile != NoTile);
		switch (playerRelative(targetPlayer, gameStat->PlayerID)) {
		case sOpposite: /* 対面 */
			TileTexture->NewTile(296 - tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				TableSize - DiscardPosH - ShowTile::VertTileWidth * (tilePosCol++) - (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) * (shiftPos ? 1 : 0),
				DiscardPosV - ShowTile::HoriTileWidth * tilePosRow,
				UpsideDown, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case sLeft: /* 上家 */
			TileTexture->NewTile(297 + tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				DiscardPosV - ShowTile::HoriTileWidth * tilePosRow,
				DiscardPosH + ShowTile::VertTileWidth * (tilePosCol++) + (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) * (shiftPos ? 1 : 0),
				Clockwise, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case sRight: /* 下家 */
			TileTexture->NewTile(362 - tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				TableSize - DiscardPosV + ShowTile::HoriTileWidth * tilePosRow,
				TableSize - DiscardPosH - ShowTile::VertTileWidth * (tilePosCol++) - (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) * (shiftPos ? 1 : 0),
				Withershins, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case sSelf: /* 自分 */
			TileTexture->NewTile(363 + tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				DiscardPosH + ShowTile::VertTileWidth * (tilePosCol++) + (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) * (shiftPos ? 1 : 0),
				TableSize - DiscardPosV + ShowTile::HoriTileWidth * tilePosRow,
				Portrait, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		}
		if ((tilePosCol >= DiscardLineLength) && (tilePosRow < 2)) {
			tilePosCol = 0; ++tilePosRow; shiftPos = false;
		}
}
void GameTableScreen::ReconstructSutehai_rotated(const GameTable* gameStat, PLAYER_ID targetPlayer,
	unsigned tileID, unsigned& tilePosCol, unsigned& tilePosRow, bool& shiftPos) {
		assert(gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile != NoTile);
		switch (playerRelative(targetPlayer, gameStat->PlayerID)) {
		case sOpposite: /* 対面 */
			TileTexture->NewTile(296 - tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				TableSize - DiscardPosH - ShowTile::VertTileWidth * (tilePosCol++) - (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) / 2,
				DiscardPosV - ShowTile::HoriTileWidth * tilePosRow,
				Clockwise, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case sLeft: /* 上家 */
			TileTexture->NewTile(297 + tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				DiscardPosV - ShowTile::HoriTileWidth * tilePosRow,
				DiscardPosH + ShowTile::VertTileWidth * (tilePosCol++) + (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) / 2,
				Portrait, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case sRight: /* 下家 */
			TileTexture->NewTile(362 - tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				TableSize - DiscardPosV + ShowTile::HoriTileWidth * tilePosRow,
				TableSize - DiscardPosH - ShowTile::VertTileWidth * (tilePosCol++) - (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) / 2,
				UpsideDown, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		case sSelf: /* 自分 */
			TileTexture->NewTile(363 + tileID,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.tile,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].tcode.red,
				DiscardPosH + ShowTile::VertTileWidth * (tilePosCol++) + (ShowTile::HoriTileWidth - ShowTile::VertTileWidth) / 2,
				TableSize - DiscardPosV + ShowTile::HoriTileWidth * tilePosRow,
				Withershins, Obverse,
				gameStat->Player.val[targetPlayer].Discard[tileID + 1].isDiscardThrough ? 0xffcccccc : 0xffffffff);
			break;
		}
		shiftPos = true;
		if ((tilePosCol >= DiscardLineLength) && (tilePosRow < 2)) {
			tilePosCol = 0; ++tilePosRow; shiftPos = false;
		}
}
void GameTableScreen::ReconstructSutehai(const GameTable* gameStat, PLAYER_ID targetPlayer) {
	unsigned tilePosCol = 0, tilePosRow = 0; bool shiftPosFlag = false, riichiFlag = false;
	for (unsigned tileID = 0; tileID < 33; ++tileID) {
		switch (playerRelative(targetPlayer, gameStat->PlayerID)) {
			case sOpposite: TileTexture->DelTile(296 - tileID); break; /* 対面 */
			case sLeft:     TileTexture->DelTile(297 + tileID); break; /* 上家 */
			case sRight:    TileTexture->DelTile(362 - tileID); break; /* 下家 */
			case sSelf:     TileTexture->DelTile(363 + tileID); break; /* 自分 */
		}
	}
	for (unsigned i = 0; i < gameStat->Player.val[targetPlayer].DiscardPointer; ++i) {
		if ((gameStat->Player.val[targetPlayer].Discard[i + 1].dstat == discardRiichi) ||
			(gameStat->Player.val[targetPlayer].Discard[i + 1].dstat == discardRiichiTaken))
			riichiFlag = true;
		if ((gameStat->Player.val[targetPlayer].Discard[i + 1].dstat == discardNormal) ||
			(gameStat->Player.val[targetPlayer].Discard[i + 1].dstat == discardRiichi)) {
				if (riichiFlag) {
					ReconstructSutehai_rotated(gameStat, targetPlayer, i, tilePosCol, tilePosRow, shiftPosFlag);
					riichiFlag = false;
				} else {
					ReconstructSutehai_portrait(gameStat, targetPlayer, i, tilePosCol, tilePosRow, shiftPosFlag);
				}
		}
	}
}

void GameTableScreen::ReconstructPlayer(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer) {
	ReconstructYamahai(gameStat, targetPlayer, trueTargetPlayer);
	ReconstructTehai(gameStat, targetPlayer);
	nakihaiReconst->ReconstructNakihai(gameStat, targetPlayer);
	ReconstructSutehai(gameStat, targetPlayer);
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

/* リーチ棒表示する */
void GameTableScreen::ShowRiichibou(const GameTable* gameStat) {
	RECT rectH = {72, 30, 216, 42,}, rectV = {30, 2, 42, 146,};
	for (PLAYER_ID i = 0; i < PLAYERS; ++i) {
		if (!gameStat->Player.val[i].RichiFlag.RichiFlag) continue;
		switch (playerRelative(i, gameStat->PlayerID)) {
		case sSelf:
			SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(
				tRichi, RiichiPosH, RiichiPosV, 144, 12,
				0xffffffff, &rectH, 72, 6);
			break;
		case sOpposite:
			SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(
				tRichi, RiichiPosH, TableSize - RiichiPosV, 144, 12,
				0xffffffff, &rectH, 72, 6);
			break;
		case sLeft:
			SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(
				tRichi, TableSize - RiichiPosV, RiichiPosH, 12, 144,
				0xffffffff, &rectV, 6, 72);
			break;
		case sRight:
			SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(
				tRichi, RiichiPosV, RiichiPosH, 12, 144,
				0xffffffff, &rectV, 6, 72);
			break;
		}
	}
}

/* サイコロを表示する */
void GameTableScreen::ShowDice(const GameTable* gameStat) {
	RECT rect1 = {
		(DiceWidth + DicePadding) * (gameStat->Dice[0].Number - 1), (DiceHeight + DicePadding) * (gameStat->Dice[0].Direction    ),
		(DiceWidth + DicePadding) * (gameStat->Dice[0].Number    ), (DiceHeight + DicePadding) * (gameStat->Dice[0].Direction + 1),
	};
	RECT rect2 = {
		(DiceWidth + DicePadding) * (gameStat->Dice[1].Number - 1), (DiceHeight + DicePadding) * (gameStat->Dice[1].Direction    ),
		(DiceWidth + DicePadding) * (gameStat->Dice[1].Number    ), (DiceHeight + DicePadding) * (gameStat->Dice[1].Direction + 1),
	};
	switch (playerRelative(gameStat->GameRound % PLAYERS, gameStat->PlayerID)) {
	case sSelf:
		SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(
			tDice, DicePosH - (DiceWidth + DicePosInterstice) / 2, DicePosV,
			DiceWidth, DiceHeight, 0xffffffff, &rect1, DiceWidth / 2, DiceHeight / 2);
		SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(
			tDice, DicePosH + (DiceWidth + DicePosInterstice) / 2, DicePosV,
			DiceWidth, DiceHeight, 0xffffffff, &rect2, DiceWidth / 2, DiceHeight / 2);
		break;
	case sOpposite:
		SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(
			tDice, TableSize - DicePosH + (DiceWidth + DicePosInterstice) / 2, TableSize - DicePosV,
			DiceWidth, DiceHeight, 0xffffffff, &rect1, DiceWidth / 2, DiceHeight / 2);
		SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(
			tDice, TableSize - DicePosH - (DiceWidth + DicePosInterstice) / 2, TableSize - DicePosV,
			DiceWidth, DiceHeight, 0xffffffff, &rect2, DiceWidth / 2, DiceHeight / 2);
		break;
	case sLeft:
		SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(
			tDice, TableSize - DicePosV, DicePosH - (DiceWidth + DicePosInterstice) / 2,
			DiceWidth, DiceHeight, 0xffffffff, &rect1, DiceWidth / 2, DiceHeight / 2);
		SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(
			tDice, TableSize - DicePosV, DicePosH + (DiceWidth + DicePosInterstice) / 2,
			DiceWidth, DiceHeight, 0xffffffff, &rect2, DiceWidth / 2, DiceHeight / 2);
		break;
	case sRight:
		SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(
			tDice, DicePosV, TableSize - DicePosH - (DiceWidth + DicePosInterstice) / 2,
			DiceWidth, DiceHeight, 0xffffffff, &rect2, DiceWidth / 2, DiceHeight / 2);
		SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(
			tDice, DicePosV, TableSize - DicePosH + (DiceWidth + DicePosInterstice) / 2,
			DiceWidth, DiceHeight, 0xffffffff, &rect1, DiceWidth / 2, DiceHeight / 2);
		break;
	}
}

/* 起家マークを置く凹み */
void GameTableScreen::ShowTray() {
	RECT rect1 = {TrayHLeft, TrayHTop, TrayHRight, TrayHBottom,};
	RECT rect2 = {TrayVLeft, TrayVTop, TrayVRight, TrayVBottom,};
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tChiicha, TrayPosH, TrayPosV,
		TrayHWidth, TrayHHeight, 0xffffffff, &rect1, TrayHWidth / 2, TrayHHeight / 2);
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tChiicha, TableSize - TrayPosH, TableSize - TrayPosV,
		TrayHWidth, TrayHHeight, 0xffffffff, &rect1, TrayHWidth / 2, TrayHHeight / 2);
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tChiicha, TableSize - TrayPosV, TrayPosH,
		TrayVWidth, TrayVHeight, 0xffffffff, &rect2, TrayVWidth / 2, TrayVHeight / 2);
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tChiicha, TrayPosV, TableSize - TrayPosH,
		TrayVWidth, TrayVHeight, 0xffffffff, &rect2, TrayVWidth / 2, TrayVHeight / 2);
}

/* 起家マークの表示 */
void GameTableScreen::ShowChiicha(const GameTable* gameStat) {
	switch (playerRelative(0, gameStat->PlayerID)) {
	case sSelf:
		{
			RECT rect = {
				(PlateWidthH + PlatePadding * 2) * (gameStat->GameRound / PLAYERS    ) + PlatePadding, (PlateHeightH + PlatePadding * 2) * (0    ) + PlatePadding,
				(PlateWidthH + PlatePadding * 2) * (gameStat->GameRound / PLAYERS + 1) - PlatePadding, (PlateHeightH + PlatePadding * 2) * (0 + 1) - PlatePadding,
			};
			SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tChiicha, PlatePosH, PlatePosV,
				PlateWidthH, PlateHeightH, 0xffffffff, &rect, PlateWidthH / 2, PlateHeightH / 2);
		}
		break;
	case sOpposite:
		{
			RECT rect = {
				(PlateWidthH + PlatePadding * 2) * (gameStat->GameRound / PLAYERS    ) + PlatePadding, (PlateHeightH + PlatePadding * 2) * (1    ) + PlatePadding,
				(PlateWidthH + PlatePadding * 2) * (gameStat->GameRound / PLAYERS + 1) - PlatePadding, (PlateHeightH + PlatePadding * 2) * (1 + 1) - PlatePadding,
			};
			SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tChiicha, TableSize - PlatePosH, TableSize - PlatePosV,
				PlateWidthH, PlateHeightH, 0xffffffff, &rect, PlateWidthH / 2, PlateHeightH / 2);
		}
		break;
	case sRight:
		{
			RECT rect = {
				(PlateWidthV + PlatePadding * 2) * (gameStat->GameRound / PLAYERS    ) + PlatePadding, (PlateHeightV + PlatePadding * 2) * (0    ) + PlatePadding + (PlateHeightH + PlatePadding * 2) * 2,
				(PlateWidthV + PlatePadding * 2) * (gameStat->GameRound / PLAYERS + 1) - PlatePadding, (PlateHeightV + PlatePadding * 2) * (0 + 1) - PlatePadding + (PlateHeightH + PlatePadding * 2) * 2,
			};
			SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tChiicha, PlatePosV, TableSize - PlatePosH,
				PlateWidthV, PlateHeightV, 0xffffffff, &rect, PlateWidthV / 2, PlateHeightV / 2);
		}
		break;
	case sLeft:
		{
			RECT rect = {
				(PlateWidthV + PlatePadding * 2) * (gameStat->GameRound / PLAYERS    ) + PlatePadding, (PlateHeightV + PlatePadding * 2) * (1    ) + PlatePadding + (PlateHeightH + PlatePadding * 2) * 2,
				(PlateWidthV + PlatePadding * 2) * (gameStat->GameRound / PLAYERS + 1) - PlatePadding, (PlateHeightV + PlatePadding * 2) * (1 + 1) - PlatePadding + (PlateHeightH + PlatePadding * 2) * 2,
			};
			SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tChiicha, TableSize - PlatePosV, PlatePosH,
				PlateWidthV, PlateHeightV, 0xffffffff, &rect, PlateWidthV / 2, PlateHeightV / 2);
		}
		break;
	}
}

/* ヤキトリマークの表示 */
void GameTableScreen::ShowYakitori(const GameTable* gameStat) {
	for (PLAYER_ID i = 0; i < PLAYERS; ++i) {
		if (!gameStat->Player.val[i].YakitoriFlag) continue;
		switch (playerRelative(i, gameStat->PlayerID)) {
		case sSelf:
			{
				RECT rect = {
					(PlateWidthH + PlatePadding * 2) * (PlateID_Yakitori    ) + PlatePadding, (PlateHeightH + PlatePadding * 2) * (0    ) + PlatePadding,
					(PlateWidthH + PlatePadding * 2) * (PlateID_Yakitori + 1) - PlatePadding, (PlateHeightH + PlatePadding * 2) * (0 + 1) - PlatePadding,
				};
				SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tChiicha, YakitoriPosH, YakitoriPosV,
					PlateWidthH, PlateHeightH, 0xffffffff, &rect, PlateWidthH / 2, PlateHeightH / 2);
			}
			break;
		case sOpposite:
			{
				RECT rect = {
					(PlateWidthH + PlatePadding * 2) * (PlateID_Yakitori    ) + PlatePadding, (PlateHeightH + PlatePadding * 2) * (1    ) + PlatePadding,
					(PlateWidthH + PlatePadding * 2) * (PlateID_Yakitori + 1) - PlatePadding, (PlateHeightH + PlatePadding * 2) * (1 + 1) - PlatePadding,
				};
				SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tChiicha, TableSize - YakitoriPosH, TableSize - YakitoriPosV,
					PlateWidthH, PlateHeightH, 0xffffffff, &rect, PlateWidthH / 2, PlateHeightH / 2);
			}
			break;
		case sRight:
			{
				RECT rect = {
					(PlateWidthV + PlatePadding * 2) * (PlateID_Yakitori    ) + PlatePadding, (PlateHeightV + PlatePadding * 2) * (0    ) + PlatePadding + (PlateHeightH + PlatePadding * 2) * 2,
					(PlateWidthV + PlatePadding * 2) * (PlateID_Yakitori + 1) - PlatePadding, (PlateHeightV + PlatePadding * 2) * (0 + 1) - PlatePadding + (PlateHeightH + PlatePadding * 2) * 2,
				};
				SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tChiicha, YakitoriPosV, TableSize - YakitoriPosH,
					PlateWidthV, PlateHeightV, 0xffffffff, &rect, PlateWidthV / 2, PlateHeightV / 2);
			}
			break;
		case sLeft:
			{
				RECT rect = {
					(PlateWidthV + PlatePadding * 2) * (PlateID_Yakitori    ) + PlatePadding, (PlateHeightV + PlatePadding * 2) * (1    ) + PlatePadding + (PlateHeightH + PlatePadding * 2) * 2,
					(PlateWidthV + PlatePadding * 2) * (PlateID_Yakitori + 1) - PlatePadding, (PlateHeightV + PlatePadding * 2) * (1 + 1) - PlatePadding + (PlateHeightH + PlatePadding * 2) * 2,
				};
				SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tChiicha, TableSize - YakitoriPosV, YakitoriPosH,
					PlateWidthV, PlateHeightV, 0xffffffff, &rect, PlateWidthV / 2, PlateHeightV / 2);
			}
			break;
		}
	}
}

/* 卓を表示 ここから */
void GameTableScreen::cls() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0); // バッファクリア
}

void GameTableScreen::RenderTable() {
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tBaize, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tBorder, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	if (GameStatus::isModified())
		Reconstruct(GameStatus::retrGameStat());
	ShowRiichibou(GameStatus::gameStat());
	ShowDice(GameStatus::gameStat());
	ShowTray();
	ShowChiicha(GameStatus::gameStat());
	ShowYakitori(GameStatus::gameStat());
	TileTexture->Render();
}

void GameTableScreen::RenderSideBar() {
	ShowSidebar();
	ShowScorePanel();
	logWindow->Render();
}

void GameTableScreen::Render() {
	cls();
	RenderTable();
	if (TryEnterCriticalSection(&subSceneCS)) {
		mySubScene->Render();
		LeaveCriticalSection(&subSceneCS);
	}
	RenderSideBar();
}

void GameTableScreen::SetSubscene(unsigned int scene_ID) {
	EnterCriticalSection(&subSceneCS);
	delete mySubScene;
	switch (static_cast<TableSubsceneID>(scene_ID)) {
	case tblSubsceneBeginning:
		mySubScene = new TableSubsceneBeginning(caller->getDevice());
		break;
	case tblSubsceneHonba:
		mySubScene = new TableSubsceneTitlingHonba(caller->getDevice());
		break;
	case tblSubsceneRyuukyoku:
		mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("流局"));
		break;
	case tblSubsceneSifeng:
		mySubScene = new TableSubsceneMsg(caller->getDevice(),
			(GameStatus::gameStat()->gameType & AllSanma) ? _T("三風連打") : _T("四風連打"));
		break;
	case tblSubsceneTripleRon:
		mySubScene = new TableSubsceneMsg(caller->getDevice(),
			(GameStatus::gameStat()->gameType & AllSanma) ? _T("二家和") : _T("三家和"));
		break;
	case tblSubsceneSikang:
		mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("四開槓"));
		break;
	case tblSubsceneFourRiichi:
		mySubScene = new TableSubsceneMsg(caller->getDevice(),
			(GameStatus::gameStat()->gameType & AllSanma) ? _T("三家立直") : _T("四家立直"));
		break;
	case tblSubsceneCall:
		mySubScene = new TableSubsceneCall(caller->getDevice());
		break;
	case tblSubsceneCallFade:
		mySubScene = new TableSubsceneCallFade(caller->getDevice());
		break;
	case tblSubsceneChkTenpai:
		mySubScene = new TableSubsceneCheckTenpai(caller->getDevice());
		break;
	default:
		mySubScene = new TableSubsceneNormal(caller->getDevice());
		break;
	}
	LeaveCriticalSection(&subSceneCS);
}

// -------------------------------------------------------------------------

/* 鳴いた牌を表示する */
std::tuple<std::function<unsigned (unsigned)>, std::function<int (unsigned)>, std::function<int (unsigned)>, TileDirection, TileDirection, TileDirection>
	GameTableScreen::NakihaiReconst::playerPosition(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID,
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
	default:
		throw "Argument error";
	}
}
void GameTableScreen::NakihaiReconst::NakihaiAnkan(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
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
void GameTableScreen::NakihaiReconst::NakihaiKamicha(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
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
void GameTableScreen::NakihaiReconst::NakihaiToimen(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
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
void GameTableScreen::NakihaiReconst::NakihaiShimocha(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
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
void GameTableScreen::NakihaiReconst::MinkanKamicha(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
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
void GameTableScreen::NakihaiReconst::MinkanToimen(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
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
void GameTableScreen::NakihaiReconst::MinkanShimocha(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
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
void GameTableScreen::NakihaiReconst::NakihaiSelRoutine(const GameTable* gameStat, PLAYER_ID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID) {
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
void GameTableScreen::NakihaiReconst::ReconstructNakihai(const GameTable* gameStat, PLAYER_ID targetPlayer) {
	unsigned posOffset[5] = {0,};
	for (int i = 1; i <= gameStat->Player.val[targetPlayer].MeldPointer; ++i) {
		switch (gameStat->Player.val[targetPlayer].Meld[i].mstat) {
		case meldQuadAddedLeft: case meldQuadAddedCenter: case meldQuadAddedRight:
			posOffset[i] = posOffset[i - 1] + ShowTile::VertTileWidth * 2;
			break;
		default:
			posOffset[i] = posOffset[i - 1] + ShowTile::HoriTileWidth;
			break;
		}
	}
	for (int i = 1; i <= gameStat->Player.val[targetPlayer].MeldPointer; ++i)
		NakihaiSelRoutine(gameStat, targetPlayer, posOffset[i - 1],
			(gameStat->Player.val[targetPlayer].MeldPointer - i) * 4, i);
}

}
