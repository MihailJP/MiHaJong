#include "table.h"
#include "../resource.h"
#include "../rule.h"
#include "../sprite.h"
#include <iomanip>
#include "../../sound/sound.h"
#include "../../common/bgmid.h"
#include "../event.h"
#include "../../common/discard.h"
#include "../utils.h"
#include "../extchar.h"
#include <functional>
#ifndef _WIN32
#include "../keycode.h"
#endif /*_WIN32*/

#include "table/yamahai.h"
#include "table/tehai.h"
#include "table/nakihai.h"
#include "table/sutehai.h"
#include "table/showdice.h"
#include "table/richibou.h"
#include "table/chicha.h"
#include "table/nakibtn.h"
#include "table/gari.h"
#include "table/tiletip.h"
#include "table/clock.h"

#include "table/naki_id.h"

namespace mihajong_graphic {
	
GameTableScreen::GameTableScreen(ScreenManipulator* const manipulator) : TableProtoScene(manipulator) {
	LoadTexture(&tBorder, MAKEINTRESOURCE(IDB_PNG_TBLBORDER));
	LoadTexture(&tBaize, MAKEINTRESOURCE(IDB_PNG_TBLBAIZE));
	buttonReconst = new ButtonReconst(this);
	trayReconst = new TrayReconst(this);
	richibouReconst = new RichibouReconst(this);
	diceReconst = new DiceReconst(this);
	yamahaiReconst = new YamahaiReconst(this);
	tehaiReconst = new TehaiReconst(this);
	nakihaiReconst = new NakihaiReconst(this);
	sutehaiReconst = new SutehaiReconst(this);
	gariReconst = new GariReconst(this);
	clockPanel = new Clock(this);
	tileTipReconst = new TileTipReconst(this);
	Reconstruct(GameStatus::retrGameStat());
	const unsigned logWidth = (unsigned)floor(0.5f + // VC++2010ではround()が使えない
		(float)(((signed)Geometry::WindowWidth - (signed)Geometry::WindowHeight) / Geometry::WindowScale() - 36)) / 9u;
	logWindow = new logwnd::LogWindow(caller->getHWnd(), caller->getDevice(),
		1100, 100, logWidth, 20);
	chatInput = new EditBox(caller->getHWnd(), caller->getDevice(),
		1100, 100 + 20 * 20 + 10, logWidth);
	setRegion(ChatInputRegion,
		1100               , 100 + 20 * 20 + 10,
		1100 + logWidth * 9, 100 + 20 * 20 + 10 + 20);
	mySubScene = new TableSubsceneNormal(manipulator->getDevice());
	myTextRenderer = new TextRenderer(manipulator->getDevice());
	tileSelectMode = 0;
#ifndef _WIN32
	reconstructFlag = true;
#endif /*_WIN32*/
}

GameTableScreen::~GameTableScreen() {
	delete mySubScene;
	delete chatInput;
	delete logWindow;
	delete tileTipReconst;
	delete gariReconst;
	delete sutehaiReconst;
	delete nakihaiReconst;
	delete tehaiReconst;
	delete yamahaiReconst;
	delete diceReconst;
	delete richibouReconst;
	delete trayReconst;
	delete buttonReconst;
	delete clockPanel;
	delete myTextRenderer;
#if defined(_WIN32) && defined(WITH_DIRECTX)
	if (tBorder) tBorder->Release();
	if (tBaize) tBaize->Release();
#endif
}

void GameTableScreen::ReconstructPlayer(const GameTable* gameStat, PlayerID deckTargetPlayer, PlayerID targetPlayer) {
	yamahaiReconst->Reconstruct(gameStat, deckTargetPlayer, targetPlayer);
	tehaiReconst->Reconstruct(gameStat, targetPlayer);
	nakihaiReconst->Reconstruct(gameStat, targetPlayer);
	sutehaiReconst->Reconstruct(gameStat, targetPlayer);
}

void GameTableScreen::Reconstruct(const GameTable* gameStat) {
	if (gameStat->chkGameType(Yonma) || gameStat->chkGameType(GuobiaoMJ)) {
		for (PlayerID i = 0; i < 4; i++)
			ReconstructPlayer(gameStat, i, i);
	} else if (gameStat->chkGameType(Sanma4)) {
		const PlayerID tobePlayed[4][4] = {
			{0, 1, 2, 3}, {3, 1, 2, 0}, {1, 3, 2, 0}, {1, 2, 3, 0},
		};
		for (PlayerID i = 0; i < 4; i++) {
			PlayerID j(tobePlayed[gameStat->GameRound % Players][i]);
			if (j < 3) ReconstructPlayer(gameStat, j, i);
		}
	} else {
		for (PlayerID i = 0; i < 3; i++)
			ReconstructPlayer(gameStat, i, i);
	}
}

/* 供託点棒などの情報を表示 */
void GameTableScreen::ShowStatus(const GameTable* gameStat) {
	const wchar_t* const WindName = L"東南西北白發中";
	const wchar_t* const Numeral = L"一二三四五六七八九十";
	const wchar_t* const FWDigit = L"０１２３４５６７８９";
	CodeConv::tostringstream o;
	o << CodeConv::EnsureTStr(std::wstring(WindName + gameStat->GameRound / 4, WindName + gameStat->GameRound / 4 + 1));
	if (rules::chkRule("game_length", "twice_east_game") || rules::chkRule("game_length", "east_only_game")) { // 東場のみのルール
		const unsigned roundnum = gameStat->LoopRound * 4 + gameStat->GameRound;
		if (roundnum < 10)
			o << CodeConv::EnsureTStr(std::wstring(Numeral + roundnum, Numeral + roundnum + 1));
		else
			o << (roundnum + 1);
	} else { // 一般のルール
		const unsigned roundnum = gameStat->GameRound % 4;
		o << CodeConv::EnsureTStr(std::wstring(Numeral + roundnum, Numeral + roundnum + 1));
	}
	o << _T("局 ");
	if (!gameStat->chkGameType(GuobiaoMJ)) {
		if (gameStat->Honba >= 10) o << std::setw(2) << (gameStat->Honba) << _T("本場");
		else if (gameStat->Honba > 0) o << CodeConv::EnsureTStr(std::wstring(FWDigit + gameStat->Honba, FWDigit + gameStat->Honba + 1)) << _T("本場");
		else o << _T("平　場");
	}
	myTextRenderer->NewText(0, o.str(), Geometry::BaseSize + 5, 5, 0.875f);

	o.str(_T(""));
	const int tiles = gameStat->RinshanPointer - gameStat->TilePointer - gameStat->ExtraRinshan - gameStat->DeadTiles + 1;
	if (tiles >= 100) o << _T("残--牌");
	else o << _T("残") << std::setw(2) << tiles << _T("牌");
	if (!gameStat->chkGameType(GuobiaoMJ)) {
		o << _T(" 供託");
		if (gameStat->Deposit) o << std::setw(2) << gameStat->Deposit << _T("本");
		else o << _T("なし");
	}
	myTextRenderer->NewText(1, o.str(), Geometry::BaseSize + 5, 35, 0.875f);

	myTextRenderer->Render();
}

/* 卓を表示 ここから */
void GameTableScreen::cls() {
#if defined(_WIN32) && defined(WITH_DIRECTX)
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
		roundColor(), 1.0f, 0); // バッファクリア
#else
	glClearColor(
		(double)((roundColor() & 0x00ff0000) >> 16) / 255.0,
		(double)((roundColor() & 0x0000ff00) >>  8) / 255.0,
		(double)((roundColor() & 0x000000ff)      ) / 255.0,
		(double)((roundColor() & 0xff000000) >> 24) / 255.0);
	glClear(GL_COLOR_BUFFER_BIT);
#endif
}

