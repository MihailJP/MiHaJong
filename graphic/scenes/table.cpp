#include "table.h"
#include "../resource.h"
#include "../rule.h"
#include "../sprite.h"
#include <iomanip>
#include "../../sound/sound.h"
#include "../../mjcore/bgmid.h"
#include "../event.h"
#include "../../mjcore/discard.h"
#include "../utils.h"
#include <functional>

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
	const unsigned logWidth = (unsigned)std::floor(0.5f + // VC++2010�ł�round()���g���Ȃ�
		(float)(((signed)Geometry::WindowWidth - (signed)Geometry::WindowHeight) / Geometry::WindowScale() - 36)) / 9u;
	logWindow = new logwnd::LogWindow(caller->getHWnd(), caller->getDevice(),
		1100, 100, logWidth, 20);
	InitializeCriticalSection(&subSceneCS);
	mySubScene = new TableSubsceneNormal(manipulator->getDevice());
	myTextRenderer = new TextRenderer(manipulator->getDevice());
	tileSelectMode = 0;
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

/* �����_�_�Ȃǂ̏���\�� */
void GameTableScreen::ShowStatus(const GameTable* gameStat) {
	const wchar_t* const WindName = L"���쐼�k��ᢒ�";
	const wchar_t* const Numeral = L"���O�l�ܘZ������\";
	const wchar_t* const FWDigit = L"�O�P�Q�R�S�T�U�V�W�X";
	CodeConv::tostringstream o;
	o << CodeConv::EnsureTStr(std::wstring(WindName + gameStat->GameRound / 4, WindName + gameStat->GameRound / 4 + 1));
	if (rules::chkRule("game_length", "twice_east_game") || rules::chkRule("game_length", "east_only_game")) { // ����݂̂̃��[��
		const unsigned roundnum = gameStat->LoopRound * 4 + gameStat->GameRound;
		if (roundnum < 10)
			o << CodeConv::EnsureTStr(std::wstring(Numeral + roundnum, Numeral + roundnum + 1));
		else
			o << (roundnum + 1);
	} else { // ��ʂ̃��[��
		const unsigned roundnum = gameStat->GameRound % 4;
		o << CodeConv::EnsureTStr(std::wstring(Numeral + roundnum, Numeral + roundnum + 1));
	}
	o << _T("�� ");
	if (gameStat->Honba >= 10) o << std::setw(2) << (gameStat->Honba) << _T("�{��");
	else if (gameStat->Honba > 0) o << CodeConv::EnsureTStr(std::wstring(FWDigit + gameStat->Honba, FWDigit + gameStat->Honba + 1)) << _T("�{��");
	else o << _T("���@��");
	myTextRenderer->NewText(0, o.str(), Geometry::BaseSize + 5, 5, 0.875f);

	o.str(_T(""));
	const int tiles = gameStat->RinshanPointer - gameStat->TilePointer - gameStat->ExtraRinshan - gameStat->DeadTiles + 1;
	if (tiles >= 100) o << _T("�c--�v");
	else o << _T("�c") << std::setw(2) << tiles << _T("�v");
	o << _T(" ����");
	if (gameStat->Deposit) o << std::setw(2) << gameStat->Deposit << _T("�{");
	else o << _T("�Ȃ�");
	myTextRenderer->NewText(1, o.str(), Geometry::BaseSize + 5, 35, 0.875f);

	myTextRenderer->Render();
}

