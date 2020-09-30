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
#include <unordered_set>
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

#ifdef min
#undef min
#endif

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
	const unsigned logWidth = static_cast<unsigned>(floor(0.5f + // VC++2010ではround()が使えない
		static_cast<float>((static_cast<signed>(Geometry::WindowWidth) - static_cast<signed>(Geometry::WindowHeight)) / Geometry::WindowScale() - 36)) / 9u);
	mySubScene = new TableSubsceneNormal(manipulator->getDevice());
	myTextRenderer = new TextRenderer(manipulator->getDevice());
	tileSelectMode = 0;
#ifndef _WIN32
	reconstructFlag = true;
#endif /*_WIN32*/
}

GameTableScreen::~GameTableScreen() {
	delete mySubScene;
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
		constexpr PlayerID tobePlayed[4][4] = {
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
	constexpr wchar_t WindName[] = L"東南西北白發中";
	constexpr wchar_t Numeral[] = L"一二三四五六七八九十";
	constexpr wchar_t FWDigit[] = L"０１２３４５６７８９";
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
		static_cast<double>((roundColor() & 0x00ff0000) >> 16) / 255.0,
		static_cast<double>((roundColor() & 0x0000ff00) >>  8) / 255.0,
		static_cast<double>((roundColor() & 0x000000ff)      ) / 255.0,
		static_cast<double>((roundColor() & 0xff000000) >> 24) / 255.0);
	glClear(GL_COLOR_BUFFER_BIT);
#endif
}

void GameTableScreen::RenderTable() {
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tBaize, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tBorder, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	Reconstruct(GameStatus::retrGameStat());
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
	clockPanel->Render();
}

