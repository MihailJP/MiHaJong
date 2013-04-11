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
	tileTipReconst = new TileTipReconst(this);
	Reconstruct(GameStatus::retrGameStat());
	const unsigned logWidth = (unsigned)floor(0.5f + // VC++2010�ł�round()���g���Ȃ�
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
	delete myTextRenderer;
	if (tBorder) tBorder->Release();
	if (tBaize) tBaize->Release();
}

void GameTableScreen::ReconstructPlayer(const GameTable* gameStat, PlayerID deckTargetPlayer, PlayerID targetPlayer) {
	yamahaiReconst->Reconstruct(gameStat, deckTargetPlayer, targetPlayer);
	tehaiReconst->Reconstruct(gameStat, targetPlayer);
	nakihaiReconst->Reconstruct(gameStat, targetPlayer);
	sutehaiReconst->Reconstruct(gameStat, targetPlayer);
}

void GameTableScreen::Reconstruct(const GameTable* gameStat) {
	if (gameStat->chkGameType(Yonma)) {
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
		roundColor(), 1.0f, 0); // �o�b�t�@�N���A
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
	logWindow->Render();
	chatInput->Render();
}

void GameTableScreen::checkTimeout() {
	if ((mySubScene) && (mySubScene->timeout() <= 0) && (buttonReconst->areEnabled().any() || tehaiReconst->isCursorEnabled() || buttonReconst->isCursorEnabled())) { /* �^�C���A�E�g�̏��� */
		const bool isNakiSel = (buttonReconst->getButtonSet() == ButtonReconst::btnSetNormal) && buttonReconst->areEnabled().any();
		if (isNakiSel) { // ���I�𒆂̎�
			ui::UIEvent->set(naki::nakiNone); // �v�̔ԍ���ݒ�
		} else if (buttonReconst->getButtonSet() == ButtonReconst::btnSetTsumo) {
			if (GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].Tsumohai().tile != NoTile)
				ui::UIEvent->set(NumOfTilesInHand - 1); // �c���؂�
			else ui::UIEvent->set(0); // ��������̏ꍇ
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
		ui::UIEvent->reset(); ui::cancellableWait->reset();
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
		case tblSubsceneAlice:
			mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("�A���X����"));
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
			(void)GameStatus::retrGameStat();
			// �J�[�\���ƃ{�^���̐ݒ�
			tehaiReconst->setTileCursor(NumOfTilesInHand - 1);
			while (GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].Hand[tehaiReconst->getTileCursor()].tile == NoTile)
				tehaiReconst->decrTileCursor(); // ��������̎��̃J�[�\�������ʒu
			buttonReconst->btnSetForDahai();
			tehaiReconst->enable();
			if (GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].RichiFlag.RichiFlag)
				for (int i = 0; i < (NumOfTilesInHand - 1); ++i)
					tehaiReconst->disable(i);
			tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
			// ���[�`��I�[�g�c���؂�
			if ((GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].RichiFlag.RichiFlag) &&
				buttonReconst->areEnabled().none())
				ui::UIEvent->set(NumOfTilesInHand - 1);
			else if (checkBoxes[ChkBoxAutoDiscard]->isChecked() && // �����c���؂�
				buttonReconst->areEnabled().none())
				ui::UIEvent->set(NumOfTilesInHand - 1);
			else if ((buttonReconst->isEnabled(buttonReconst->btnTsumo)) &&
				(checkBoxes[ChkBoxAutoAgari]->isChecked())) // �I�[�g�a��
				CallTsumoAgari();
			else // ���̔Ԃ������特��炷
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
			// �J�[�\���ƃ{�^���̐ݒ�
			buttonReconst->btnSetForNaki();
			buttonReconst->setCursor(buttonReconst->isEnabled(GameTableScreen::ButtonReconst::btnRon) ? GameTableScreen::ButtonReconst::btnRon : GameTableScreen::ButtonReconst::btnPass);
			buttonReconst->reconstruct();
			if (buttonReconst->areEnabled().none()) // �Y������v���Ȃ��Ȃ�X���[
				ui::UIEvent->set(naki::nakiNone);
			else if ((!(buttonReconst->isEnabled(buttonReconst->btnRon))) && /* ButtonReconst::btnRon ���Ɖ��̂��G���[�ɂȂ� */
				(checkBoxes[ChkBoxAutoPass]->isChecked())) // �I�[�g�p�X
				ui::UIEvent->set(naki::nakiNone);
			else if ((buttonReconst->isEnabled(buttonReconst->btnRon)) &&
				(checkBoxes[ChkBoxAutoAgari]->isChecked())) // �I�[�g�a��
				ui::UIEvent->set(naki::nakiRon);
			else // ����炷
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