void GameTableScreen::RenderTable() {
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tBaize, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tBorder, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	Reconstruct(GameStatus::retrGameStat());
	clockPanel->Render();
	richibouReconst->Render();
	diceReconst->Render();
	trayReconst->Render();
	yamahaiReconst->Render(); // 0
	tehaiReconst->Render(); // 144
	nakihaiReconst->Render(); // 200
	sutehaiReconst->Render(); // 264
	gariReconst->Render();
	buttonReconst->Render();
	tileTipReconst->Render();
}

void GameTableScreen::RenderSideBar() {
	ShowSidebar();
	ShowStatus(GameStatus::gameStat());
	ShowScorePanel();
	logWindow->Render();
	chatInput->Render();
}

void GameTableScreen::checkTimeout() {
	if ((mySubScene) && (mySubScene->timeout() <= 0) && (buttonReconst->areEnabled().any() || tehaiReconst->isCursorEnabled() || buttonReconst->isCursorEnabled())) { /* タイムアウトの処理 */
		const bool isNakiSel = (buttonReconst->getButtonSet() == ButtonReconst::btnSetNormal) && buttonReconst->areEnabled().any();
		if (isNakiSel) { // 鳴き選択中の時
			ui::UIEvent->set(naki::nakiNone); // 牌の番号を設定
		} else if (buttonReconst->getButtonSet() == ButtonReconst::btnSetTsumo) {
			if (GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].Tsumohai().tile != NoTile)
				ui::UIEvent->set(NumOfTilesInHand - 1); // ツモ切り
			else ui::UIEvent->set(0); // 鳴いた直後の場合
		}
		tehaiReconst->setTileCursor();
		buttonReconst->setCursor();
		tileTipReconst->reconstruct();
	}
}

