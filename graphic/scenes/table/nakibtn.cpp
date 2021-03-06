﻿#include <cmath>
#include <cassert>
#include "../../pi.h"

#include "nakibtn.h"

#include "../../geometry.h"
#include "../../utils.h"
#include "../../gametbl.h"
#include "../../rule.h"
#include "../../event.h"
#include "../../../sound/sound.h"
#include "../../../common/bgmid.h"

#include "tehai.h"
#include "naki_id.h"

namespace mihajong_graphic {

namespace {

template <typename T> T scaleToWindow(T val) {
	return static_cast<T>(static_cast<float>(val) * Geometry::WindowScale());
}

}

constexpr GameTableScreen::ButtonReconst::BtnData
GameTableScreen::ButtonReconst::buttonDat[2][btnMAXIMUM] = {
		{
			{_T(""),         5 + 117 * 0, Geometry::BaseSize - 40, 0xff666666},
			{_T(""),         5 + 117 * 1, Geometry::BaseSize - 40, 0xff666666},
			{_T("チー"),     5 + 117 * 2, Geometry::BaseSize - 40, 0xffccff66},
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

ButtonSet GameTableScreen::ButtonReconst::getButtonSet() {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	return currentButtonSet;
}
GameTableScreen::ButtonReconst::ButtonFlags GameTableScreen::ButtonReconst::areEnabled() {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	return buttonEnabled;
}
bool GameTableScreen::ButtonReconst::isEnabled(ButtonID buttonID) {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	return (static_cast<int>(buttonID) >= 0) && (static_cast<int>(buttonID) < btnMAXIMUM) && buttonEnabled[buttonID];
}
bool GameTableScreen::ButtonReconst::isSunkenButtonExists() {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	return sunkenButton != NoSunkenButton;
}
ButtonID GameTableScreen::ButtonReconst::getSunkenButtonID() {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	return sunkenButton;
}
void GameTableScreen::ButtonReconst::setSunkenButton(ButtonID buttonID) {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	sunkenButton = buttonID;
}
bool GameTableScreen::ButtonReconst::isCursorEnabled() {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	return cursor != CursorDisabled;
}
ButtonID GameTableScreen::ButtonReconst::getCursor() {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	return cursor;
}
void GameTableScreen::ButtonReconst::setCursor(ButtonID cursorPos) {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	cursor = cursorPos;
}
ButtonID GameTableScreen::ButtonReconst::incCursor() {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	return static_cast<ButtonID>(++(*reinterpret_cast<int*>(&cursor)));
}
ButtonID GameTableScreen::ButtonReconst::decCursor() {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	return static_cast<ButtonID>(--(*reinterpret_cast<int*>(&cursor)));
}

void GameTableScreen::ButtonReconst::Render() {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
#ifndef _WIN32
	if (!initialized) {
		reconstruct();
		initialized = true;
	}
#endif /*_WIN32*/
	if (cursor != CursorDisabled) {
#include "color.h"
		Color btnColor; btnColor.rgbaAsOneValue = getButtonData(currentButtonSet, cursor).color;
		const double Zeit = static_cast<double>(myTimer.currTime() % 9000000ULL);
		btnColor.rgbaAsStruct.r = static_cast<unsigned>(static_cast<double>(btnColor.rgbaAsStruct.r) * (sin(Zeit / 450000.0 * M_PI) / 4.0 + 0.75));
		btnColor.rgbaAsStruct.g = static_cast<unsigned>(static_cast<double>(btnColor.rgbaAsStruct.g) * (sin(Zeit / 450000.0 * M_PI) / 4.0 + 0.75));
		btnColor.rgbaAsStruct.b = static_cast<unsigned>(static_cast<double>(btnColor.rgbaAsStruct.b) * (sin(Zeit / 450000.0 * M_PI) / 4.0 + 0.75));
		buttons->setButton(static_cast<int>(cursor),
			(sunkenButton == cursor) ? ButtonStat::sunken : (buttonEnabled[cursor] ? ButtonStat::raised : ButtonStat::clear),
			scaleToWindow(getButtonData(currentButtonSet, cursor).x),
			scaleToWindow(getButtonData(currentButtonSet, cursor).y),
			scaleToWindow(117u), scaleToWindow(36u),
			btnColor.rgbaAsOneValue, getButtonData(currentButtonSet, cursor).label);
	}
	buttons->Render();
	buttons->Render();
	buttons->Render();
}

void GameTableScreen::ButtonReconst::reconstruct(ButtonID buttonID) {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
#include "color.h"
	Color btnColor; btnColor.rgbaAsOneValue = getButtonData(currentButtonSet, buttonID).color;
	/*if (!buttonEnabled[buttonID]) { // 暗転処理
		btnColor.rgbaAsStruct.r /= 3;
		btnColor.rgbaAsStruct.g /= 3;
		btnColor.rgbaAsStruct.b /= 3;
	}*/
	buttons->setButton(static_cast<int>(buttonID),
		(sunkenButton == buttonID) ? ButtonStat::sunken : (buttonEnabled[buttonID] ? ButtonStat::raised : ButtonStat::clear),
		scaleToWindow(getButtonData(currentButtonSet, buttonID).x),
		scaleToWindow(getButtonData(currentButtonSet, buttonID).y),
		scaleToWindow(117u), scaleToWindow(36u),
		btnColor.rgbaAsOneValue, getButtonData(currentButtonSet, buttonID).label);
	caller->setRegion(static_cast<int>(buttonID) + ButtonRegionNum,
		getButtonData(currentButtonSet, buttonID).x      , getButtonData(currentButtonSet, buttonID).y,
		getButtonData(currentButtonSet, buttonID).x + 117, getButtonData(currentButtonSet, buttonID).y + 36);
}
void GameTableScreen::ButtonReconst::reconstruct() {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	for (unsigned i = 0; i < btnMAXIMUM; ++i)
		reconstruct(static_cast<ButtonID>(i));
}

void GameTableScreen::ButtonReconst::ChangeButtonSet(ButtonSet btnSet) {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
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
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	buttonEnabled[buttonID] = true; reconstruct(buttonID);
}
void GameTableScreen::ButtonReconst::disable(ButtonID buttonID) {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	buttonEnabled[buttonID] = false; reconstruct(buttonID);
}
void GameTableScreen::ButtonReconst::enable(const std::bitset<btnMAXIMUM>& flagset) {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	buttonEnabled = reinterpret_cast<const ButtonFlags&>(flagset);
	reconstruct();
}

void GameTableScreen::ButtonReconst::btnSetForDahai() { // ツモ番の時用の
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	currentButtonSet = ButtonSet::tsumo; buttonEnabled.reset(); // 状態をリセット
	const GameTable* const gameStat = GameStatus::retrGameStat();
	auto tilesMoreThan = [gameStat](int tiles) {
		return (gameStat->TilePointer + tiles) < (gameStat->RinshanPointer - gameStat->DeadTiles - 1);
	};
	const PlayerID ActivePlayer = gameStat->CurrentPlayer.Active;
	const PlayerTable* const playerStat = &(gameStat->Player[ActivePlayer]);
	const Shanten shanten = utils::calcShanten(gameStat, ActivePlayer, ShantenType::all);
	if (utils::isRichiReqSatisfied(gameStat, ActivePlayer) && // 点棒要件を満たしている（点棒が足りている）
		(shanten <= 0) && // テンパイしている
		gameStat->chkGameType(GameTypeID::richiMJ) && // 日麻である
		(playerStat->MenzenFlag || (!rules::chkRule("riichi_shibari", "no"))) && // 門前であるか、リーチ縛りルールである
		(!playerStat->RichiFlag.RichiFlag) && // まだリーチしていない
		(tilesMoreThan(gameStat->chkGameType(GameTypeID::allSanma) ? 2 : 3))) { // 残りツモ牌が十分あるなら
			buttonEnabled[ButtonID::riichi] = true; // リーチボタンを有効に
			if ((!rules::chkRule("open_riichi", "no")) && playerStat->MenzenFlag) // オープンリーチありの場合で門前の場合
				buttonEnabled[ButtonID::openRiichi] = true; // ボタンを有効に
	}

	const bool DaoPaiAbilityFlag = gameStat->chkGameType(GameTypeID::richiMJ) && utils::chkdaopaiability(gameStat, ActivePlayer);
	if (gameStat->chkGameType(GameTypeID::richiMJ) && (DaoPaiAbilityFlag) && (playerStat->FirstDrawFlag))
		buttonEnabled[ButtonID::kyuushu] = true; // 九種九牌ボタン

	const bool ShisanBuDa = gameStat->chkGameType(GameTypeID::richiMJ) && utils::chkShisanBuDa(gameStat, ActivePlayer);
	const bool ShisiBuDa = gameStat->chkGameType(GameTypeID::richiMJ) && utils::chkShisiBuDa(gameStat, ActivePlayer);
	if (((shanten <= -1) && (playerStat->Tsumohai())) || // 和了になっているか
		ShisanBuDa || ShisiBuDa) // 十三不塔の場合（十三不塔なしの場合この変数はfalseになる）
		buttonEnabled[ButtonID::tsumo] = true; // 和了ボタン

	const Int8ByTile TileCount = utils::countTilesInHand(gameStat, ActivePlayer);
	const int kanLim = gameStat->chkGameType(GameTypeID::guobiaoMJ) ? 16 : rules::chkRule("fifth_kong", "no") ? 4 : 5;
	const bool KanFlag = [tilesMoreThan, gameStat, TileCount, playerStat, kanLim]() -> bool {
		for (int i = 1; i < TileNonflowerMax; ++i) {
			if (tilesMoreThan(0) && (gameStat->KangNum < kanLim)) {
				// 暗槓できる場合
				if (TileCount[i] == 4)
					return true;
				// 加槓できる場合
				else if (TileCount[i] == 1) {
					for (int j = 1; j <= playerStat->MeldPointer; ++j)
						if ((playerStat->Meld[j].tile == static_cast<TileCode>(i)) &&
							((playerStat->Meld[j].mstat == MeldStat::tripletExposedLeft) ||
							(playerStat->Meld[j].mstat == MeldStat::tripletExposedCenter) ||
							(playerStat->Meld[j].mstat == MeldStat::tripletExposedRight)))
							return true;
				}
			}
		}
		return false;
	} ();
	if ((KanFlag && (playerStat->Tsumohai()) &&
		(!playerStat->RichiFlag.RichiFlag)) || utils::chkAnkanAbility(gameStat, ActivePlayer))
		buttonEnabled[ButtonID::kan] = true;

	const TileCode flowerTile = gameStat->chkGameType(GameTypeID::sanmaX) ? TileCode::northWind : TileCode::flower;
	const bool Flowerabilityflag = [gameStat, playerStat]() -> bool {
		if (gameStat->chkGameType(GameTypeID::sanmaX))
			return (playerStat->Tsumohai().tile == TileCode::northWind) && (!rules::chkRule("flower_tiles", "no"));
		else
			return playerStat->Tsumohai().isFlower();
	} ();
	if ((gameStat->chkGameType(GameTypeID::guobiaoMJ) || (!rules::chkRule("flower_tiles", "no"))) &&
		(playerStat->Tsumohai()) &&
		(TileCount[flowerTile] >= 1) &&
		((!playerStat->RichiFlag.RichiFlag) || Flowerabilityflag))
		buttonEnabled[ButtonID::flower] = true;

	reconstruct();
}

void GameTableScreen::ButtonReconst::btnSetForNaki() { // 鳴きの時用の
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	currentButtonSet = ButtonSet::normal; buttonEnabled.reset(); // 状態をリセット
	GameTable* const gameStat = new GameTable;
	memcpy(gameStat, GameStatus::retrGameStat(), sizeof (GameTable));
	auto tilesMoreThan = [gameStat](int tiles) {
		return (gameStat->TilePointer + tiles) < (gameStat->RinshanPointer - gameStat->DeadTiles - 1);
	};
	const PlayerID ActivePlayer = gameStat->CurrentPlayer.Active;
	const PlayerID PassivePlayer = gameStat->CurrentPlayer.Passive;
	PlayerTable* const playerStat = &(gameStat->Player[PassivePlayer]);
	playerStat->Tsumohai().tile = gameStat->CurrentDiscard.tile;
	const Shanten shanten = utils::calcShanten(gameStat, gameStat->PlayerID, ShantenType::all);
	playerStat->Tsumohai().tile = TileCode::noTile;

	if (gameStat->CurrentDiscard.isFlower()) goto end; /* 花牌の場合は残りの判定をスキップ */
	if (playerStat->AgariHouki) goto end; /* 和了り放棄だったら残りの判定をスキップ */
	if ((gameStat->KangFlag.chankanFlag == ChankanStat::ankan) && // 暗槓に対する搶槓判定のときで、
		gameStat->chkGameType(GameTypeID::richiMJ) && // 中国ルール以外で
		(utils::calcShanten(gameStat, PassivePlayer, ShantenType::orphans) >= 0)) // 国士聴牌でない場合は
		goto end; // 残りの判定をスキップ

	if (shanten < 0) // 出た牌が当たり牌
		buttonEnabled[ButtonID::ron] = true; // 和了ボタン（フリテンの場合なども点灯）

	if ((!playerStat->RichiFlag.RichiFlag) && tilesMoreThan(0)) { // リーチしてなくて河底でないとき……
		const Int8ByTile TileCount = utils::countTilesInHand(gameStat, PassivePlayer);
		const int kanLim = gameStat->chkGameType(GameTypeID::guobiaoMJ) ? 16 : rules::chkRule("fifth_kong", "no") ? 4 : 5;

		if (TileCount[gameStat->CurrentDiscard.tile] >= 2)
			buttonEnabled[ButtonID::pon] = true; // ポン
		if ((TileCount[gameStat->CurrentDiscard.tile] >= 3) && // 出てきた牌を暗刻で持っていて
			(gameStat->KangFlag.chankanFlag == ChankanStat::none) && // 槍槓の判定中ではなくて
			(gameStat->KangNum < kanLim)) // 限度以内の場合
			buttonEnabled[ButtonID::kan] = true; // カン

		// チーできる条件：上家の捨牌であること、かつ、数牌であること
		if ((gameStat->chkGameType(GameTypeID::yonma) || gameStat->chkGameType(GameTypeID::guobiaoMJ)) && // 四麻である
			(gameStat->CurrentDiscard.isNumber()) && // 数牌で
			(gameStat->KangFlag.chankanFlag == ChankanStat::none) && // 槍槓の判定中ではなくて
			(gameStat->CurrentPlayer.Active == ((gameStat->CurrentPlayer.Passive + 3) % 4))) { // 捨てたのが上家
				if ((gameStat->CurrentDiscard.tile >= static_cast<TileCode>(1)) &&
					(TileCount[offsetTileNumber(gameStat->CurrentDiscard.tile, +1)] >= 1) && (TileCount[offsetTileNumber(gameStat->CurrentDiscard.tile, +2)] >= 1)) { // 下吃
						buttonEnabled[ButtonID::chii] = true;
				}
				if ((gameStat->CurrentDiscard.tile >= static_cast<TileCode>(2)) &&
					(TileCount[offsetTileNumber(gameStat->CurrentDiscard.tile, -1)] >= 1) && (TileCount[offsetTileNumber(gameStat->CurrentDiscard.tile, +1)] >= 1)) { // 嵌張吃
						buttonEnabled[ButtonID::chii] = true;
				}
				if ((gameStat->CurrentDiscard.tile >= static_cast<TileCode>(3)) &&
					(TileCount[offsetTileNumber(gameStat->CurrentDiscard.tile, -2)] >= 1) && (TileCount[offsetTileNumber(gameStat->CurrentDiscard.tile, -1)] >= 1)) { // 上吃
						buttonEnabled[ButtonID::chii] = true;
				}
		}
	}

	if (buttonEnabled.any()) // 何らかのボタンが点灯していたら無視するボタンも点灯する
		buttonEnabled[ButtonID::pass] = true;

end:
	delete gameStat;
	reconstruct();
	return;
}

GameTableScreen::ButtonReconst::ButtonReconst(GameTableScreen* parent) {
	caller = parent;
	cursor = CursorDisabled; sunkenButton = NoSunkenButton;
	currentButtonSet = ButtonSet::normal;
	buttons = new ButtonPic(caller->caller->getDevice());
	ChangeButtonSet(ButtonSet::normal);
};

GameTableScreen::ButtonReconst::~ButtonReconst() {
	delete buttons;
}

/* モード設定 */
void GameTableScreen::ButtonReconst::setMode(DiscardType mode, ButtonID button, std::function<bool(int, GameTable*)> f) {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	caller->tileSelectMode = mode;
	this->setSunkenButton(button);
	for (int i = 0; i < btnMAXIMUM; ++i)
		if (i != static_cast<int>(button))
			this->disable(static_cast<ButtonID>(i));
	this->reconstruct();
	caller->tehaiReconst->enable();
	for (int i = 0; i < NumOfTilesInHand; ++i) {
		GameTable tmpStat(*GameStatus::gameStat());
		if ((i == caller->tehaiReconst->getFirstChosenTile()) || f(i, &tmpStat))
			caller->tehaiReconst->disable(i);
	}
	caller->tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->CurrentPlayer.Active);
}

/* ボタンが押された時の処理 */
void GameTableScreen::ButtonReconst::ButtonPressed() {
	std::unique_lock<std::recursive_mutex> lock(reconstructionCS);
	sound::Play(sound::IDs::sndButton);
	if (!this->isEnabled(static_cast<ButtonID>(this->getCursor()))) {
		sound::Play(sound::IDs::sndCuohu);
	} else if (this->getButtonSet() == ButtonSet::tsumo) {
		auto isTenpaiTile = [](int i, GameTable* tmpStat) -> bool {
			tmpStat->statOfActive().Hand[i].tile = TileCode::noTile;
			Shanten shanten = utils::calcShanten(tmpStat, tmpStat->CurrentPlayer.Active, ShantenType::all);
			return (shanten > 0);
		};
		switch (this->getCursor()) {
		case ButtonID::tsumo:
			caller->CallTsumoAgari();
			break;
		case ButtonID::kyuushu:
			caller->CallKyuushuKyuuhai();
			break;
		case ButtonID::riichi: // 立直
			setMode(DiscardType::riichi, ButtonID::riichi, isTenpaiTile);
			break;
		case ButtonID::openRiichi: // オープン立直
			setMode(DiscardType::openRiichi, ButtonID::openRiichi, isTenpaiTile);
			break;
		case ButtonID::kan: // カン
			setMode(DiscardType::ankan, ButtonID::kan,
				[](int i, GameTable* tmpStat) -> bool {
					if (tmpStat->statOfActive().RichiFlag.RichiFlag)
						return (i != TsumohaiIndex); // リーチ時は自摸牌以外選択できないようにする
					bool flag = false;
					const PlayerID ActivePlayer = tmpStat->CurrentPlayer.Active;
					const Int8ByTile TileCount = utils::countTilesInHand(tmpStat, ActivePlayer);
					const PlayerTable& playerStat = tmpStat->statOfActive();
					if (TileCount[playerStat.Hand[i].tile] < 4) flag = true;
					if (TileCount[playerStat.Hand[i].tile] == 1) {
						for (int j = 1; j <= playerStat.MeldPointer; ++j)
							if ((playerStat.Meld[j].tile == playerStat.Hand[i].tile) &&
								((playerStat.Meld[j].mstat == MeldStat::tripletExposedLeft) ||
								(playerStat.Meld[j].mstat == MeldStat::tripletExposedCenter) ||
								(playerStat.Meld[j].mstat == MeldStat::tripletExposedRight)))
								flag = false;
					}
					return flag;
				});
			break;
		case ButtonID::flower: // 花牌
			setMode(DiscardType::flower, ButtonID::flower,
				[](int i, GameTable* tmpStat) -> bool {
					if (tmpStat->statOfActive().RichiFlag.RichiFlag)
						return (i != TsumohaiIndex); // リーチ時は自摸牌以外選択できないようにする
					if (tmpStat->chkGameType(GameTypeID::sanmaX))
						return tmpStat->statOfActive().Hand[i].tile != TileCode::northWind;
					else
						return !tmpStat->statOfActive().Hand[i].isFlower();
				});
			break;
		default:
			sound::Play(sound::IDs::sndCuohu);
			break;
		}
	} else if (this->getButtonSet() == ButtonSet::normal) {
		const auto tilesInHand = utils::countTilesInHand(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
		switch (this->getCursor()) {
		case ButtonID::pass:
			ui::UIEvent->set(naki::nakiNone);
			break;
		case ButtonID::ron:
			ui::UIEvent->set(naki::nakiRon);
			break;
		case ButtonID::kan:
			ui::UIEvent->set(naki::nakiKan);
			break;
		case ButtonID::pon:
			if (tilesInHand[GameStatus::gameStat()->CurrentDiscard.tile] == 2) {
				ui::UIEvent->set(naki::nakiPon);
			} else if (caller->countTiles([](TileCode p, TileCode q) {return p == q;}) > 1) {
				setMode(DiscardType::meldSel, ButtonID::pon,
					[](int i, GameTable* tmpStat) -> bool {
						return tmpStat->statOfMine().Hand[i].tile != tmpStat->CurrentDiscard.tile;
					});
				caller->tehaiReconst->setTileCursor(0), setCursor();
				caller->tehaiReconst->Render();
			} else {
				ui::UIEvent->set(naki::nakiPon);
			}
			break;
		case ButtonID::chii:
			assert(GameStatus::gameStat()->CurrentDiscard.isNumber());
			{
				const auto chiiable = [&tilesInHand](TileCode p, TileCode q) {
					if (getTileSuit(p) != getTileSuit(q))
						return false;
					else if ((p == offsetTileNumber(q, +2)) && (tilesInHand[offsetTileNumber(q, 1)] > 0)) return true;
					else if  (p == offsetTileNumber(q, +1))                                               return true;
					else if  (p == offsetTileNumber(q, -1))                                               return true;
					else if ((p == offsetTileNumber(q, -2)) && (tilesInHand[offsetTileNumber(p, 1)] > 0)) return true;
					else                                               return false;
				};
				if (caller->countTiles(chiiable) > 2) {
					setMode(DiscardType::meldSel, ButtonID::chii,
						[chiiable](int i, GameTable* tmpStat) -> bool {
						return !chiiable(tmpStat->statOfMine().Hand[i].tile, tmpStat->CurrentDiscard.tile);
					});
					caller->tehaiReconst->setTileCursor(0), setCursor();
					caller->tehaiReconst->Render();
				} else {
					const auto discard = GameStatus::gameStat()->CurrentDiscard.tile;
					if ((getTileNumber(discard) > 2) && (tilesInHand[offsetTileNumber(discard, -2)] > 0)) {
						ui::UIEvent->set(naki::nakiChiUpper);
					} else if ((getTileNumber(discard) < 8) && (tilesInHand[offsetTileNumber(discard, 2)] > 0)) {
						ui::UIEvent->set(naki::nakiChiLower);
					} else {
						ui::UIEvent->set(naki::nakiChiMiddle);
					}
				}
			}
			break;
		default:
			sound::Play(sound::IDs::sndCuohu);
			break;
		}
	}
}

}