void GameTableScreen::checkTimeout() {
	if ((mySubScene) && (mySubScene->timeout() <= 0) && (buttonReconst->areEnabled().any() || tehaiReconst->isCursorEnabled() || buttonReconst->isCursorEnabled())) { /* タイムアウトの処理 */
		const bool isNakiSel = (buttonReconst->getButtonSet() == ButtonReconst::btnSetNormal) && buttonReconst->areEnabled().any();
		tehaiReconst->setFirstChosenTile(); // 鳴き選択を取り消し
		if (isNakiSel) { // 鳴き選択中の時
			ui::UIEvent->set(naki::nakiNone); // 牌の番号を設定
		} else if (buttonReconst->getButtonSet() == ButtonReconst::btnSetTsumo) {
			if (GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].Tsumohai())
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
	std::unique_lock<std::recursive_mutex> lock(subSceneCS, std::try_to_lock);
	if (lock.owns_lock()) {
#ifndef _WIN32
		if (reconstructFlag) {
			tileTipReconst->reconstruct();
			reconstructFlag = false;
		}
#endif /*_WIN32*/
		checkTimeout();
		mySubScene->Render();

		lock.unlock();
	}
	RenderSideBar();
	if (utils::isWatchMode()) {
		const CodeConv::tstring DemoMsg(_T("ＤＥＭＯＮＳＴＲＡＴＩＯＮ"));
		const auto alpha = static_cast<uint32_t>(
			std::min(255.0,
				pow(sin(static_cast<double>(myTimer.elapsed()) / 159154.943) / 2.0 + 0.5, 1.5) * 224.0 + 32.0)
			);
		myTextRenderer->NewText(255,
			DemoMsg,
			Geometry::BaseSize / 2 - myTextRenderer->strWidthByPix(DemoMsg),
			Geometry::BaseSize / 2,
			2.0f, 1.0f, (alpha << 24) | 0x00ffffff);
		myTextRenderer->Render();
	}
}

void GameTableScreen::SetSubscene(SubSceneID scene_ID) {
	std::unique_lock<std::recursive_mutex> lock(subSceneCS);
	buttonReconst->ChangeButtonSet(GameTableScreen::ButtonReconst::btnSetNormal);
	tehaiReconst->enable();
	tileSelectMode = 0;
	delete mySubScene; tehaiReconst->setTileCursor();
	tileTipReconst->reconstruct();
#ifndef _WIN32
	reconstructFlag = true;
#endif /*_WIN32*/
	ui::UIEvent->reset(); ui::cancellableWait->reset();
	switch (scene_ID.tableSubsceneID) {
	case TableSubsceneID::beginning:
		mySubScene = new TableSubsceneBeginning(caller->getDevice());
		break;
	case TableSubsceneID::honba:
		mySubScene = new TableSubsceneTitlingHonba(caller->getDevice());
		break;
	case TableSubsceneID::ryuukyoku:
		mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("流局"));
		break;
	case TableSubsceneID::sifeng:
		mySubScene = new TableSubsceneMsg(caller->getDevice(),
			(GameStatus::gameStat()->gameType & AllSanma) ? _T("三風連打") : _T("四風連打"));
		break;
	case TableSubsceneID::tripleRon:
		mySubScene = new TableSubsceneMsg(caller->getDevice(),
			(GameStatus::gameStat()->gameType & AllSanma) ? _T("二家和") : _T("三家和"));
		break;
	case TableSubsceneID::sikang:
		mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("四開槓"));
		break;
	case TableSubsceneID::fourRiichi:
		mySubScene = new TableSubsceneMsg(caller->getDevice(),
			(GameStatus::gameStat()->gameType & AllSanma) ? _T("三家立直") : _T("四家立直"));
		break;
	case TableSubsceneID::chonbo:
		mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("錯和"));
		break;
	case TableSubsceneID::alice:
		mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("アリス判定"));
		break;
	case TableSubsceneID::call:
		mySubScene = new TableSubsceneCall(caller->getDevice());
		break;
	case TableSubsceneID::callFade:
		mySubScene = new TableSubsceneCallFade(caller->getDevice());
		break;
	case TableSubsceneID::callCut:
		mySubScene = new TableSubsceneCallCut(caller->getDevice());
		break;
	case TableSubsceneID::callChankanPre:
		mySubScene = new TableSubsceneCallChankanPre(caller->getDevice());
		break;
	case TableSubsceneID::callChankan:
		mySubScene = new TableSubsceneCallChankanRon(caller->getDevice());
		break;
	case TableSubsceneID::callVal:
		mySubScene = new TableSubsceneCallValue(caller->getDevice());
		break;
	case TableSubsceneID::callValNotenBappu:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("不聴罰符"));
		break;
	case TableSubsceneID::callValAgariten:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("和了点"));
		break;
	case TableSubsceneID::callValTsumibou:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("積棒精算"));
		break;
	case TableSubsceneID::callValChip:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("祝儀精算"));
		break;
	case TableSubsceneID::callValKyoutaku:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("供託精算"));
		break;
	case TableSubsceneID::callValChonboBappu:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("錯和罰符"));
		break;
	case TableSubsceneID::callValNagashiMangan:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("幺九振切"));
		break;
	case TableSubsceneID::callValDobon:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("飛び罰符"));
		break;
	case TableSubsceneID::callValKitamakura:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("北枕罰符"));
		break;
	case TableSubsceneID::callValYakuman:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("役満祝儀"));
		break;
	case TableSubsceneID::chkTenpai:
		mySubScene = new TableSubsceneCheckTenpai(caller->getDevice());
		break;
	case TableSubsceneID::playerDahai:
		mySubScene = new TableSubscenePlayerDahai(caller->getDevice());
		(void)GameStatus::retrGameStat();
		// カーソルとボタンの設定
		tehaiReconst->setTileCursor(NumOfTilesInHand - 1);
		while (!GameStatus::gameStat()->statOfMine().Hand[tehaiReconst->getTileCursor()])
			tehaiReconst->decrTileCursor(); // 鳴いた直後の時のカーソル初期位置
		buttonReconst->btnSetForDahai();
		tehaiReconst->enable();
		if (GameStatus::gameStat()->statOfMine().RichiFlag.RichiFlag)
			for (int i = 0; i < (NumOfTilesInHand - 1); ++i)
				tehaiReconst->disable(i);
		tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
		// リーチ後オートツモ切り
		if ((GameStatus::gameStat()->statOfMine().RichiFlag.RichiFlag) &&
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
	case TableSubsceneID::playerNaki:
		mySubScene = new TableSubscenePlayerNaki(caller->getDevice());
		goto setNakiButton;
	case TableSubsceneID::playerChankan:
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
	case TableSubsceneID::agari:
		mySubScene = new TableSubsceneAgariScreen(caller->getDevice());
		break;
	case TableSubsceneID::agariUradora:
		mySubScene = new TableSubsceneAgariScreenUradora(caller->getDevice());
		break;
	default:
		mySubScene = new TableSubsceneNormal(caller->getDevice());
		break;
	}
}