void GameTableScreen::Render() {
	cls();
	RenderTable();
	subSceneCS.trySyncDo<void>(nullptr, [this]() -> void {
#ifndef _WIN32
		if (reconstructFlag) {
			tileTipReconst->reconstruct();
			reconstructFlag = false;
		}
#endif /*_WIN32*/
		checkTimeout();
		mySubScene->Render();
	});
	RenderSideBar();
}

void GameTableScreen::SetSubscene(unsigned int scene_ID) {
	subSceneCS.syncDo<void>([this, scene_ID]() -> void {
		buttonReconst->ChangeButtonSet(GameTableScreen::ButtonReconst::btnSetNormal);
		tehaiReconst->enable();
		tileSelectMode = 0;
		delete mySubScene; tehaiReconst->setTileCursor();
		tileTipReconst->reconstruct();
#ifndef _WIN32
		reconstructFlag = true;
#endif /*_WIN32*/
		ui::UIEvent->reset(); ui::cancellableWait->reset();
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
		case tblSubsceneAlice:
			mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("アリス判定"));
			break;
		case tblSubsceneCall:
			mySubScene = new TableSubsceneCall(caller->getDevice());
			break;
		case tblSubsceneCallFade:
			mySubScene = new TableSubsceneCallFade(caller->getDevice());
			break;
		case tblSubsceneCallCut:
			mySubScene = new TableSubsceneCallCut(caller->getDevice());
			break;
		case tblSubsceneCallChankanPre:
			mySubScene = new TableSubsceneCallChankanPre(caller->getDevice());
			break;
		case tblSubsceneCallChankan:
			mySubScene = new TableSubsceneCallChankanRon(caller->getDevice());
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
			mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("幺九振切"));
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
			(void)GameStatus::retrGameStat();
			// カーソルとボタンの設定
			tehaiReconst->setTileCursor(NumOfTilesInHand - 1);
			while (GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].Hand[tehaiReconst->getTileCursor()].tile == NoTile)
				tehaiReconst->decrTileCursor(); // 鳴いた直後の時のカーソル初期位置
			buttonReconst->btnSetForDahai();
			tehaiReconst->enable();
			if (GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].RichiFlag.RichiFlag)
				for (int i = 0; i < (NumOfTilesInHand - 1); ++i)
					tehaiReconst->disable(i);
			tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
			// リーチ後オートツモ切り
			if ((GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].RichiFlag.RichiFlag) &&
				buttonReconst->areEnabled().none())
				ui::UIEvent->set(NumOfTilesInHand - 1);
			else if (checkBoxes[ChkBoxAutoDiscard]->isChecked() && // 自動ツモ切り
				buttonReconst->areEnabled().none())
				ui::UIEvent->set(NumOfTilesInHand - 1);
			else if ((buttonReconst->isEnabled(buttonReconst->btnTsumo)) &&
				(checkBoxes[ChkBoxAutoAgari]->isChecked())) // オート和了
				CallTsumoAgari();
			else // 自摸番が来たら音を鳴らす
				sound::Play(sound::IDs::sndBell);
			tileTipReconst->reconstruct();
			break;
		case tblSubscenePlayerNaki:
			mySubScene = new TableSubscenePlayerNaki(caller->getDevice());
			goto setNakiButton;
		case tblSubscenePlayerChankan:
			mySubScene = new TableSubscenePlayerNakiChankan(caller->getDevice());
			goto setNakiButton;
		setNakiButton:
			// カーソルとボタンの設定
			buttonReconst->btnSetForNaki();
			buttonReconst->setCursor(buttonReconst->isEnabled(GameTableScreen::ButtonReconst::btnRon) ? GameTableScreen::ButtonReconst::btnRon : GameTableScreen::ButtonReconst::btnPass);
			buttonReconst->reconstruct();
			if (buttonReconst->areEnabled().none()) // 該当する牌がないならスルー
				ui::UIEvent->set(naki::nakiNone);
			else if ((!(buttonReconst->isEnabled(buttonReconst->btnRon))) && /* ButtonReconst::btnRon だと何故かエラーになる */
				(checkBoxes[ChkBoxAutoPass]->isChecked())) // オートパス
				ui::UIEvent->set(naki::nakiNone);
			else if ((buttonReconst->isEnabled(buttonReconst->btnRon)) &&
				(checkBoxes[ChkBoxAutoAgari]->isChecked())) // オート和了
				ui::UIEvent->set(naki::nakiRon);
			else // 音を鳴らす
				sound::Play(sound::IDs::sndSignal);
			tileTipReconst->reconstruct();
			break;
		case tblSubsceneAgari:
			mySubScene = new TableSubsceneAgariScreen(caller->getDevice());
			break;
		case tblSubsceneAgariUradora:
			mySubScene = new TableSubsceneAgariScreenUradora(caller->getDevice());
			break;
		default:
			mySubScene = new TableSubsceneNormal(caller->getDevice());
			break;
		}
	});
}

