#include "nakibtn.h"

#include "../../geometry.h"
#include "../../utils.h"
#include "../../gametbl.h"
#include "../../rule.h"

namespace mihajong_graphic {

const GameTableScreen::ButtonReconst::BtnData
	GameTableScreen::ButtonReconst::buttonDat[2][GameTableScreen::ButtonReconst::btnMAXIMUM] = {
		{
			{_T("���`�["),   5 + 117 * 0, Geometry::BaseSize - 40, 0xffccff66},
			{_T("�ƃ`�["),   5 + 117 * 1, Geometry::BaseSize - 40, 0xff99ff99},
			{_T("�E�`�["),   5 + 117 * 2, Geometry::BaseSize - 40, 0xff66ff99},
			{_T("�|��"),     5 + 117 * 3, Geometry::BaseSize - 40, 0xff99ccff},
			{_T("�J��"),     5 + 117 * 4, Geometry::BaseSize - 40, 0xff9966ff},
			{_T("�p�X"),     5 + 117 * 5, Geometry::BaseSize - 40, 0xffcccccc},
			{_T("����"),     5 + 117 * 6, Geometry::BaseSize - 40, 0xffffcc66},
		}, {
			{_T(""),         5 + 117 * 0, Geometry::BaseSize - 40, 0xff666666},
			{_T(""),         5 + 117 * 1, Geometry::BaseSize - 40, 0xff666666},
			{_T("����v"), 5 + 117 * 2, Geometry::BaseSize - 40, 0xff99ffff},
			{_T("�Ԕv"),     5 + 117 * 3, Geometry::BaseSize - 40, 0xff99ccff},
			{_T("�J��"),     5 + 117 * 4, Geometry::BaseSize - 40, 0xff9966ff},
			{_T("���[�`"),   5 + 117 * 5, Geometry::BaseSize - 40, 0xffff66ff},
			{_T("�c��"),     5 + 117 * 6, Geometry::BaseSize - 40, 0xffffff66},
		},
};

void GameTableScreen::ButtonReconst::Render() {
	EnterCriticalSection(&reconstructionCS);
	buttons->Render();
	buttons->Render();
	LeaveCriticalSection(&reconstructionCS);
}

void GameTableScreen::ButtonReconst::reconstruct(ButtonID buttonID) {
#include "color.h"
	Color btnColor; btnColor.rgbaAsOneValue = buttonDat[currentButtonSet][buttonID].color;
	if (!buttonEnabled[buttonID]) { // �Ó]����
		btnColor.rgbaAsStruct.r /= 3;
		btnColor.rgbaAsStruct.g /= 3;
		btnColor.rgbaAsStruct.b /= 3;
	}
	buttons->setButton(buttonID,
		(sunkenButton == buttonID) ? ButtonPic::sunken : ((cursor == buttonID) ? ButtonPic::raised : ButtonPic::clear),
		buttonDat[currentButtonSet][buttonID].x * Geometry::WindowScale(),
		buttonDat[currentButtonSet][buttonID].y * Geometry::WindowScale(),
		117 * Geometry::WindowScale(), 36 * Geometry::WindowScale(),
		btnColor.rgbaAsOneValue, buttonDat[currentButtonSet][buttonID].label);
	const Region nullRegion = {0, 0, -1, -1};
	if (caller->regions.size() <= (buttonID + ButtonRegionNum))
		caller->regions.resize(ButtonRegionNum + 1 + buttonID, nullRegion);
	caller->regions[buttonID + ButtonRegionNum].Left   = buttonDat[currentButtonSet][buttonID].x;
	caller->regions[buttonID + ButtonRegionNum].Top    = buttonDat[currentButtonSet][buttonID].y;
	caller->regions[buttonID + ButtonRegionNum].Right  = buttonDat[currentButtonSet][buttonID].x + 117;
	caller->regions[buttonID + ButtonRegionNum].Bottom = buttonDat[currentButtonSet][buttonID].y + 36;
}
void GameTableScreen::ButtonReconst::reconstruct() {
	EnterCriticalSection(&reconstructionCS);
	for (unsigned i = 0; i < btnMAXIMUM; ++i)
		reconstruct((ButtonID)i);
	LeaveCriticalSection(&reconstructionCS);
}

void GameTableScreen::ButtonReconst::ChangeButtonSet(ButtonSet btnSet) {
	currentButtonSet = btnSet;
	cursor = CursorDisabled;
	sunkenButton = NoSunkenButton;
	buttonEnabled.reset();
	reconstruct();
}

void GameTableScreen::ButtonReconst::enable(ButtonID buttonID) {
	buttonEnabled[buttonID] = true; reconstruct(buttonID);
}
void GameTableScreen::ButtonReconst::disable(ButtonID buttonID) {
	buttonEnabled[buttonID] = false; reconstruct(buttonID);
}
void GameTableScreen::ButtonReconst::enable(const std::bitset<btnMAXIMUM>& flagset) {
	buttonEnabled = flagset;
	reconstruct();
}

void GameTableScreen::ButtonReconst::btnSetForDahai() { // �c���Ԃ̎��p��
	currentButtonSet = btnSetTsumo; buttonEnabled.reset(); // ��Ԃ����Z�b�g
	const GameTable* const gameStat = GameStatus::retrGameStat();
	auto tilesMoreThan = [gameStat](int tiles) {
		return (gameStat->TilePointer + tiles) < (gameStat->RinshanPointer - gameStat->DeadTiles - 1);
	};
	const PLAYER_ID ActivePlayer = gameStat->CurrentPlayer.Active;
	const PlayerTable* const playerStat = &(gameStat->Player.val[ActivePlayer]);
	const SHANTEN Shanten = utils::calcShanten(gameStat, ActivePlayer, ShantenAnalyzer::shantenAll);
	if (utils::isRichiReqSatisfied(gameStat, ActivePlayer) && // �_�_�v���𖞂����Ă���i�_�_������Ă���j
		(Shanten <= 0) && // �e���p�C���Ă���
		(playerStat->MenzenFlag || (!rules::chkRule("riichi_shibari", "no"))) && // ��O�ł��邩�A���[�`���胋�[���ł���
		(!playerStat->RichiFlag.RichiFlag) && // �܂����[�`���Ă��Ȃ�
		(tilesMoreThan((gameStat->gameType & AllSanma) ? 2 : 3))) // �c��c���v���\������Ȃ�
		buttonEnabled[btnRiichi] = true; // ���[�`�{�^����L����

	const bool DaoPaiAbilityFlag = utils::chkdaopaiability(gameStat, ActivePlayer);
	if ((DaoPaiAbilityFlag) && (playerStat->FirstDrawFlag))
		buttonEnabled[btnKyuushu] = true; // ����v�{�^��

	const bool ShisanBuDa = utils::chkShisanBuDa(gameStat, ActivePlayer);
	const bool ShisiBuDa = utils::chkShisiBuDa(gameStat, ActivePlayer);
	if (((Shanten <= -1) && (playerStat->Hand[NUM_OF_TILES_IN_HAND - 1].tile != NoTile)) || // �a���ɂȂ��Ă��邩
		ShisanBuDa || ShisiBuDa) // �\�O�s���̏ꍇ�i�\�O�s���Ȃ��̏ꍇ���̕ϐ���false�ɂȂ�j
		buttonEnabled[btnTsumo] = true; // �a���{�^��

	const Int8ByTile TileCount = utils::countTilesInHand(gameStat, ActivePlayer);
	const int kanLim = rules::chkRule("fifth_kong", "no") ? 4 : 5;
	const bool KanFlag = [tilesMoreThan, gameStat, TileCount, playerStat, kanLim]() -> bool {
		for (int i = 1; i < TILE_NONFLOWER_MAX; ++i) {
			if (tilesMoreThan(0) && (gameStat->KangNum < kanLim)) {
				// �ÞȂł���ꍇ
				if (TileCount.val[i] == 4)
					return true;
				// ���Ȃł���ꍇ
				else if (TileCount.val[i] == 1) {
					for (int j = 1; j <= playerStat->MeldPointer; ++j)
						if ((playerStat->Meld[j].tile == i) &&
							((playerStat->Meld[j].mstat == meldTripletExposedLeft) ||
							(playerStat->Meld[j].mstat == meldTripletExposedCenter) ||
							(playerStat->Meld[j].mstat == meldTripletExposedRight)))
							return true;
				}
			}
		}
		return false;
	} ();
	if (KanFlag && (playerStat->Hand[NUM_OF_TILES_IN_HAND - 1].tile != NoTile) &&
		(!playerStat->RichiFlag.RichiFlag) || utils::chkAnkanAbility(gameStat, ActivePlayer))
		buttonEnabled[btnKan] = true;

	reconstruct();
}

GameTableScreen::ButtonReconst::ButtonReconst(GameTableScreen* parent) {
	InitializeCriticalSection(&reconstructionCS);
	caller = parent;
	cursor = CursorDisabled; sunkenButton = NoSunkenButton;
	buttons = new ButtonPic(caller->caller->getDevice());
	ChangeButtonSet(btnSetNormal);
}

GameTableScreen::ButtonReconst::~ButtonReconst() {
	delete buttons;
	DeleteCriticalSection(&reconstructionCS);
}

}