// -------------------------------------------------------------------------

#ifdef _WIN32
void GameTableScreen::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
	const bool keyDown = od->dwData;
#else /*_WIN32*/
void GameTableScreen::KeyboardInput(const XEvent* od) {
	const bool keyDown = od->type == KeyPress;
#endif /*_WIN32*/

	const bool isNakiSel = (buttonReconst->getButtonSet() == ButtonReconst::btnSetNormal) && buttonReconst->areEnabled().any();
	auto cursorMoved = [&]() -> void {
		sound::Play(sound::IDs::sndCursor);
		tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
		buttonReconst->reconstruct();
		tileTipReconst->reconstruct();
	};
	const PlayerTable* const plDat = &(GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID]);
	auto directTileCursor = [&](int cursorPos) -> void {
		if (keyDown && ((tehaiReconst->isCursorEnabled()) || (buttonReconst->isCursorEnabled())) && (plDat->Hand[cursorPos])) {
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
	case DIK_UP: case DIK_K: case DIK_W: // 牌選択モードに切り替え
		if (keyDown && (buttonReconst->isCursorEnabled()) && ((!isNakiSel) || (tileSelectMode == DiscardTileNum::MeldSel))) {
			tehaiReconst->setTileCursor(NumOfTilesInHand - 1);
			buttonReconst->setCursor();
			cursorMoved();
		}
		break;
	case DIK_DOWN: case DIK_J: case DIK_S: // ボタン選択モードに切り替え
		if (keyDown && (tehaiReconst->isCursorEnabled())) {
			tehaiReconst->setTileCursor();
			buttonReconst->setCursor(ButtonReconst::btnMAXIMUM - 1);
			cursorMoved();
		}
		break;
	/* カーソル移動 */
	case DIK_LEFT: case DIK_H: case DIK_A:
		if (keyDown && (tehaiReconst->isCursorEnabled())) {
			do {
				if (tehaiReconst->decrTileCursor() < 0) tehaiReconst->setTileCursor(NumOfTilesInHand - 1);
			} while (!plDat->Hand[tehaiReconst->getTileCursor()]);
			cursorMoved();
		}
		else if (keyDown && (buttonReconst->isCursorEnabled())) {
			if (buttonReconst->decCursor() < 0) buttonReconst->setCursor(ButtonReconst::btnMAXIMUM - 1);
			cursorMoved();
		}
		break;
	case DIK_RIGHT: case DIK_L: case DIK_D:
		if (keyDown && (tehaiReconst->isCursorEnabled())) {
			do {
				if (tehaiReconst->incrTileCursor() >= NumOfTilesInHand) tehaiReconst->setTileCursor(0);
			} while (!plDat->Hand[tehaiReconst->getTileCursor()]);
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
		if (keyDown) {
			if (utils::isWatchMode()) {
				ui::UIEvent->set(0);
				ui::cancellableWait->set(0);
				ui::clickEvent->set();
			}
			std::unique_lock<std::recursive_mutex> lock(subSceneCS, std::try_to_lock);
			if (lock.owns_lock())
				mySubScene->skipEvent();
		}
		if (keyDown && (tehaiReconst->isCursorEnabled()))
			FinishTileChoice();
		else if (keyDown && (buttonReconst->isCursorEnabled()))
			buttonReconst->ButtonPressed();
		break;
	/* キャンセルキー */
	case DIK_ESCAPE: case DIK_X:
		if (keyDown && isNakiSel && (buttonReconst->isCursorEnabled())) {
			tehaiReconst->setFirstChosenTile(); // 鳴き選択を取り消し
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
	const int scaledX = static_cast<int>(static_cast<float>(X) / Geometry::WindowScale());
	const int scaledY = static_cast<int>(static_cast<float>(Y) / Geometry::WindowScale());
	const int region = whichRegion(scaledX, scaledY);
	const bool isCursorEnabled = tehaiReconst->isCursorEnabled() || buttonReconst->isCursorEnabled();
	const bool isValidTile = (region >= 0) && (region < NumOfTilesInHand) &&
		isCursorEnabled && ((!isNakiSel) || (tileSelectMode == DiscardTileNum::MeldSel)) &&
		(GameStatus::gameStat()->statOfMine().Hand[region]);
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
		if ((region != tehaiReconst->getTileCursor()) && (isValidTile)) {
			tehaiReconst->setTileCursor(region);
			buttonReconst->setCursor();
			sound::Play(sound::IDs::sndCursor);
			tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
			buttonReconst->reconstruct();
			tileTipReconst->reconstruct();
		} else if ((region != (ButtonReconst::ButtonRegionNum + buttonReconst->getCursor())) && (isButton)) {
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
			{
				if (utils::isWatchMode()) {
					ui::UIEvent->set(0);
					ui::cancellableWait->set(0);
					ui::clickEvent->set();
				}
				std::unique_lock<std::recursive_mutex> lock(subSceneCS, std::try_to_lock);
				if (lock.owns_lock())
					mySubScene->skipEvent();
			}
		if ((isValidTile)
#ifdef _WIN32
			&& (od->dwData)
#endif /*_WIN32*/
		)
			FinishTileChoice();
		else if ((isButton)
#ifdef _WIN32
			&& (od->dwData)
#endif /*_WIN32*/
		)
			buttonReconst->ButtonPressed();
#ifdef _WIN32
		break;
	case DIMOFS_BUTTON1: // マウス右クリック
#else /*_WIN32*/
		} else if (od->xbutton.button == Button3) {
#endif /*_WIN32*/
		if (
#ifdef _WIN32
			(od->dwData) &&
#endif /*_WIN32*/
			isNakiSel
		) { // 鳴き選択中の時
			sound::Play(sound::IDs::sndClick);
			tehaiReconst->setFirstChosenTile(); // 鳴き選択を取り消し
			ui::UIEvent->set(naki::nakiNone); // 牌の番号を設定
		}
#ifndef _WIN32
		}
#endif /*_WIN32*/
		break;
	}
}

/* 牌を数える */
int GameTableScreen::countTiles(std::function<bool(TileCode, TileCode)> f) {
	std::unordered_set<Tile> appliedTile;
	for (int i = 0; i < TsumohaiIndex; ++i)
		if (i != tehaiReconst->getFirstChosenTile())
			if (GameStatus::gameStat()->statOfMine().Hand[i])
				if (f(GameStatus::gameStat()->statOfMine().Hand[i].tile, GameStatus::gameStat()->CurrentDiscard.tile))
					appliedTile.insert(GameStatus::gameStat()->statOfMine().Hand[i]);
	return appliedTile.size();
}

/* 捨牌を決定する */
void GameTableScreen::FinishTileChoice() {
	sound::Play(sound::IDs::sndClick);
	if (tehaiReconst->isCursorEnabled() && tehaiReconst->isEnabled(tehaiReconst->getTileCursor())) {
		const Int8ByTile TileCount = utils::countTilesInHand(GameStatus::gameStat(), GameStatus::gameStat()->CurrentPlayer.Active);
		const Tile chosenTile = GameStatus::gameStat()->statOfMine().Hand[tehaiReconst->getTileCursor()];
		if ((tileSelectMode == DiscardTileNum::Ankan) && (TileCount[chosenTile.tile] == 1)) {
			ui::UIEvent->set(static_cast<unsigned>(tehaiReconst->getTileCursor()) +
				static_cast<unsigned>(DiscardTileNum::Kakan * DiscardTileNum::TypeStep)); // 加槓の場合
		} else if (tileSelectMode == DiscardTileNum::MeldSel) {
			if (tehaiReconst->getFirstChosenTile() < 0) {
				if (chosenTile.tile == GameStatus::gameStat()->CurrentDiscard.tile) { // ポン選択
					if (countTiles([](TileCode p, TileCode q) {return p == q;}) > 1) {
						tehaiReconst->setFirstChosenTile(tehaiReconst->getTileCursor());
						buttonReconst->setMode(DiscardTileNum::MeldSel, ButtonReconst::btnPon,
							[](int i, GameTable* tmpStat) -> bool {
							return tmpStat->statOfMine().Hand[i].tile != tmpStat->CurrentDiscard.tile;
						});
						tehaiReconst->Render();
						return;
					}
				} else { // チー選択
					const auto validTile = [this](TileCode p, TileCode q) {
						if (Tile(p).getSuit() != Tile(q).getSuit())
							return false;
						switch (static_cast<int>(GameStatus::gameStat()->statOfMine().Hand[tehaiReconst->getTileCursor()].tile) - static_cast<int>(q)) {
						case -2:
							return p == q - 1;
						case -1:
							return (p == q + 1) || (p == q - 2);
						case 1:
							return (p == q - 1) || (p == q + 2);
						case 2:
							return p == q + 1;
						default:
							return false;
						}
					};
					if (countTiles(validTile) > 1) {
						tehaiReconst->setFirstChosenTile(tehaiReconst->getTileCursor());
						buttonReconst->setMode(DiscardTileNum::MeldSel, ButtonReconst::btnChii,
							[validTile](int i, GameTable* tmpStat) -> bool {
								return !validTile(tmpStat->statOfMine().Hand[i].tile, tmpStat->CurrentDiscard.tile);
							});
						tehaiReconst->Render();
						return;
					}
				}
			}
			for (int i = 0; i < TsumohaiIndex; ++i)
				if ((i == tehaiReconst->getFirstChosenTile()) || (i == tehaiReconst->getTileCursor()))
					utils::moveTile(GameStatus::gameStat()->PlayerID, i, 0);
			if (chosenTile.tile == GameStatus::gameStat()->CurrentDiscard.tile) {
				tehaiReconst->setFirstChosenTile();
				ui::UIEvent->set(naki::nakiPon); // ポンの場合
			} else { // チーの場合
				const auto selected = [this](TileCode tCode) {
					if (GameStatus::gameStat()->statOfMine().Hand[tehaiReconst->getTileCursor()].tile == tCode) {
						return true;
					} else if (tehaiReconst->getFirstChosenTile() >= 0) {
						if (GameStatus::gameStat()->statOfMine().Hand[tehaiReconst->getFirstChosenTile()].tile == tCode) {
							return true;
						}
					}
					return false;
				};
				if (selected(static_cast<TileCode>(GameStatus::gameStat()->CurrentDiscard.tile - 2))) {
					tehaiReconst->setFirstChosenTile();
					ui::UIEvent->set(naki::nakiChiUpper);
				} else if (selected(static_cast<TileCode>(GameStatus::gameStat()->CurrentDiscard.tile + 2))) {
					tehaiReconst->setFirstChosenTile();
					ui::UIEvent->set(naki::nakiChiLower);
				} else {
					tehaiReconst->setFirstChosenTile();
					ui::UIEvent->set(naki::nakiChiMiddle);
				}
			}
		} else {
			ui::UIEvent->set(static_cast<unsigned>(tehaiReconst->getTileCursor()) +
				static_cast<unsigned>(tileSelectMode * DiscardTileNum::TypeStep)); // 牌の番号を設定
		}
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