// -------------------------------------------------------------------------

#ifdef _WIN32
void GameTableScreen::IMEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	if (chatInput->is_Active())
		chatInput->IMEvent(message, wParam, lParam);
}
void GameTableScreen::KeyboardInput(WPARAM wParam, LPARAM lParam)
#else /*_WIN32*/
/* TODO: Linuxでは日本語入力が未実装 */
void GameTableScreen::KeyboardInput(const XEvent* od)
#endif /*_WIN32*/
{
	if (chatInput->is_Active()) {
#ifdef _WIN32
		if (wParam == CHARDAT_CURSOR_ENTER)
#else /*_WIN32*/
		if (od->type != KeyPress) return;
		if (od->xkey.keycode == DIK_RETURN)
#endif /*_WIN32*/
		{
			sound::Play(sound::IDs::sndClick);
			utils::sendchat(chatInput->getText().c_str());
			chatInput->setText(_T(""));
		}
#ifdef _WIN32
		else if ((wParam == CHARDAT_CURSOR_ESCAPE) || (wParam == '\t'))
#else /*_WIN32*/
		else if ((od->xkey.keycode == DIK_ESCAPE) || (od->xkey.keycode == DIK_TAB))
#endif /*_WIN32*/
		{
			sound::Play(sound::IDs::sndClick);
			chatInput->deactivate();
		} else {
#ifdef _WIN32
			chatInput->KeyboardInput(wParam, lParam);
#else /*_WIN32*/
			chatInput->KeyboardInput(od);
#endif /*_WIN32*/
		}
#ifndef _WIN32
		return;
#endif /*_WIN32*/
	} else {
#ifdef _WIN32
		if ((wParam == '\t') && (!(utils::isStandAlone())))
#else /*_WIN32*/
		if ((od->type == KeyPress) && (od->xkey.keycode == DIK_TAB) && (!(utils::isStandAlone())))
#endif /*_WIN32*/
		{
			sound::Play(sound::IDs::sndClick);
			chatInput->activate();
		}
	}
#ifdef _WIN32
}