void GameTableScreen::IMEvent(UINT message, WPARAM wParam, LPARAM lParam) {
	if (chatInput->is_Active())
		chatInput->IMEvent(message, wParam, lParam);
}
void GameTableScreen::KeyboardInput(WPARAM wParam, LPARAM lParam) {
	if (chatInput->is_Active()) {
		if (wParam == CHARDAT_CURSOR_ENTER) {
			sound::Play(sound::IDs::sndClick);
			utils::sendchat(chatInput->getText().c_str());
			chatInput->setText(_T(""));
		} else if ((wParam == CHARDAT_CURSOR_ESCAPE) || (wParam == '\t')) {
			sound::Play(sound::IDs::sndClick);
			chatInput->deactivate();
		} else {
			chatInput->KeyboardInput(wParam, lParam);
		}
	} else {
		if (wParam == '\t') {
			sound::Play(sound::IDs::sndClick);
			chatInput->activate();
		}
	}
}

void GameTableScreen::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
	if (chatInput->is_Active()) return; // ���͒��͖���

	const bool isNakiSel = (buttonReconst->getButtonSet() == ButtonReconst::btnSetNormal) && buttonReconst->areEnabled().any();
	auto cursorMoved = [&]() -> void {
		sound::Play(sound::IDs::sndCursor);
		tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
		buttonReconst->reconstruct();
		tileTipReconst->reconstruct();
	};
	const PlayerTable* const plDat = &(GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID]);
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
		if ((od->dwData) && (buttonReconst->isCursorEnabled()) && (!isNakiSel)) {
			tehaiReconst->setTileCursor(NumOfTilesInHand - 1);
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
				if (tehaiReconst->decrTileCursor() < 0) tehaiReconst->setTileCursor(NumOfTilesInHand - 1);
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
				if (tehaiReconst->incrTileCursor() >= NumOfTilesInHand) tehaiReconst->setTileCursor(0);
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
		if (od->dwData)
			subSceneCS.trySyncDo<void>(nullptr, [this]() -> void {
				mySubScene->skipEvent();
			});
		if ((od->dwData) && (tehaiReconst->isCursorEnabled()))
			FinishTileChoice();
		else if ((od->dwData) && (buttonReconst->isCursorEnabled()))
			buttonReconst->ButtonPressed();
		break;
	}
}

void GameTableScreen::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {
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
	switch (od->dwOfs) {
	case DIMOFS_X: case DIMOFS_Y: // �}�E�X�J�[�\���𓮂������ꍇ
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
	case DIMOFS_BUTTON0: // �}�E�X�N���b�N
		if (od->dwData)
			subSceneCS.trySyncDo<void>(nullptr, [this]() -> void {
				mySubScene->skipEvent();
			});
		if ((!chatInput->is_Active()) && (isValidTile) && (od->dwData))
			FinishTileChoice();
		else if ((!chatInput->is_Active()) && (isButton) && (od->dwData))
			buttonReconst->ButtonPressed();
		else if ((!chatInput->is_Active()) && (region == ChatInputRegion) && (od->dwData)) {
			sound::Play(sound::IDs::sndClick);
			chatInput->activate();
		}
		else if ((chatInput->is_Active()) && (region != ChatInputRegion) && (od->dwData)) {
			sound::Play(sound::IDs::sndClick);
			chatInput->deactivate();
		}
		break;
	case DIMOFS_BUTTON1: // �}�E�X�E�N���b�N
		if ((!chatInput->is_Active()) && (od->dwData) && isNakiSel) { // ���I�𒆂̎�
			sound::Play(sound::IDs::sndClick);
			ui::UIEvent->set(naki::nakiNone); // �v�̔ԍ���ݒ�
		}
		break;
	}
}

/* �̔v�����肷�� */
void GameTableScreen::FinishTileChoice() {
	sound::Play(sound::IDs::sndClick);
	if (tehaiReconst->isCursorEnabled() && tehaiReconst->isEnabled(tehaiReconst->getTileCursor())) {
		const Int8ByTile TileCount = utils::countTilesInHand(GameStatus::gameStat(), GameStatus::gameStat()->CurrentPlayer.Active);
		if ((tileSelectMode == DiscardTileNum::Ankan) && (TileCount[GameStatus::gameStat()->statOfActive().Hand[tehaiReconst->getTileCursor()].tile] == 1))
			ui::UIEvent->set((unsigned)tehaiReconst->getTileCursor() + (unsigned)(DiscardTileNum::Kakan * DiscardTileNum::TypeStep)); // ���Ȃ̏ꍇ
		else
			ui::UIEvent->set((unsigned)tehaiReconst->getTileCursor() + (unsigned)(tileSelectMode * DiscardTileNum::TypeStep)); // �v�̔ԍ���ݒ�
	} else {
		sound::Play(sound::IDs::sndCuohu);
	}
}

void GameTableScreen::CallTsumoAgari() { // �c���A�K��
	ui::UIEvent->set(0xffffffff);
}
void GameTableScreen::CallKyuushuKyuuhai() { // ����v
	ui::UIEvent->set(0xfffffffe);
}


}
