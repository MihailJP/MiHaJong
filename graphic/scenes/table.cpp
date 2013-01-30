#include "table.h"
#include "../scrmanip.h"
#include "../resource.h"
#include "../rule.h"
#include "../sprite.h"
#include "../utils.h"
#include "../geometry.h"
#include <tuple>
#include <cassert>
#include <cmath>
#include "scene_id.h"
#include "../../sound/sound.h"
#include "../../mjcore/bgmid.h"
#include "../event.h"

#include "table/nakihai.h"

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
	tileCursor = tileCursorOff;
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
		for (int i = 0; i <= HandLength; ++i) {
			if (gameStat->Player.val[targetPlayer].Hand[i].tile != NoTile) {
				const int tileX = HandPosH + ShowTile::VertTileWidth * (tilePos++) + ((i == HandLength) && (!gameStat->TianHuFlag) ? ShowTile::VertTileWidth / 3 : 0);
				const int tileY = TableSize - HandPosV;
				TileTexture->NewTile(144+42+i,
					gameStat->Player.val[targetPlayer].Hand[i].tile,
					gameStat->Player.val[targetPlayer].Hand[i].red,
					tileX, tileY, Portrait, Obverse,
					(tileCursor == i) ? 0xffff9999 : 0xffffffff);
				if (regions.size() <= i) regions.resize(i + 1);
				const Region newRegion = {
					tileX - ShowTile::VertTileWidth / 2, tileY - ShowTile::VertTileHeight / 2,
					tileX + ShowTile::VertTileWidth / 2, tileY + ShowTile::VertTileHeight / 2,
				};
				regions[i] = newRegion;
			} else {
				TileTexture->DelTile(144+42+i);
				if (regions.size() <= i) regions.resize(i + 1);
				const Region nullRegion = {0, 0, -1, -1};
				regions[i] = nullRegion;
			}
		}
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
	delete mySubScene; tileCursor = tileCursorOff;
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
	case tblSubsceneChonbo:
		mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("錯和"));
		break;
	case tblSubsceneCall:
		mySubScene = new TableSubsceneCall(caller->getDevice());
		break;
	case tblSubsceneCallFade:
		mySubScene = new TableSubsceneCallFade(caller->getDevice());
		break;
	case tblSubsceneCallVal:
		mySubScene = new TableSubsceneCallValue(caller->getDevice());
		break;
	case tblSubsceneCallValNotenBappu:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("不聴罰符"));
		break;
	case tblSubsceneCallValAgariten:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("和了点"));
		break;
	case tblSubsceneCallValTsumibou:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("積棒精算"));
		break;
	case tblSubsceneCallValChip:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("祝儀精算"));
		break;
	case tblSubsceneCallValKyoutaku:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("供託精算"));
		break;
	case tblSubsceneCallValChonboBappu:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("錯和罰符"));
		break;
	case tblSubsceneCallValNagashiMangan:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("流し満貫"));
		break;
	case tblSubsceneCallValDobon:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("飛び罰符"));
		break;
	case tblSubsceneCallValKitamakura:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("北枕罰符"));
		break;
	case tblSubsceneCallValYakuman:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("役満祝儀"));
		break;
	case tblSubsceneChkTenpai:
		mySubScene = new TableSubsceneCheckTenpai(caller->getDevice());
		break;
	case tblSubscenePlayerDahai:
		mySubScene = new TableSubscenePlayerDahai(caller->getDevice());
		tileCursor = NUM_OF_TILES_IN_HAND - 1;
		break;
	default:
		mySubScene = new TableSubsceneNormal(caller->getDevice());
		break;
	}
	LeaveCriticalSection(&subSceneCS);
}

// -------------------------------------------------------------------------