void GameTableScreen::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
	const bool keyDown = od->dwData;
#else /*_WIN32*/
	const bool keyDown = od->type == KeyPress;
#endif /*_WIN32*/
	if (chatInput->is_Active()) return; // 入力中は無視

	const bool isNakiSel = (buttonReconst->getButtonSet() == ButtonReconst::btnSetNormal) && buttonReconst->areEnabled().any();
	auto cursorMoved = [&]() -> void {
		sound::Play(sound::IDs::sndCursor);
		tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
		buttonReconst->reconstruct();
		tileTipReconst->reconstruct();
	};
	const PlayerTable* const plDat = &(GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID]);
	auto directTileCursor = [&](int cursorPos) -> void {
		if (keyDown && ((tehaiReconst->isCursorEnabled()) || (buttonReconst->isCursorEnabled())) && (plDat->Hand[cursorPos].tile != NoTile)) {
			if (tehaiReconst->getTileCursor() == cursorPos) {
				FinishTileChoice();
			} else {
				buttonReconst->setCursor();
				tehaiReconst->setTileCursor(cursorPos);
				cursorMoved();
			}
		}
	};
#ifdef _WIN32
	switch (od->dwOfs)
#else /*_WIN32*/
	switch (od->xkey.keycode)
#endif /*_WIN32*/
	{
	/* ボタン選択/牌選択 モード切り替え */
	case DIK_UP: case DIK_K: // 牌選択モードに切り替え
		if (keyDown && (buttonReconst->isCursorEnabled()) && (!isNakiSel)) {
			tehaiReconst->setTileCursor(NumOfTilesInHand - 1);
			buttonReconst->setCursor();
			cursorMoved();
		}
		break;
	case DIK_DOWN: case DIK_J: // ボタン選択モードに切り替え
		if (keyDown && (tehaiReconst->isCursorEnabled())) {
			tehaiReconst->setTileCursor();
			buttonReconst->setCursor(ButtonReconst::btnMAXIMUM - 1);
			cursorMoved();
		}
		break;
	/* カーソル移動 */
	case DIK_LEFT: case DIK_H:
		if (keyDown && (tehaiReconst->isCursorEnabled())) {
			do {
				if (tehaiReconst->decrTileCursor() < 0) tehaiReconst->setTileCursor(NumOfTilesInHand - 1);
			} while (plDat->Hand[tehaiReconst->getTileCursor()].tile == NoTile);
			cursorMoved();
		}
		else if (keyDown && (buttonReconst->isCursorEnabled())) {
			if (buttonReconst->decCursor() < 0) buttonReconst->setCursor(ButtonReconst::btnMAXIMUM - 1);
			cursorMoved();
		}
		break;
	case DIK_RIGHT: case DIK_L:
		if (keyDown && (tehaiReconst->isCursorEnabled())) {
			do {
				if (tehaiReconst->incrTileCursor() >= NumOfTilesInHand) tehaiReconst->setTileCursor(0);
			} while (plDat->Hand[tehaiReconst->getTileCursor()].tile == NoTile);
			cursorMoved();
		}
		if (keyDown && (buttonReconst->isCursorEnabled())) {
			if (buttonReconst->incCursor() >= ButtonReconst::btnMAXIMUM) buttonReconst->setCursor(0);
			cursorMoved();
		}
		break;
	/* カーソル位置の直截指定 */
	/* ASSUMING JAPANESE KEYBOARD: 1 2 3 4 5 6 7 8 9 0 - ^ ￥ BS */
	case DIK_1:          directTileCursor( 0); break;
	case DIK_2:          directTileCursor( 1); break;
	case DIK_3:          directTileCursor( 2); break;
	case DIK_4:          directTileCursor( 3); break;
	case DIK_5:          directTileCursor( 4); break;
	case DIK_6:          directTileCursor( 5); break;
	case DIK_7:          directTileCursor( 6); break;
	case DIK_8:          directTileCursor( 7); break;
	case DIK_9:          directTileCursor( 8); break;
	case DIK_0:          directTileCursor( 9); break;
	case DIK_MINUS:      directTileCursor(10); break;
	case DIK_CIRCUMFLEX: directTileCursor(11); break;
	case DIK_YEN:        directTileCursor(12); break;
	case DIK_BACK:       directTileCursor(13); break;
	/* 決定キー */
	case DIK_RETURN: case DIK_SPACE: case DIK_Z:
		if (keyDown)
			subSceneCS.trySyncDo<void>(nullptr, [this]() -> void {
				mySubScene->skipEvent();
			});
		if (keyDown && (tehaiReconst->isCursorEnabled()))
			FinishTileChoice();
		else if (keyDown && (buttonReconst->isCursorEnabled()))
			buttonReconst->ButtonPressed();
		break;
	/* キャンセルキー */
	case DIK_ESCAPE: case DIK_X:
		if (keyDown && isNakiSel && (buttonReconst->isCursorEnabled())) {
			buttonReconst->setCursor(ButtonReconst::btnPass);
			buttonReconst->ButtonPressed();
		}
		break;
	}
}