/* ���\�� �������� */
void GameTableScreen::cls() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0); // �o�b�t�@�N���A
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
	buttonReconst->ChangeButtonSet(ButtonReconst::btnSetNormal);
	tehaiReconst->enable();
	tileSelectMode = 0;
	delete mySubScene; tehaiReconst->setTileCursor();
	switch (static_cast<TableSubsceneID>(scene_ID)) {
	case tblSubsceneBeginning:
		mySubScene = new TableSubsceneBeginning(caller->getDevice());
		break;
	case tblSubsceneHonba:
		mySubScene = new TableSubsceneTitlingHonba(caller->getDevice());
		break;
	case tblSubsceneRyuukyoku:
		mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("����"));
		break;
	case tblSubsceneSifeng:
		mySubScene = new TableSubsceneMsg(caller->getDevice(),
			(GameStatus::gameStat()->gameType & AllSanma) ? _T("�O���A��") : _T("�l���A��"));
		break;
	case tblSubsceneTripleRon:
		mySubScene = new TableSubsceneMsg(caller->getDevice(),
			(GameStatus::gameStat()->gameType & AllSanma) ? _T("��Ƙa") : _T("�O�Ƙa"));
		break;
	case tblSubsceneSikang:
		mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("�l�J��"));
		break;
	case tblSubsceneFourRiichi:
		mySubScene = new TableSubsceneMsg(caller->getDevice(),
			(GameStatus::gameStat()->gameType & AllSanma) ? _T("�O�Ɨ���") : _T("�l�Ɨ���"));
		break;
	case tblSubsceneChonbo:
		mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("���a"));
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
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("�s������"));
		break;
	case tblSubsceneCallValAgariten:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("�a���_"));
		break;
	case tblSubsceneCallValTsumibou:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("�ϖ_���Z"));
		break;
	case tblSubsceneCallValChip:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("�j�V���Z"));
		break;
	case tblSubsceneCallValKyoutaku:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("�������Z"));
		break;
	case tblSubsceneCallValChonboBappu:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("���a����"));
		break;
	case tblSubsceneCallValNagashiMangan:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("��������"));
		break;
	case tblSubsceneCallValDobon:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("��є���"));
		break;
	case tblSubsceneCallValKitamakura:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("�k������"));
		break;
	case tblSubsceneCallValYakuman:
		mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("�𖞏j�V"));
		break;
	case tblSubsceneChkTenpai:
		mySubScene = new TableSubsceneCheckTenpai(caller->getDevice());
		break;
	case tblSubscenePlayerDahai:
		mySubScene = new TableSubscenePlayerDahai(caller->getDevice());
		tehaiReconst->setTileCursor(NUM_OF_TILES_IN_HAND - 1);
		buttonReconst->btnSetForDahai();
		tehaiReconst->enable();
		if (GameStatus::gameStat()->Player.val[GameStatus::gameStat()->PlayerID].RichiFlag.RichiFlag)
			for (int i = 0; i < (NUM_OF_TILES_IN_HAND - 1); ++i)
				tehaiReconst->disable(i);
		tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
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
		buttonReconst->reconstruct();
	};
	const PlayerTable* const plDat = &(GameStatus::gameStat()->Player.val[GameStatus::gameStat()->PlayerID]);
	auto directTileCursor = [&](int cursorPos) -> void {
		if ((od->dwData) && ((tehaiReconst->isCursorEnabled()) || (buttonReconst->isCursorEnabled())) && (plDat->Hand[cursorPos].tile != NoTile)) {
			if (tehaiReconst->getTileCursor() == cursorPos) {
				FinishTileChoice();
			} else {
				buttonReconst->setCursor();
				tehaiReconst->setTileCursor(cursorPos);
				cursorMoved();
			}
		}
	};
	switch (od->dwOfs) {
	/* �{�^���I��/�v�I�� ���[�h�؂�ւ� */
	case DIK_UP: case DIK_K: // �v�I�����[�h�ɐ؂�ւ�
		if ((od->dwData) && (buttonReconst->isCursorEnabled())) {
			tehaiReconst->setTileCursor(NUM_OF_TILES_IN_HAND - 1);
			buttonReconst->setCursor();
			cursorMoved();
		}
		break;
	case DIK_DOWN: case DIK_J: // �{�^���I�����[�h�ɐ؂�ւ�
		if ((od->dwData) && (tehaiReconst->isCursorEnabled())) {
			tehaiReconst->setTileCursor();
			buttonReconst->setCursor(ButtonReconst::btnMAXIMUM - 1);
			cursorMoved();
		}
		break;
	/* �J�[�\���ړ� */
	case DIK_LEFT: case DIK_H:
		if ((od->dwData) && (tehaiReconst->isCursorEnabled())) {
			do {
				if (tehaiReconst->decrTileCursor() < 0) tehaiReconst->setTileCursor(NUM_OF_TILES_IN_HAND - 1);
			} while (plDat->Hand[tehaiReconst->getTileCursor()].tile == NoTile);
			cursorMoved();
		}
		else if ((od->dwData) && (buttonReconst->isCursorEnabled())) {
			if (buttonReconst->decCursor() < 0) buttonReconst->setCursor(ButtonReconst::btnMAXIMUM - 1);
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
		if ((od->dwData) && (buttonReconst->isCursorEnabled())) {
			if (buttonReconst->incCursor() >= ButtonReconst::btnMAXIMUM) buttonReconst->setCursor(0);
			cursorMoved();
		}
		break;
	/* �J�[�\���ʒu�̒��B�w�� */
	/* ASSUMING JAPANESE KEYBOARD: 1 2 3 4 5 6 7 8 9 0 - ^ �� BS */
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
	/* ����L�[ */
	case DIK_RETURN: case DIK_SPACE: case DIK_Z:
		if ((od->dwData) && (tehaiReconst->isCursorEnabled()))
			FinishTileChoice();
		else if ((od->dwData) && (buttonReconst->isCursorEnabled()))
			ButtonPressed();
		break;
	}
}

