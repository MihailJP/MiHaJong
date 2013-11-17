#include <cmath>
#include "../../pi.h"

#include "nakibtn.h"

#include "../../geometry.h"
#include "../../utils.h"
#include "../../gametbl.h"
#include "../../rule.h"
#include "../../event.h"
#include "../../../common/discard.h"
#include "../../../sound/sound.h"
#include "../../../common/bgmid.h"

#include "tehai.h"
#include "naki_id.h"

namespace mihajong_graphic {

const GameTableScreen::ButtonReconst::BtnData
	GameTableScreen::ButtonReconst::buttonDat[2][GameTableScreen::ButtonReconst::btnMAXIMUM] = {
		{
			{_T("左チー"),   5 + 117 * 0, Geometry::BaseSize - 40, 0xffccff66},
			{_T("嵌チー"),   5 + 117 * 1, Geometry::BaseSize - 40, 0xff99ff99},
			{_T("右チー"),   5 + 117 * 2, Geometry::BaseSize - 40, 0xff66ff99},
			{_T("ポン"),     5 + 117 * 3, Geometry::BaseSize - 40, 0xff99ccff},
			{_T("カン"),     5 + 117 * 4, Geometry::BaseSize - 40, 0xff9966ff},
			{_T("パス"),     5 + 117 * 5, Geometry::BaseSize - 40, 0xffcccccc},
			{_T("ロン"),     5 + 117 * 6, Geometry::BaseSize - 40, 0xffffcc66},
		}, {
			{_T("開立直"),   5 + 117 * 0, Geometry::BaseSize - 40, 0xffff6666},
			{_T("リーチ"),   5 + 117 * 1, Geometry::BaseSize - 40, 0xffff66ff},
			{_T("九種九牌"), 5 + 117 * 2, Geometry::BaseSize - 40, 0xff99ffff},
			{_T("花牌"),     5 + 117 * 3, Geometry::BaseSize - 40, 0xff99ccff},
			{_T("カン"),     5 + 117 * 4, Geometry::BaseSize - 40, 0xff9966ff},
			{_T(""),         5 + 117 * 5, Geometry::BaseSize - 40, 0xff666666},
			{_T("ツモ"),     5 + 117 * 6, Geometry::BaseSize - 40, 0xffffff66},
		},
};

GameTableScreen::ButtonReconst::ButtonSet GameTableScreen::ButtonReconst::getButtonSet() {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	return currentButtonSet;
}
std::bitset<GameTableScreen::ButtonReconst::btnMAXIMUM> GameTableScreen::ButtonReconst::areEnabled() {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	return buttonEnabled;
}
bool GameTableScreen::ButtonReconst::isEnabled(ButtonID buttonID) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	return (buttonID >= 0) && (buttonID < btnMAXIMUM) && buttonEnabled[buttonID];
}
bool GameTableScreen::ButtonReconst::isSunkenButtonExists() {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	return sunkenButton != NoSunkenButton;
}
int GameTableScreen::ButtonReconst::getSunkenButtonID() {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	return sunkenButton;
}
void GameTableScreen::ButtonReconst::setSunkenButton(int buttonID) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	sunkenButton = buttonID;
}
bool GameTableScreen::ButtonReconst::isCursorEnabled() {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	return cursor != CursorDisabled;
}
int GameTableScreen::ButtonReconst::getCursor() {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	return cursor;
}
void GameTableScreen::ButtonReconst::setCursor(int cursorPos) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	cursor = cursorPos;
}
int GameTableScreen::ButtonReconst::incCursor() {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	return ++cursor;
}
int GameTableScreen::ButtonReconst::decCursor() {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	return --cursor;
}

