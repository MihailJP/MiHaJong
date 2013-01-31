#include "table.h"
#include "../resource.h"
#include "../rule.h"
#include "../sprite.h"
#include <iomanip>
#include "../../sound/sound.h"
#include "../../mjcore/bgmid.h"
#include "../event.h"

#include "table/yamahai.h"
#include "table/tehai.h"
#include "table/nakihai.h"
#include "table/sutehai.h"
#include "table/showdice.h"
#include "table/richibou.h"
#include "table/chicha.h"
#include "table/nakibtn.h"

namespace mihajong_graphic {
	
GameTableScreen::GameTableScreen(ScreenManipulator* const manipulator) : TableProtoScene(manipulator) {
	LoadTexture(&tBorder, MAKEINTRESOURCE(IDB_PNG_TBLBORDER), 1080, 1080);
	LoadTexture(&tBaize, MAKEINTRESOURCE(IDB_PNG_TBLBAIZE), 1080, 1080);
	buttonReconst = new ButtonReconst(this);
	trayReconst = new TrayReconst(this);
	richibouReconst = new RichibouReconst(this);
	diceReconst = new DiceReconst(this);
	yamahaiReconst = new YamahaiReconst(this);
	tehaiReconst = new TehaiReconst(this);
	nakihaiReconst = new NakihaiReconst(this);
	sutehaiReconst = new SutehaiReconst(this);
	Reconstruct(GameStatus::retrGameStat());
	const unsigned logWidth = (unsigned)std::floor(0.5f + // VC++2010Ç≈ÇÕround()Ç™égÇ¶Ç»Ç¢
		(float)(((signed)Geometry::WindowWidth - (signed)Geometry::WindowHeight) / Geometry::WindowScale() - 36)) / 9u;
	logWindow = new logwnd::LogWindow(caller->getHWnd(), caller->getDevice(),
		1100, 100, logWidth, 20);
	InitializeCriticalSection(&subSceneCS);
	mySubScene = new TableSubsceneNormal(manipulator->getDevice());
	myTextRenderer = new TextRenderer(manipulator->getDevice());
}

GameTableScreen::~GameTableScreen() {
	delete mySubScene; DeleteCriticalSection(&subSceneCS);
	delete logWindow;
	delete sutehaiReconst;
	delete nakihaiReconst;
	delete tehaiReconst;
	delete yamahaiReconst;
	delete diceReconst;
	delete richibouReconst;
	delete trayReconst;
	delete buttonReconst;
	delete myTextRenderer;
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

/* ãüëıì_ñ_Ç»Ç«ÇÃèÓïÒÇï\é¶ */
void GameTableScreen::ShowStatus(const GameTable* gameStat) {
	const wchar_t* const WindName = L"ìåìÏêºñkîí·¢íÜ";
	const wchar_t* const Numeral = L"àÍìÒéOélå‹òZéµî™ã„è\";
	const wchar_t* const FWDigit = L"ÇOÇPÇQÇRÇSÇTÇUÇVÇWÇX";
	CodeConv::tostringstream o;
	o << CodeConv::EnsureTStr(std::wstring(WindName + gameStat->GameRound / 4, WindName + gameStat->GameRound / 4 + 1));
	if (rules::chkRule("game_length", "twice_east_game") || rules::chkRule("game_length", "east_only_game")) { // ìåèÍÇÃÇ›ÇÃÉãÅ[Éã
		const unsigned roundnum = gameStat->LoopRound * 4 + gameStat->GameRound;
		if (roundnum < 10)
			o << CodeConv::EnsureTStr(std::wstring(Numeral + roundnum, Numeral + roundnum + 1));
		else
			o << (roundnum + 1);
	} else { // àÍî ÇÃÉãÅ[Éã
		const unsigned roundnum = gameStat->GameRound % 4;
		o << CodeConv::EnsureTStr(std::wstring(Numeral + roundnum, Numeral + roundnum + 1));
	}
	o << _T("ã« ");
	if (gameStat->Honba >= 10) o << std::setw(2) << (gameStat->Honba) << _T("ñ{èÍ");
	else if (gameStat->Honba > 0) o << CodeConv::EnsureTStr(std::wstring(FWDigit + gameStat->Honba, FWDigit + gameStat->Honba + 1)) << _T("ñ{èÍ");
	else o << _T("ïΩÅ@èÍ");
	myTextRenderer->NewText(0, o.str(), Geometry::BaseSize + 5, 5, 0.875f);

	o.str(_T(""));
	const int tiles = gameStat->RinshanPointer - gameStat->TilePointer - gameStat->ExtraRinshan - gameStat->DeadTiles + 1;
	if (tiles >= 100) o << _T("éc--îv");
	else o << _T("éc") << std::setw(2) << tiles << _T("îv");
	o << _T(" ãüëı");
	if (gameStat->Deposit) o << std::setw(2) << gameStat->Deposit << _T("ñ{");
	else o << _T("Ç»Çµ");
	myTextRenderer->NewText(1, o.str(), Geometry::BaseSize + 5, 35, 0.875f);

	myTextRenderer->Render();
}

/* ëÏÇï\é¶ Ç±Ç±Ç©ÇÁ */
void GameTableScreen::cls() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0); // ÉoÉbÉtÉ@ÉNÉäÉA
}