void GameTableScreen::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {
	const int scaledX = X / Geometry::WindowScale() * (Geometry::WindowWidth * 0.75f / Geometry::WindowHeight);
	const int scaledY = Y / Geometry::WindowScale();
	const int region = whichRegion(scaledX, scaledY);
	const bool isCursorEnabled = tehaiReconst->isCursorEnabled() || buttonReconst->isCursorEnabled();
	const bool isValidTile = (region >= 0) && (region < NUM_OF_TILES_IN_HAND) &&
		isCursorEnabled &&
		(GameStatus::gameStat()->Player.val[GameStatus::gameStat()->PlayerID].Hand[region].tile != NoTile);
	const bool isButton = (region >= ButtonReconst::ButtonRegionNum) &&
		(region < ButtonReconst::ButtonRegionNum + ButtonReconst::btnMAXIMUM) &&
		isCursorEnabled;
	switch (od->dwOfs) {
	case DIMOFS_X: case DIMOFS_Y: // �}�E�X�J�[�\���𓮂������ꍇ
		if ((region != tehaiReconst->getTileCursor()) && (isValidTile)) {
			tehaiReconst->setTileCursor(region);
			buttonReconst->setCursor();
			sound::Play(sound::IDs::sndCursor);
			tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
			buttonReconst->reconstruct();
		} else if ((region != (ButtonReconst::ButtonRegionNum + buttonReconst->getCursor())) && (isButton)) {
			tehaiReconst->setTileCursor();
			buttonReconst->setCursor(region - ButtonReconst::ButtonRegionNum);
			sound::Play(sound::IDs::sndCursor);
			tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
			buttonReconst->reconstruct();
		}
		break;
	case DIMOFS_BUTTON0: // �}�E�X�N���b�N
		if ((isValidTile) && (od->dwData))
			FinishTileChoice();
		else if ((isButton) && (od->dwData))
			ButtonPressed();
		break;
	}
}

/* �̔v�����肷�� */
void GameTableScreen::FinishTileChoice() {
	sound::Play(sound::IDs::sndClick);
	if (tehaiReconst->isCursorEnabled() && tehaiReconst->isEnabled(tehaiReconst->getTileCursor())) {
		const Int8ByTile TileCount = utils::countTilesInHand(GameStatus::gameStat(), GameStatus::gameStat()->CurrentPlayer.Active);
		if ((tileSelectMode == DiscardTileNum::Ankan) && (TileCount.val[tehaiReconst->getTileCursor()] == 1))
			ui::UIEvent->set((unsigned)tehaiReconst->getTileCursor() + (unsigned)(DiscardTileNum::Kakan * DiscardTileNum::TypeStep)); // ���Ȃ̏ꍇ
		else
			ui::UIEvent->set((unsigned)tehaiReconst->getTileCursor() + (unsigned)(tileSelectMode * DiscardTileNum::TypeStep)); // �v�̔ԍ���ݒ�
	} else {
		sound::Play(sound::IDs::sndCuohu);
	}
}

