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

#include "table/yamahai.h"
#include "table/tehai.h"
#include "table/nakihai.h"
#include "table/sutehai.h"

namespace mihajong_graphic {

using utils::playerRelative;
	
GameTableScreen::GameTableScreen(ScreenManipulator* const manipulator) : TableProtoScene(manipulator) {
	LoadTexture(&tBorder, MAKEINTRESOURCE(IDB_PNG_TBLBORDER), 1080, 1080);
	LoadTexture(&tBaize, MAKEINTRESOURCE(IDB_PNG_TBLBAIZE), 1080, 1080);
	LoadTexture(&tChiicha, MAKEINTRESOURCE(IDB_PNG_CHICHAMARK), 419, 174);
	LoadTexture(&tRichi, MAKEINTRESOURCE(IDB_PNG_TENBOU), 218, 148);
	LoadTexture(&tDice, MAKEINTRESOURCE(IDB_PNG_DICE), 156, 144);
	yamahaiReconst = new YamahaiReconst(this);
	tehaiReconst = new TehaiReconst(this);
	nakihaiReconst = new NakihaiReconst(this);
	sutehaiReconst = new SutehaiReconst(this);
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
	delete sutehaiReconst;
	delete nakihaiReconst;
	delete tehaiReconst;
	delete yamahaiReconst;
	if (tDice) tDice->Release();
	if (tRichi) tRichi->Release();
	if (tChiicha) tChiicha->Release();
	if (tBorder) tBorder->Release();
	if (tBaize) tBaize->Release();
}

void GameTableScreen::ReconstructPlayer(const GameTable* gameStat, PLAYER_ID targetPlayer, PLAYER_ID trueTargetPlayer) {
	yamahaiReconst->Reconstruct(gameStat, targetPlayer, trueTargetPlayer);
	tehaiReconst->Reconstruct(gameStat, targetPlayer);
	nakihaiReconst->Reconstruct(gameStat, targetPlayer);
	sutehaiReconst->Reconstruct(gameStat, targetPlayer);
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
	yamahaiReconst->Render(); // 0
	tehaiReconst->Render(); // 144
	nakihaiReconst->Render(); // 200
	sutehaiReconst->Render(); // 264
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
	delete mySubScene; tehaiReconst->setTileCursor();
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
		tehaiReconst->setTileCursor(NUM_OF_TILES_IN_HAND - 1);
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
		tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
	};
	const PlayerTable* const plDat = &(GameStatus::gameStat()->Player.val[GameStatus::gameStat()->PlayerID]);
	switch (od->dwOfs) {
	case DIK_LEFT:
		if ((od->dwData) && (tehaiReconst->isTileCursorDisable())) {
			do {
				if (tehaiReconst->decrTileCursor() < 0) tehaiReconst->setTileCursor(NUM_OF_TILES_IN_HAND - 1);
			} while (plDat->Hand[tehaiReconst->getTileCursor()].tile == NoTile);
			cursorMoved();
		}
		break;
	case DIK_RIGHT:
		if ((od->dwData) && (tehaiReconst->isTileCursorDisable())) {
			do {
				if (tehaiReconst->incrTileCursor() >= NUM_OF_TILES_IN_HAND) tehaiReconst->setTileCursor(0);
			} while (plDat->Hand[tehaiReconst->getTileCursor()].tile == NoTile);
			cursorMoved();
		}
		break;
	/* カーソル位置の直截指定 */
	/* ASSUMING JAPANESE KEYBOARD: 1 2 3 4 5 6 7 8 9 0 - ^ ￥ BS */
	case DIK_1:          if ((od->dwData) && (tehaiReconst->isTileCursorDisable()) && (plDat->Hand[ 0].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  0) FinishTileChoice(); else {tehaiReconst->setTileCursor( 0); cursorMoved();}} break;
	case DIK_2:          if ((od->dwData) && (tehaiReconst->isTileCursorDisable()) && (plDat->Hand[ 1].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  1) FinishTileChoice(); else {tehaiReconst->setTileCursor( 1); cursorMoved();}} break;
	case DIK_3:          if ((od->dwData) && (tehaiReconst->isTileCursorDisable()) && (plDat->Hand[ 2].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  2) FinishTileChoice(); else {tehaiReconst->setTileCursor( 2); cursorMoved();}} break;
	case DIK_4:          if ((od->dwData) && (tehaiReconst->isTileCursorDisable()) && (plDat->Hand[ 3].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  3) FinishTileChoice(); else {tehaiReconst->setTileCursor( 3); cursorMoved();}} break;
	case DIK_5:          if ((od->dwData) && (tehaiReconst->isTileCursorDisable()) && (plDat->Hand[ 4].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  4) FinishTileChoice(); else {tehaiReconst->setTileCursor( 4); cursorMoved();}} break;
	case DIK_6:          if ((od->dwData) && (tehaiReconst->isTileCursorDisable()) && (plDat->Hand[ 5].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  5) FinishTileChoice(); else {tehaiReconst->setTileCursor( 5); cursorMoved();}} break;
	case DIK_7:          if ((od->dwData) && (tehaiReconst->isTileCursorDisable()) && (plDat->Hand[ 6].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  6) FinishTileChoice(); else {tehaiReconst->setTileCursor( 6); cursorMoved();}} break;
	case DIK_8:          if ((od->dwData) && (tehaiReconst->isTileCursorDisable()) && (plDat->Hand[ 7].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  7) FinishTileChoice(); else {tehaiReconst->setTileCursor( 7); cursorMoved();}} break;
	case DIK_9:          if ((od->dwData) && (tehaiReconst->isTileCursorDisable()) && (plDat->Hand[ 8].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  8) FinishTileChoice(); else {tehaiReconst->setTileCursor( 8); cursorMoved();}} break;
	case DIK_0:          if ((od->dwData) && (tehaiReconst->isTileCursorDisable()) && (plDat->Hand[ 9].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  9) FinishTileChoice(); else {tehaiReconst->setTileCursor( 9); cursorMoved();}} break;
	case DIK_MINUS:      if ((od->dwData) && (tehaiReconst->isTileCursorDisable()) && (plDat->Hand[10].tile != NoTile)) {if (tehaiReconst->getTileCursor() == 10) FinishTileChoice(); else {tehaiReconst->setTileCursor(10); cursorMoved();}} break;
	case DIK_CIRCUMFLEX: if ((od->dwData) && (tehaiReconst->isTileCursorDisable()) && (plDat->Hand[11].tile != NoTile)) {if (tehaiReconst->getTileCursor() == 11) FinishTileChoice(); else {tehaiReconst->setTileCursor(11); cursorMoved();}} break;
	case DIK_YEN:        if ((od->dwData) && (tehaiReconst->isTileCursorDisable()) && (plDat->Hand[12].tile != NoTile)) {if (tehaiReconst->getTileCursor() == 12) FinishTileChoice(); else {tehaiReconst->setTileCursor(12); cursorMoved();}} break;
	case DIK_BACK:       if ((od->dwData) && (tehaiReconst->isTileCursorDisable()) && (plDat->Hand[13].tile != NoTile)) {if (tehaiReconst->getTileCursor() == 13) FinishTileChoice(); else {tehaiReconst->setTileCursor(13); cursorMoved();}} break;
	/* 決定キー */
	case DIK_RETURN: case DIK_SPACE: case DIK_Z:
		if ((od->dwData) && (tehaiReconst->isTileCursorDisable()))
			FinishTileChoice();
		break;
	}
}

void GameTableScreen::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {
	const int scaledX = X / Geometry::WindowScale() * (Geometry::WindowWidth * 0.75f / Geometry::WindowHeight);
	const int scaledY = Y / Geometry::WindowScale();
	const int region = whichRegion(scaledX, scaledY);
	const bool isValidTile = (region >= 0) && (region < NUM_OF_TILES_IN_HAND) &&
		(region != tehaiReconst->getTileCursor()) && (tehaiReconst->isTileCursorDisable()) &&
		(GameStatus::gameStat()->Player.val[GameStatus::gameStat()->PlayerID].Hand[region].tile != NoTile);
	switch (od->dwOfs) {
	case DIMOFS_X: case DIMOFS_Y: // マウスカーソルを動かした場合
		if (isValidTile) {
			tehaiReconst->setTileCursor(region);
			sound::Play(sound::IDs::sndCursor);
			tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
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
	ui::UIEvent->set((unsigned)tehaiReconst->getTileCursor()); // 牌の番号を設定
}

}