#ifdef _WIN32
void GameTableScreen::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y)
#else /*_WIN32*/
void GameTableScreen::MouseInput(const XEvent* od, int X, int Y)
#endif /*_WIN32*/
{
	TableProtoScene::MouseInput(od, X, Y);
	const bool isNakiSel = (buttonReconst->getButtonSet() == ButtonReconst::btnSetNormal) && buttonReconst->areEnabled().any();
	const int scaledX = (int)((float)X / Geometry::WindowScale());
	const int scaledY = (int)((float)Y / Geometry::WindowScale());
	const int region = whichRegion(scaledX, scaledY);
	const bool isCursorEnabled = tehaiReconst->isCursorEnabled() || buttonReconst->isCursorEnabled();
	const bool isValidTile = (region >= 0) && (region < NumOfTilesInHand) &&
		isCursorEnabled && (!isNakiSel) &&
		(GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].Hand[region].tile != NoTile);
	const bool isButton = (region >= ButtonReconst::ButtonRegionNum) &&
		(region < ButtonReconst::ButtonRegionNum + ButtonReconst::btnMAXIMUM) &&
		isCursorEnabled;
#ifdef _WIN32
	switch (od->dwOfs)
#else /*_WIN32*/
	switch (od->type)
#endif /*_WIN32*/
	{
#ifdef _WIN32
	case DIMOFS_X: case DIMOFS_Y: // マウスカーソルを動かした場合
#else /*_WIN32*/
	case MotionNotify: // マウスカーソルを動かした場合
#endif /*_WIN32*/
		if ((!chatInput->is_Active()) && (region != tehaiReconst->getTileCursor()) && (isValidTile)) {
			tehaiReconst->setTileCursor(region);
			buttonReconst->setCursor();
			sound::Play(sound::IDs::sndCursor);
			tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
			buttonReconst->reconstruct();
			tileTipReconst->reconstruct();
		} else if ((!chatInput->is_Active()) && (region != (ButtonReconst::ButtonRegionNum + buttonReconst->getCursor())) && (isButton)) {
			tehaiReconst->setTileCursor();
			buttonReconst->setCursor(region - ButtonReconst::ButtonRegionNum);
			sound::Play(sound::IDs::sndCursor);
			tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
			buttonReconst->reconstruct();
			tileTipReconst->reconstruct();
		}
		break;
#ifdef _WIN32
	case DIMOFS_BUTTON0: // マウスクリック
		if (od->dwData)
#else /*_WIN32*/
	case ButtonPress: // マウスクリック
		if (od->xbutton.button == Button1) {
#endif /*_WIN32*/
			subSceneCS.trySyncDo<void>(nullptr, [this]() -> void {
				mySubScene->skipEvent();
			});
		if ((!chatInput->is_Active()) && (isValidTile)
#ifdef _WIN32
			&& (od->dwData)
#endif /*_WIN32*/
		)
			FinishTileChoice();
		else if ((!chatInput->is_Active()) && (isButton)
#ifdef _WIN32
			&& (od->dwData)
#endif /*_WIN32*/
		)
			buttonReconst->ButtonPressed();
		else if ((!chatInput->is_Active()) && (region == ChatInputRegion)
#ifdef _WIN32
			&& (od->dwData)
#endif /*_WIN32*/
			&& (!(utils::isStandAlone()))
		) {
			sound::Play(sound::IDs::sndClick);
			chatInput->activate();
		}
		else if ((chatInput->is_Active()) && (region != ChatInputRegion)
#ifdef _WIN32
			&& (od->dwData)
#endif /*_WIN32*/
		) {
			sound::Play(sound::IDs::sndClick);
			chatInput->deactivate();
		}
#ifdef _WIN32
		break;
	case DIMOFS_BUTTON1: // マウス右クリック
#else /*_WIN32*/
		} else if (od->xbutton.button == Button3) {
#endif /*_WIN32*/
		if ((!chatInput->is_Active())
#ifdef _WIN32
			&& (od->dwData)
#endif /*_WIN32*/
			&& isNakiSel
		) { // 鳴き選択中の時
			sound::Play(sound::IDs::sndClick);
			ui::UIEvent->set(naki::nakiNone); // 牌の番号を設定
		}
#ifndef _WIN32
		}
#endif /*_WIN32*/
		break;
	}
}