/* �{�^���������ꂽ���̏��� */
void GameTableScreen::ButtonPressed() {
	auto setMode = [&](DiscardTileNum::discardType mode, ButtonReconst::ButtonID button, std::function<bool(int, GameTable*)> f) -> void {
		tileSelectMode = mode;
		buttonReconst->setSunkenButton(button);
		for (int i = 0; i < mihajong_graphic::GameTableScreen::ButtonReconst::btnMAXIMUM; ++i)
			if (i != button)
				buttonReconst->disable((mihajong_graphic::GameTableScreen::ButtonReconst::ButtonID)i);
		buttonReconst->reconstruct();
		tehaiReconst->enable();
		for (int i = 0; i < NUM_OF_TILES_IN_HAND; ++i) {
			GameTable tmpStat; std::memcpy(&tmpStat, GameStatus::gameStat(), sizeof (GameTable));
			if (f(i, &tmpStat)) tehaiReconst->disable(i);
		}
		tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->CurrentPlayer.Active);
	};

	sound::Play(sound::IDs::sndButton);
	if (!buttonReconst->isEnabled((ButtonReconst::ButtonID)buttonReconst->getCursor())) {
		sound::Play(sound::IDs::sndCuohu);
	} else if (buttonReconst->getButtonSet() == ButtonReconst::btnSetTsumo) {
		switch (buttonReconst->getCursor()) {
		case ButtonReconst::btnTsumo:
			CallTsumoAgari();
			break;
		case ButtonReconst::btnKyuushu:
			CallKyuushuKyuuhai();
			break;
		case ButtonReconst::btnRiichi: // ����
			setMode(DiscardTileNum::Riichi, ButtonReconst::btnRiichi,
				[](int i, GameTable* tmpStat) -> bool {
					tmpStat->Player.val[tmpStat->CurrentPlayer.Active].Hand[i].tile = NoTile;
					SHANTEN Shanten = utils::calcShanten(tmpStat, tmpStat->CurrentPlayer.Active, ShantenAnalyzer::shantenAll);
					return (Shanten > 0);
				});
			break;
		case ButtonReconst::btnKan: // �J��
			setMode(DiscardTileNum::Ankan, ButtonReconst::btnKan,
				[](int i, GameTable* tmpStat) -> bool {
					bool flag = false;
					const PLAYER_ID ActivePlayer = tmpStat->CurrentPlayer.Active;
					const Int8ByTile TileCount = utils::countTilesInHand(tmpStat, ActivePlayer);
					const PlayerTable* const playerStat = &(tmpStat->Player.val[ActivePlayer]);
					if (TileCount.val[playerStat->Hand[i].tile] < 4) flag = true;
					if (TileCount.val[playerStat->Hand[i].tile] == 4) {
						for (int j = 1; j <= playerStat->MeldPointer; ++j)
							if ((playerStat->Meld[j].tile == i) &&
								((playerStat->Meld[j].mstat == meldTripletExposedLeft) ||
								(playerStat->Meld[j].mstat == meldTripletExposedCenter) ||
								(playerStat->Meld[j].mstat == meldTripletExposedRight)))
								flag = false;
					}
					return flag;
				});
			break;
		case ButtonReconst::btnFlower: // �Ԕv
			setMode(DiscardTileNum::Flower, ButtonReconst::btnFlower,
				[](int i, GameTable* tmpStat) -> bool {
					return tmpStat->Player.val[tmpStat->CurrentPlayer.Active].Hand[i].tile !=
						(tmpStat->gameType & SanmaX) ? NorthWind : Flower;
				});
			break;
		default:
			sound::Play(sound::IDs::sndCuohu);
			break;
		}
	}
}

void GameTableScreen::CallTsumoAgari() { // �c���A�K��
	ui::UIEvent->set(0xffffffff);
}
void GameTableScreen::CallKyuushuKyuuhai() { // ����v
	ui::UIEvent->set(0xfffffffe);
}


}