void GameTableScreen::RenderTable() {
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tBaize, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tBorder, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	if (GameStatus::isModified())
		Reconstruct(GameStatus::retrGameStat());
	richibouReconst->Render();
	diceReconst->Render();
	trayReconst->Render();
	yamahaiReconst->Render(); // 0
	tehaiReconst->Render(); // 144
	nakihaiReconst->Render(); // 200
	sutehaiReconst->Render(); // 264
	buttonReconst->Render();
}

void GameTableScreen::RenderSideBar() {
	ShowSidebar();
	ShowStatus(GameStatus::gameStat());
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
		mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("ó¨ã«"));
		break;
	case tblSubsceneSifeng:
		mySubScene = new TableSubsceneMsg(caller->getDevice(),
			(GameStatus::gameStat()->gameType & AllSanma) ? _T("éOïóòAë≈") : _T("élïóòAë≈"));
		break;
	case tblSubsceneTripleRon:
		mySubScene = new TableSubsceneMsg(caller->getDevice(),
			(GameStatus::gameStat()->gameType & AllSanma) ? _T("ìÒâ∆òa") : _T("éOâ∆òa"));
		break;
	case tblSubsceneSikang:
		mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("éläJû»"));
		break;
	case tblSubsceneFourRiichi:
		mySubScene = new TableSubsceneMsg(caller->getDevice(),
			(GameStatus::gameStat()->gameType & AllSanma) ? _T("éOâ∆óßíº") : _T("élâ∆óßíº"));
		break;
	case tblSubsceneChonbo:
		mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("çˆòa"));
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
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("ïsíÆî±ïÑ"));
		break;
	case tblSubsceneCallValAgariten:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("òaóπì_"));
		break;
	case tblSubsceneCallValTsumibou:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("êœñ_ê∏éZ"));
		break;
	case tblSubsceneCallValChip:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("èjãVê∏éZ"));
		break;
	case tblSubsceneCallValKyoutaku:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("ãüëıê∏éZ"));
		break;
	case tblSubsceneCallValChonboBappu:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("çˆòaî±ïÑ"));
		break;
	case tblSubsceneCallValNagashiMangan:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("ó¨Çµñûä—"));
		break;
	case tblSubsceneCallValDobon:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("îÚÇ—î±ïÑ"));
		break;
	case tblSubsceneCallValKitamakura:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("ñkñçî±ïÑ"));
		break;
	case tblSubsceneCallValYakuman:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("ññûèjãV"));
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
	case DIK_LEFT: case DIK_H:
		if ((od->dwData) && (tehaiReconst->isCursorEnabled())) {
			do {
				if (tehaiReconst->decrTileCursor() < 0) tehaiReconst->setTileCursor(NUM_OF_TILES_IN_HAND - 1);
			} while (plDat->Hand[tehaiReconst->getTileCursor()].tile == NoTile);
			cursorMoved();
		}
		break;
	case DIK_RIGHT: case DIK_L:
		if ((od->dwData) && (tehaiReconst->isCursorEnabled())) {
			do {
				if (tehaiReconst->incrTileCursor() >= NUM_OF_TILES_IN_HAND) tehaiReconst->setTileCursor(0);
			} while (plDat->Hand[tehaiReconst->getTileCursor()].tile == NoTile);
			cursorMoved();
		}
		break;
	/* ÉJÅ[É\Éãà íuÇÃíºùBéwíË */
	/* ASSUMING JAPANESE KEYBOARD: 1 2 3 4 5 6 7 8 9 0 - ^ Åè BS */
	case DIK_1:          if ((od->dwData) && (tehaiReconst->isCursorEnabled()) && (plDat->Hand[ 0].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  0) FinishTileChoice(); else {tehaiReconst->setTileCursor( 0); cursorMoved();}} break;
	case DIK_2:          if ((od->dwData) && (tehaiReconst->isCursorEnabled()) && (plDat->Hand[ 1].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  1) FinishTileChoice(); else {tehaiReconst->setTileCursor( 1); cursorMoved();}} break;
	case DIK_3:          if ((od->dwData) && (tehaiReconst->isCursorEnabled()) && (plDat->Hand[ 2].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  2) FinishTileChoice(); else {tehaiReconst->setTileCursor( 2); cursorMoved();}} break;
	case DIK_4:          if ((od->dwData) && (tehaiReconst->isCursorEnabled()) && (plDat->Hand[ 3].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  3) FinishTileChoice(); else {tehaiReconst->setTileCursor( 3); cursorMoved();}} break;
	case DIK_5:          if ((od->dwData) && (tehaiReconst->isCursorEnabled()) && (plDat->Hand[ 4].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  4) FinishTileChoice(); else {tehaiReconst->setTileCursor( 4); cursorMoved();}} break;
	case DIK_6:          if ((od->dwData) && (tehaiReconst->isCursorEnabled()) && (plDat->Hand[ 5].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  5) FinishTileChoice(); else {tehaiReconst->setTileCursor( 5); cursorMoved();}} break;
	case DIK_7:          if ((od->dwData) && (tehaiReconst->isCursorEnabled()) && (plDat->Hand[ 6].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  6) FinishTileChoice(); else {tehaiReconst->setTileCursor( 6); cursorMoved();}} break;
	case DIK_8:          if ((od->dwData) && (tehaiReconst->isCursorEnabled()) && (plDat->Hand[ 7].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  7) FinishTileChoice(); else {tehaiReconst->setTileCursor( 7); cursorMoved();}} break;
	case DIK_9:          if ((od->dwData) && (tehaiReconst->isCursorEnabled()) && (plDat->Hand[ 8].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  8) FinishTileChoice(); else {tehaiReconst->setTileCursor( 8); cursorMoved();}} break;
	case DIK_0:          if ((od->dwData) && (tehaiReconst->isCursorEnabled()) && (plDat->Hand[ 9].tile != NoTile)) {if (tehaiReconst->getTileCursor() ==  9) FinishTileChoice(); else {tehaiReconst->setTileCursor( 9); cursorMoved();}} break;
	case DIK_MINUS:      if ((od->dwData) && (tehaiReconst->isCursorEnabled()) && (plDat->Hand[10].tile != NoTile)) {if (tehaiReconst->getTileCursor() == 10) FinishTileChoice(); else {tehaiReconst->setTileCursor(10); cursorMoved();}} break;
	case DIK_CIRCUMFLEX: if ((od->dwData) && (tehaiReconst->isCursorEnabled()) && (plDat->Hand[11].tile != NoTile)) {if (tehaiReconst->getTileCursor() == 11) FinishTileChoice(); else {tehaiReconst->setTileCursor(11); cursorMoved();}} break;
	case DIK_YEN:        if ((od->dwData) && (tehaiReconst->isCursorEnabled()) && (plDat->Hand[12].tile != NoTile)) {if (tehaiReconst->getTileCursor() == 12) FinishTileChoice(); else {tehaiReconst->setTileCursor(12); cursorMoved();}} break;
	case DIK_BACK:       if ((od->dwData) && (tehaiReconst->isCursorEnabled()) && (plDat->Hand[13].tile != NoTile)) {if (tehaiReconst->getTileCursor() == 13) FinishTileChoice(); else {tehaiReconst->setTileCursor(13); cursorMoved();}} break;
	/* åàíËÉLÅ[ */
	case DIK_RETURN: case DIK_SPACE: case DIK_Z:
		if ((od->dwData) && (tehaiReconst->isCursorEnabled()))
			FinishTileChoice();
		break;
	}
}