void GameTableScreen::ButtonReconst::Render() {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
#ifndef _WIN32
	if (!initialized) {
		reconstruct();
		initialized = true;
	}
#endif /*_WIN32*/
	if (cursor != CursorDisabled) {
#include "color.h"
		Color btnColor; btnColor.rgbaAsOneValue = buttonDat[currentButtonSet][cursor].color;
		const double Zeit = (double)(myTimer.currTime() % 9000000ULL);
		btnColor.rgbaAsStruct.r = (unsigned)((double)btnColor.rgbaAsStruct.r * (sin(Zeit / 450000.0 * M_PI) / 4.0 + 0.75));
		btnColor.rgbaAsStruct.g = (unsigned)((double)btnColor.rgbaAsStruct.g * (sin(Zeit / 450000.0 * M_PI) / 4.0 + 0.75));
		btnColor.rgbaAsStruct.b = (unsigned)((double)btnColor.rgbaAsStruct.b * (sin(Zeit / 450000.0 * M_PI) / 4.0 + 0.75));
		buttons->setButton(cursor,
			(sunkenButton == cursor) ? ButtonPic::sunken : (buttonEnabled[cursor] ? ButtonPic::raised : ButtonPic::clear),
			buttonDat[currentButtonSet][cursor].x * Geometry::WindowScale(),
			buttonDat[currentButtonSet][cursor].y * Geometry::WindowScale(),
			117 * Geometry::WindowScale(), 36 * Geometry::WindowScale(),
			btnColor.rgbaAsOneValue, buttonDat[currentButtonSet][cursor].label);
	}
	buttons->Render();
	buttons->Render();
	buttons->Render();
}

void GameTableScreen::ButtonReconst::reconstruct(ButtonID buttonID) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
#include "color.h"
	Color btnColor; btnColor.rgbaAsOneValue = buttonDat[currentButtonSet][buttonID].color;
	/*if (!buttonEnabled[buttonID]) { // 暗転処理
		btnColor.rgbaAsStruct.r /= 3;
		btnColor.rgbaAsStruct.g /= 3;
		btnColor.rgbaAsStruct.b /= 3;
	}*/
	buttons->setButton(buttonID,
		(sunkenButton == buttonID) ? ButtonPic::sunken : (buttonEnabled[buttonID] ? ButtonPic::raised : ButtonPic::clear),
		buttonDat[currentButtonSet][buttonID].x * Geometry::WindowScale(),
		buttonDat[currentButtonSet][buttonID].y * Geometry::WindowScale(),
		117 * Geometry::WindowScale(), 36 * Geometry::WindowScale(),
		btnColor.rgbaAsOneValue, buttonDat[currentButtonSet][buttonID].label);
	caller->setRegion(buttonID + ButtonRegionNum,
		buttonDat[currentButtonSet][buttonID].x      , buttonDat[currentButtonSet][buttonID].y,
		buttonDat[currentButtonSet][buttonID].x + 117, buttonDat[currentButtonSet][buttonID].y + 36);
}
void GameTableScreen::ButtonReconst::reconstruct() {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	for (unsigned i = 0; i < btnMAXIMUM; ++i)
		reconstruct((ButtonID)i);
}

void GameTableScreen::ButtonReconst::ChangeButtonSet(ButtonSet btnSet) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	currentButtonSet = btnSet;
	cursor = CursorDisabled;
	sunkenButton = NoSunkenButton;
	buttonEnabled.reset();
	reconstruct();
#ifndef _WIN32
	initialized = false;
#endif /*_WIN32*/
}

void GameTableScreen::ButtonReconst::enable(ButtonID buttonID) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	buttonEnabled[buttonID] = true; reconstruct(buttonID);
}
void GameTableScreen::ButtonReconst::disable(ButtonID buttonID) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	buttonEnabled[buttonID] = false; reconstruct(buttonID);
}
void GameTableScreen::ButtonReconst::enable(const std::bitset<btnMAXIMUM>& flagset) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	buttonEnabled = flagset;
	reconstruct();
}