void GameTableScreen::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
	auto cursorMoved = [&]() -> void {
		sound::Play(sound::IDs::sndCursor);
		ReconstructTehai(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
	};
	const PlayerTable* const plDat = &(GameStatus::gameStat()->Player.val[GameStatus::gameStat()->PlayerID]);
	switch (od->dwOfs) {
	case DIK_LEFT:
		if ((od->dwData) && (tileCursor != tileCursorOff)) {
			do {
				if ((--tileCursor) < 0) tileCursor = NUM_OF_TILES_IN_HAND - 1;
			} while (plDat->Hand[tileCursor].tile == NoTile);
			cursorMoved();
		}
		break;
	case DIK_RIGHT:
		if ((od->dwData) && (tileCursor != tileCursorOff)) {
			do {
				if ((++tileCursor) >= NUM_OF_TILES_IN_HAND) tileCursor = 0;
			} while (plDat->Hand[tileCursor].tile == NoTile);
			cursorMoved();
		}
		break;
	/* カーソル位置の直截指定 */
	/* ASSUMING JAPANESE KEYBOARD: 1 2 3 4 5 6 7 8 9 0 - ^ ￥ BS */
	case DIK_1:          if ((od->dwData) && (tileCursor != tileCursorOff) && (plDat->Hand[ 0].tile != NoTile)) {if (tileCursor ==  0) FinishTileChoice(); else {tileCursor =  0; cursorMoved();}} break;
	case DIK_2:          if ((od->dwData) && (tileCursor != tileCursorOff) && (plDat->Hand[ 1].tile != NoTile)) {if (tileCursor ==  1) FinishTileChoice(); else {tileCursor =  1; cursorMoved();}} break;
	case DIK_3:          if ((od->dwData) && (tileCursor != tileCursorOff) && (plDat->Hand[ 2].tile != NoTile)) {if (tileCursor ==  2) FinishTileChoice(); else {tileCursor =  2; cursorMoved();}} break;
	case DIK_4:          if ((od->dwData) && (tileCursor != tileCursorOff) && (plDat->Hand[ 3].tile != NoTile)) {if (tileCursor ==  3) FinishTileChoice(); else {tileCursor =  3; cursorMoved();}} break;
	case DIK_5:          if ((od->dwData) && (tileCursor != tileCursorOff) && (plDat->Hand[ 4].tile != NoTile)) {if (tileCursor ==  4) FinishTileChoice(); else {tileCursor =  4; cursorMoved();}} break;
	case DIK_6:          if ((od->dwData) && (tileCursor != tileCursorOff) && (plDat->Hand[ 5].tile != NoTile)) {if (tileCursor ==  5) FinishTileChoice(); else {tileCursor =  5; cursorMoved();}} break;
	case DIK_7:          if ((od->dwData) && (tileCursor != tileCursorOff) && (plDat->Hand[ 6].tile != NoTile)) {if (tileCursor ==  6) FinishTileChoice(); else {tileCursor =  6; cursorMoved();}} break;
	case DIK_8:          if ((od->dwData) && (tileCursor != tileCursorOff) && (plDat->Hand[ 7].tile != NoTile)) {if (tileCursor ==  7) FinishTileChoice(); else {tileCursor =  7; cursorMoved();}} break;
	case DIK_9:          if ((od->dwData) && (tileCursor != tileCursorOff) && (plDat->Hand[ 8].tile != NoTile)) {if (tileCursor ==  8) FinishTileChoice(); else {tileCursor =  8; cursorMoved();}} break;
	case DIK_0:          if ((od->dwData) && (tileCursor != tileCursorOff) && (plDat->Hand[ 9].tile != NoTile)) {if (tileCursor ==  9) FinishTileChoice(); else {tileCursor =  9; cursorMoved();}} break;
	case DIK_MINUS:      if ((od->dwData) && (tileCursor != tileCursorOff) && (plDat->Hand[10].tile != NoTile)) {if (tileCursor == 10) FinishTileChoice(); else {tileCursor = 10; cursorMoved();}} break;
	case DIK_CIRCUMFLEX: if ((od->dwData) && (tileCursor != tileCursorOff) && (plDat->Hand[11].tile != NoTile)) {if (tileCursor == 11) FinishTileChoice(); else {tileCursor = 11; cursorMoved();}} break;
	case DIK_YEN:        if ((od->dwData) && (tileCursor != tileCursorOff) && (plDat->Hand[12].tile != NoTile)) {if (tileCursor == 12) FinishTileChoice(); else {tileCursor = 12; cursorMoved();}} break;
	case DIK_BACK:       if ((od->dwData) && (tileCursor != tileCursorOff) && (plDat->Hand[13].tile != NoTile)) {if (tileCursor == 13) FinishTileChoice(); else {tileCursor = 13; cursorMoved();}} break;
	/* 決定キー */
	case DIK_RETURN: case DIK_SPACE: case DIK_Z:
		if ((od->dwData) && (tileCursor != tileCursorOff))
			FinishTileChoice();
		break;
	}
}

void GameTableScreen::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {
	const int scaledX = X / Geometry::WindowScale() * (Geometry::WindowWidth * 0.75f / Geometry::WindowHeight);
	const int scaledY = Y / Geometry::WindowScale();
	const int region = whichRegion(scaledX, scaledY);
	const bool isValidTile = (region >= 0) && (region < NUM_OF_TILES_IN_HAND) && (region != tileCursor) &&
		(tileCursor != tileCursorOff) &&
		(GameStatus::gameStat()->Player.val[GameStatus::gameStat()->PlayerID].Hand[region].tile != NoTile);
	switch (od->dwOfs) {
	case DIMOFS_X: case DIMOFS_Y: // マウスカーソルを動かした場合
		if (isValidTile) {
			tileCursor = region;
			sound::Play(sound::IDs::sndCursor);
			ReconstructTehai(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
		}
		break;
	case DIMOFS_BUTTON0: // マウスクリック
		if ((isValidTile) && (od->dwData))
			FinishTileChoice();
		break;
	}
}

/* 捨牌を決定する */
void GameTableScreen::FinishTileChoice() {
	ui::UIEvent->set((unsigned)tileCursor); // 牌の番号を設定
}

}