/* 捨牌を決定する */
void GameTableScreen::FinishTileChoice() {
	sound::Play(sound::IDs::sndClick);
	if (tehaiReconst->isCursorEnabled() && tehaiReconst->isEnabled(tehaiReconst->getTileCursor())) {
		const Int8ByTile TileCount = utils::countTilesInHand(GameStatus::gameStat(), GameStatus::gameStat()->CurrentPlayer.Active);
		if ((tileSelectMode == DiscardTileNum::Ankan) && (TileCount[GameStatus::gameStat()->statOfActive().Hand[tehaiReconst->getTileCursor()].tile] == 1))
			ui::UIEvent->set((unsigned)tehaiReconst->getTileCursor() + (unsigned)(DiscardTileNum::Kakan * DiscardTileNum::TypeStep)); // 加槓の場合
		else
			ui::UIEvent->set((unsigned)tehaiReconst->getTileCursor() + (unsigned)(tileSelectMode * DiscardTileNum::TypeStep)); // 牌の番号を設定
	} else {
		sound::Play(sound::IDs::sndCuohu);
	}
}

void GameTableScreen::CallTsumoAgari() { // ツモアガリ
	ui::UIEvent->set(0xffffffff);
}
void GameTableScreen::CallKyuushuKyuuhai() { // 九種九牌
	ui::UIEvent->set(0xfffffffe);
}


}