void GameTableScreen::ButtonReconst::btnSetForDahai() { // ツモ番の時用の
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	currentButtonSet = btnSetTsumo; buttonEnabled.reset(); // 状態をリセット
	const GameTable* const gameStat = GameStatus::retrGameStat();
	auto tilesMoreThan = [gameStat](int tiles) {
		return (gameStat->TilePointer + tiles) < (gameStat->RinshanPointer - gameStat->DeadTiles - 1);
	};
	const PlayerID ActivePlayer = gameStat->CurrentPlayer.Active;
	const PlayerTable* const playerStat = &(gameStat->Player[ActivePlayer]);
	const Shanten shanten = utils::calcShanten(gameStat, ActivePlayer, shantenAll);
	if (utils::isRichiReqSatisfied(gameStat, ActivePlayer) && // 点棒要件を満たしている（点棒が足りている）
		(shanten <= 0) && // テンパイしている
		(gameStat->gameType & RichiMJ) && // 日麻である
		(playerStat->MenzenFlag || (!rules::chkRule("riichi_shibari", "no"))) && // 門前であるか、リーチ縛りルールである
		(!playerStat->RichiFlag.RichiFlag) && // まだリーチしていない
		(tilesMoreThan((gameStat->gameType & AllSanma) ? 2 : 3))) { // 残りツモ牌が十分あるなら
			buttonEnabled[btnRiichi] = true; // リーチボタンを有効に
			if ((!rules::chkRule("open_riichi", "no")) && playerStat->MenzenFlag) // オープンリーチありの場合で門前の場合
				buttonEnabled[btnOpenRiichi] = true; // ボタンを有効に
	}

	const bool DaoPaiAbilityFlag = (gameStat->gameType & RichiMJ) && utils::chkdaopaiability(gameStat, ActivePlayer);
	if ((gameStat->gameType & RichiMJ) && (DaoPaiAbilityFlag) && (playerStat->FirstDrawFlag))
		buttonEnabled[btnKyuushu] = true; // 九種九牌ボタン

	const bool ShisanBuDa = (gameStat->gameType & RichiMJ) && utils::chkShisanBuDa(gameStat, ActivePlayer);
	const bool ShisiBuDa = (gameStat->gameType & RichiMJ) && utils::chkShisiBuDa(gameStat, ActivePlayer);
	if (((shanten <= -1) && (playerStat->Tsumohai().tile != NoTile)) || // 和了になっているか
		ShisanBuDa || ShisiBuDa) // 十三不塔の場合（十三不塔なしの場合この変数はfalseになる）
		buttonEnabled[btnTsumo] = true; // 和了ボタン

	const Int8ByTile TileCount = utils::countTilesInHand(gameStat, ActivePlayer);
	const int kanLim = (gameStat->gameType & GuobiaoMJ) ? 16 : rules::chkRule("fifth_kong", "no") ? 4 : 5;
	const bool KanFlag = [tilesMoreThan, gameStat, TileCount, playerStat, kanLim]() -> bool {
		for (int i = 1; i < TileNonflowerMax; ++i) {
			if (tilesMoreThan(0) && (gameStat->KangNum < kanLim)) {
				// 暗槓できる場合
				if (TileCount[i] == 4)
					return true;
				// 加槓できる場合
				else if (TileCount[i] == 1) {
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
	if (KanFlag && (playerStat->Tsumohai().tile != NoTile) &&
		(!playerStat->RichiFlag.RichiFlag) || utils::chkAnkanAbility(gameStat, ActivePlayer))
		buttonEnabled[btnKan] = true;

	const TileCode flowerTile = (gameStat->gameType & SanmaX) ? NorthWind : Flower;
	const bool Flowerabilityflag = [gameStat, playerStat]() -> bool {
		if (gameStat->gameType & SanmaX)
			return (playerStat->Tsumohai().tile == NorthWind) && (!rules::chkRule("flower_tiles", "no"));
		else
			return playerStat->Tsumohai().tile > TileSuitFlowers;
	} ();
	if (((gameStat->gameType & GuobiaoMJ) || (!rules::chkRule("flower_tiles", "no"))) &&
		(playerStat->Tsumohai().tile != NoTile) &&
		(TileCount[flowerTile] >= 1) &&
		((!playerStat->RichiFlag.RichiFlag) || Flowerabilityflag))
		buttonEnabled[btnFlower] = true;

	reconstruct();
}

void GameTableScreen::ButtonReconst::btnSetForNaki() { // 鳴きの時用の
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	currentButtonSet = btnSetNormal; buttonEnabled.reset(); // 状態をリセット
	GameTable* const gameStat = new GameTable;
	memcpy(gameStat, GameStatus::retrGameStat(), sizeof (GameTable));
	auto tilesMoreThan = [gameStat](int tiles) {
		return (gameStat->TilePointer + tiles) < (gameStat->RinshanPointer - gameStat->DeadTiles - 1);
	};
	const PlayerID ActivePlayer = gameStat->CurrentPlayer.Active;
	const PlayerID PassivePlayer = gameStat->CurrentPlayer.Passive;
	PlayerTable* const playerStat = &(gameStat->Player[PassivePlayer]);
	playerStat->Tsumohai().tile = gameStat->CurrentDiscard.tile;
	const Shanten shanten = utils::calcShanten(gameStat, gameStat->PlayerID, shantenAll);
	playerStat->Tsumohai().tile = NoTile;

	if (gameStat->CurrentDiscard.tile > TileSuitFlowers) goto end; /* 花牌の場合は残りの判定をスキップ */
	if (playerStat->AgariHouki) goto end; /* 和了り放棄だったら残りの判定をスキップ */
	if ((gameStat->KangFlag.chankanFlag == chankanOfAnkan) && // 暗槓に対する搶槓判定のときで、
		(gameStat->gameType & RichiMJ) && // 中国ルール以外で
		(utils::calcShanten(gameStat, PassivePlayer, shantenOrphans) >= 0)) // 国士聴牌でない場合は
		goto end; // 残りの判定をスキップ

	if (shanten < 0) // 出た牌が当たり牌
		buttonEnabled[btnRon] = true; // 和了ボタン（フリテンの場合なども点灯）

	if ((!playerStat->RichiFlag.RichiFlag) && tilesMoreThan(0)) { // リーチしてなくて河底でないとき……
		const Int8ByTile TileCount = utils::countTilesInHand(gameStat, PassivePlayer);
		const int kanLim = (gameStat->gameType & GuobiaoMJ) ? 16 : rules::chkRule("fifth_kong", "no") ? 4 : 5;

		if (TileCount[gameStat->CurrentDiscard.tile] >= 2)
			buttonEnabled[btnPon] = true; // ポン
		if ((TileCount[gameStat->CurrentDiscard.tile] >= 3) && // 出てきた牌を暗刻で持っていて
			(gameStat->KangFlag.chankanFlag == chankanNone) && // 槍槓の判定中ではなくて
			(gameStat->KangNum < kanLim)) // 限度以内の場合
			buttonEnabled[btnKan] = true; // カン

		// チーできる条件：上家の捨牌であること、かつ、数牌であること
		if ((gameStat->gameType & (Yonma | GuobiaoMJ)) && // 四麻である
			(gameStat->CurrentDiscard.tile < TileSuitHonors) && // 数牌で
			(gameStat->KangFlag.chankanFlag == chankanNone) && // 槍槓の判定中ではなくて
			(gameStat->CurrentPlayer.Active == ((gameStat->CurrentPlayer.Passive + 3) % 4))) { // 捨てたのが上家
				if ((gameStat->CurrentDiscard.tile >= 1) &&
					(TileCount[gameStat->CurrentDiscard.tile + 1] >= 1) && (TileCount[gameStat->CurrentDiscard.tile + 2] >= 1)) { // 下吃
						buttonEnabled[btnChii1] = true;
				}
				if ((gameStat->CurrentDiscard.tile >= 2) &&
					(TileCount[gameStat->CurrentDiscard.tile - 1] >= 1) && (TileCount[gameStat->CurrentDiscard.tile + 1] >= 1)) { // 嵌張吃
						buttonEnabled[btnChii2] = true;
				}
				if ((gameStat->CurrentDiscard.tile >= 3) &&
					(TileCount[gameStat->CurrentDiscard.tile - 2] >= 1) && (TileCount[gameStat->CurrentDiscard.tile - 1] >= 1)) { // 上吃
						buttonEnabled[btnChii3] = true;
				}
		}
	}

	if (buttonEnabled.any()) // 何らかのボタンが点灯していたら無視するボタンも点灯する
		buttonEnabled[btnPass] = true;

end:
	delete gameStat;
	reconstruct();
	return;
}

GameTableScreen::ButtonReconst::ButtonReconst(GameTableScreen* parent) {
	caller = parent;
	cursor = CursorDisabled; sunkenButton = NoSunkenButton;
	buttons = new ButtonPic(caller->caller->getDevice());
	ChangeButtonSet(btnSetNormal);
};

GameTableScreen::ButtonReconst::~ButtonReconst() {
	delete buttons;
}

/* ボタンが押された時の処理 */
void GameTableScreen::ButtonReconst::ButtonPressed() {
	auto setMode = [&](DiscardTileNum::discardType mode, ButtonID button, std::function<bool(int, GameTable*)> f) -> void {
		MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
		caller->tileSelectMode = mode;
		this->setSunkenButton(button);
		for (int i = 0; i < btnMAXIMUM; ++i)
			if (i != button)
				this->disable((ButtonID)i);
		this->reconstruct();
		caller->tehaiReconst->enable();
		for (int i = 0; i < NumOfTilesInHand; ++i) {
			GameTable tmpStat; memcpy(&tmpStat, GameStatus::gameStat(), sizeof (GameTable));
			if (f(i, &tmpStat)) caller->tehaiReconst->disable(i);
		}
		caller->tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->CurrentPlayer.Active);
	};

	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(reconstructionCS);
	sound::Play(sound::IDs::sndButton);
	if (!this->isEnabled((ButtonID)this->getCursor())) {
		sound::Play(sound::IDs::sndCuohu);
	} else if (this->getButtonSet() == btnSetTsumo) {
		auto isTenpaiTile = [](int i, GameTable* tmpStat) -> bool {
			tmpStat->Player[tmpStat->CurrentPlayer.Active].Hand[i].tile = NoTile;
			Shanten shanten = utils::calcShanten(tmpStat, tmpStat->CurrentPlayer.Active, shantenAll);
			return (shanten > 0);
		};
		switch (this->getCursor()) {
		case btnTsumo:
			caller->CallTsumoAgari();
			break;
		case btnKyuushu:
			caller->CallKyuushuKyuuhai();
			break;
		case btnRiichi: // 立直
			setMode(DiscardTileNum::Riichi, btnRiichi, isTenpaiTile);
			break;
		case btnOpenRiichi: // オープン立直
			setMode(DiscardTileNum::OpenRiichi, btnOpenRiichi, isTenpaiTile);
			break;
		case btnKan: // カン
			setMode(DiscardTileNum::Ankan, btnKan,
				[](int i, GameTable* tmpStat) -> bool {
					if ((tmpStat->statOfMine().RichiFlag.RichiFlag) && (i != TsumohaiIndex))
						return false; // リーチ時は自摸牌以外選択できないようにする
					bool flag = false;
					const PlayerID ActivePlayer = tmpStat->CurrentPlayer.Active;
					const Int8ByTile TileCount = utils::countTilesInHand(tmpStat, ActivePlayer);
					const PlayerTable* const playerStat = &(tmpStat->Player[ActivePlayer]);
					if (TileCount[playerStat->Hand[i].tile] < 4) flag = true;
					if (TileCount[playerStat->Hand[i].tile] == 1) {
						for (int j = 1; j <= playerStat->MeldPointer; ++j)
							if ((playerStat->Meld[j].tile == playerStat->Hand[i].tile) &&
								((playerStat->Meld[j].mstat == meldTripletExposedLeft) ||
								(playerStat->Meld[j].mstat == meldTripletExposedCenter) ||
								(playerStat->Meld[j].mstat == meldTripletExposedRight)))
								flag = false;
					}
					return flag;
				});
			break;
		case btnFlower: // 花牌
			setMode(DiscardTileNum::Flower, btnFlower,
				[](int i, GameTable* tmpStat) -> bool {
					if ((tmpStat->statOfMine().RichiFlag.RichiFlag) && (i != TsumohaiIndex))
						return false; // リーチ時は自摸牌以外選択できないようにする
					if (tmpStat->gameType & SanmaX)
						return tmpStat->Player[tmpStat->CurrentPlayer.Active].Hand[i].tile !=
							NorthWind;
					else
						return tmpStat->Player[tmpStat->CurrentPlayer.Active].Hand[i].tile <
							TileSuitFlowers;
				});
			break;
		default:
			sound::Play(sound::IDs::sndCuohu);
			break;
		}
	} else if (this->getButtonSet() == btnSetNormal) {
		switch (this->getCursor()) {
		case btnPass:
			ui::UIEvent->set(naki::nakiNone);
			break;
		case btnRon:
			ui::UIEvent->set(naki::nakiRon);
			break;
		case btnKan:
			ui::UIEvent->set(naki::nakiKan);
			break;
		case btnPon:
			ui::UIEvent->set(naki::nakiPon);
			break;
		case btnChii1:
			ui::UIEvent->set(naki::nakiChiLower);
			break;
		case btnChii2:
			ui::UIEvent->set(naki::nakiChiMiddle);
			break;
		case btnChii3:
			ui::UIEvent->set(naki::nakiChiUpper);
			break;
		default:
			sound::Play(sound::IDs::sndCuohu);
			break;
		}
	}
}

}