void GameTableScreen::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {
	const int scaledX = X / Geometry::WindowScale() * (Geometry::WindowWidth * 0.75f / Geometry::WindowHeight);
	const int scaledY = Y / Geometry::WindowScale();
	const int region = whichRegion(scaledX, scaledY);
	const bool isValidTile = (region >= 0) && (region < NUM_OF_TILES_IN_HAND) &&
		(tehaiReconst->isCursorEnabled()) &&
		(GameStatus::gameStat()->Player.val[GameStatus::gameStat()->PlayerID].Hand[region].tile != NoTile);
	switch (od->dwOfs) {
	case DIMOFS_X: case DIMOFS_Y: // É}ÉEÉXÉJÅ[É\ÉãÇìÆÇ©ÇµÇΩèÍçá
		if ((region != tehaiReconst->getTileCursor()) && (isValidTile)) {
			tehaiReconst->setTileCursor(region);
			sound::Play(sound::IDs::sndCursor);
			tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
		}
		break;
	case DIMOFS_BUTTON0: // É}ÉEÉXÉNÉäÉbÉN
		if ((isValidTile) && (od->dwData))
			FinishTileChoice();
		break;
	}
}

/* éÃîvÇåàíËÇ∑ÇÈ */
void GameTableScreen::FinishTileChoice() {
	ui::UIEvent->set((unsigned)tehaiReconst->getTileCursor()); // îvÇÃî‘çÜÇê›íË
}

}
