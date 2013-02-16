#include "nakibtn.h"

#include "../../geometry.h"
#include "../../utils.h"
#include "../../gametbl.h"
#include "../../rule.h"
#include "../../event.h"
#include "../../../mjcore/discard.h"
#include "../../../sound/sound.h"
#include "../../../mjcore/bgmid.h"

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
			{_T(""),         5 + 117 * 0, Geometry::BaseSize - 40, 0xff666666},
			{_T(""),         5 + 117 * 1, Geometry::BaseSize - 40, 0xff666666},
			{_T("九種九牌"), 5 + 117 * 2, Geometry::BaseSize - 40, 0xff99ffff},
			{_T("花牌"),     5 + 117 * 3, Geometry::BaseSize - 40, 0xff99ccff},
			{_T("カン"),     5 + 117 * 4, Geometry::BaseSize - 40, 0xff9966ff},
			{_T("リーチ"),   5 + 117 * 5, Geometry::BaseSize - 40, 0xffff66ff},
			{_T("ツモ"),     5 + 117 * 6, Geometry::BaseSize - 40, 0xffffff66},
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
	if (!buttonEnabled[buttonID]) { // 暗転処理
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

void GameTableScreen::ButtonReconst::btnSetForDahai() { // ツモ番の時用の
	currentButtonSet = btnSetTsumo; buttonEnabled.reset(); // 状態をリセット
	const GameTable* const gameStat = GameStatus::retrGameStat();
	auto tilesMoreThan = [gameStat](int tiles) {
		return (gameStat->TilePointer + tiles) < (gameStat->RinshanPointer - gameStat->DeadTiles - 1);
	};
	const PLAYER_ID ActivePlayer = gameStat->CurrentPlayer.Active;
	const PlayerTable* const playerStat = &(gameStat->Player.val[ActivePlayer]);
	const SHANTEN Shanten = utils::calcShanten(gameStat, ActivePlayer, ShantenAnalyzer::shantenAll);
	if (utils::isRichiReqSatisfied(gameStat, ActivePlayer) && // 点棒要件を満たしている（点棒が足りている）
		(Shanten <= 0) && // テンパイしている
		(playerStat->MenzenFlag || (!rules::chkRule("riichi_shibari", "no"))) && // 門前であるか、リーチ縛りルールである
		(!playerStat->RichiFlag.RichiFlag) && // まだリーチしていない
		(tilesMoreThan((gameStat->gameType & AllSanma) ? 2 : 3))) // 残りツモ牌が十分あるなら
		buttonEnabled[btnRiichi] = true; // リーチボタンを有効に

	const bool DaoPaiAbilityFlag = utils::chkdaopaiability(gameStat, ActivePlayer);
	if ((DaoPaiAbilityFlag) && (playerStat->FirstDrawFlag))
		buttonEnabled[btnKyuushu] = true; // 九種九牌ボタン

	const bool ShisanBuDa = utils::chkShisanBuDa(gameStat, ActivePlayer);
	const bool ShisiBuDa = utils::chkShisiBuDa(gameStat, ActivePlayer);
	if (((Shanten <= -1) && (playerStat->Hand[NUM_OF_TILES_IN_HAND - 1].tile != NoTile)) || // 和了になっているか
		ShisanBuDa || ShisiBuDa) // 十三不塔の場合（十三不塔なしの場合この変数はfalseになる）
		buttonEnabled[btnTsumo] = true; // 和了ボタン

	const Int8ByTile TileCount = utils::countTilesInHand(gameStat, ActivePlayer);
	const int kanLim = rules::chkRule("fifth_kong", "no") ? 4 : 5;
	const bool KanFlag = [tilesMoreThan, gameStat, TileCount, playerStat, kanLim]() -> bool {
		for (int i = 1; i < TILE_NONFLOWER_MAX; ++i) {
			if (tilesMoreThan(0) && (gameStat->KangNum < kanLim)) {
				// 暗槓できる場合
				if (TileCount.val[i] == 4)
					return true;
				// 加槓できる場合
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

	const tileCode flowerTile = (gameStat->gameType & SanmaX) ? NorthWind : Flower;
	const bool Flowerabilityflag = [gameStat, playerStat]() -> bool {
		if (gameStat->gameType & SanmaX)
			return (playerStat->Hand[NUM_OF_TILES_IN_HAND].tile == NorthWind) && (!rules::chkRule("flower_tiles", "no"));
		else
			return playerStat->Hand[NUM_OF_TILES_IN_HAND].tile > TILE_SUIT_FLOWERS;
	} ();
	if ((!rules::chkRule("flower_tiles", "no")) &&
		(playerStat->Hand[NUM_OF_TILES_IN_HAND - 1].tile != NoTile) &&
		(TileCount.val[flowerTile] >= 1) &&
		((!playerStat->RichiFlag.RichiFlag) || Flowerabilityflag))
		buttonEnabled[btnFlower] = true;

	reconstruct();
}

void GameTableScreen::ButtonReconst::btnSetForNaki() { // 鳴きの時用の
	currentButtonSet = btnSetNormal; buttonEnabled.reset(); // 状態をリセット
	GameTable* const gameStat = new GameTable;
	memcpy(gameStat, GameStatus::retrGameStat(), sizeof (GameTable));
	auto tilesMoreThan = [gameStat](int tiles) {
		return (gameStat->TilePointer + tiles) < (gameStat->RinshanPointer - gameStat->DeadTiles - 1);
	};
	const PLAYER_ID ActivePlayer = gameStat->CurrentPlayer.Active;
	const PLAYER_ID PassivePlayer = gameStat->CurrentPlayer.Passive;
	PlayerTable* const playerStat = &(gameStat->Player.val[PassivePlayer]);
	playerStat->Hand[NUM_OF_TILES_IN_HAND - 1].tile = gameStat->CurrentDiscard.tile;
	const SHANTEN Shanten = utils::calcShanten(gameStat, gameStat->PlayerID, ShantenAnalyzer::shantenAll);
	playerStat->Hand[NUM_OF_TILES_IN_HAND - 1].tile = NoTile;

	if (gameStat->CurrentDiscard.tile > TILE_SUIT_FLOWERS) goto end; /* 花牌の場合は残りの判定をスキップ */
	if (playerStat->AgariHouki) goto end; /* 和了り放棄だったら残りの判定をスキップ */
	if ((gameStat->KangFlag.chankanFlag == chankanOfAnkan) && // 暗槓に対する搶槓判定のときで、
		(utils::calcShanten(gameStat, PassivePlayer, ShantenAnalyzer::shantenOrphans) >= 0)) // 国士聴牌でない場合は
		goto end; // 残りの判定をスキップ

	if (Shanten < 0) // 出た牌が当たり牌
		buttonEnabled[btnRon] = true; // 和了ボタン（フリテンの場合なども点灯）

	if (!playerStat->RichiFlag.RichiFlag) { // リーチしてないとき……
		const Int8ByTile TileCount = utils::countTilesInHand(gameStat, PassivePlayer);
		const int kanLim = rules::chkRule("fifth_kong", "no") ? 4 : 5;

		if (TileCount.val[gameStat->CurrentDiscard.tile] >= 2)
			buttonEnabled[btnPon] = true; // ポン
		if ((TileCount.val[gameStat->CurrentDiscard.tile] >= 3) && // 出てきた牌を暗刻で持っていて
			(gameStat->KangFlag.chankanFlag == chankanNone) && // 槍槓の判定中ではなくて
			(gameStat->KangNum < kanLim)) // 限度以内の場合
			buttonEnabled[btnKan] = true; // カン

		// チーできる条件：上家の捨牌であること、かつ、数牌であること
		if ((gameStat->gameType & Yonma) && // 四麻である
			(gameStat->CurrentDiscard.tile < TILE_SUIT_HONORS) && // 数牌で
			(gameStat->KangFlag.chankanFlag == chankanNone) && // 槍槓の判定中ではなくて
			(gameStat->CurrentPlayer.Active == ((gameStat->CurrentPlayer.Passive + 3) % 4))) { // 捨てたのが上家
				if ((gameStat->CurrentDiscard.tile >= 1) &&
					(TileCount.val[gameStat->CurrentDiscard.tile + 1] >= 1) && (TileCount.val[gameStat->CurrentDiscard.tile + 2] >= 1)) { // 下吃
						buttonEnabled[btnChii1] = true;
				}
				if ((gameStat->CurrentDiscard.tile >= 2) &&
					(TileCount.val[gameStat->CurrentDiscard.tile - 1] >= 1) && (TileCount.val[gameStat->CurrentDiscard.tile + 1] >= 1)) { // 嵌張吃
						buttonEnabled[btnChii2] = true;
				}
				if ((gameStat->CurrentDiscard.tile >= 3) &&
					(TileCount.val[gameStat->CurrentDiscard.tile - 2] >= 1) && (TileCount.val[gameStat->CurrentDiscard.tile - 1] >= 1)) { // 上吃
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

/* ボタンが押された時の処理 */
void GameTableScreen::ButtonReconst::ButtonPressed() {
	auto setMode = [&](DiscardTileNum::discardType mode, ButtonID button, std::function<bool(int, GameTable*)> f) -> void {
		caller->tileSelectMode = mode;
		this->setSunkenButton(button);
		for (int i = 0; i < btnMAXIMUM; ++i)
			if (i != button)
				this->disable((ButtonID)i);
		this->reconstruct();
		caller->tehaiReconst->enable();
		for (int i = 0; i < NUM_OF_TILES_IN_HAND; ++i) {
			GameTable tmpStat; std::memcpy(&tmpStat, GameStatus::gameStat(), sizeof (GameTable));
			if (f(i, &tmpStat)) caller->tehaiReconst->disable(i);
		}
		caller->tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->CurrentPlayer.Active);
	};

	sound::Play(sound::IDs::sndButton);
	if (!this->isEnabled((ButtonID)this->getCursor())) {
		sound::Play(sound::IDs::sndCuohu);
	} else if (this->getButtonSet() == btnSetTsumo) {
		switch (this->getCursor()) {
		case btnTsumo:
			caller->CallTsumoAgari();
			break;
		case btnKyuushu:
			caller->CallKyuushuKyuuhai();
			break;
		case btnRiichi: // 立直
			setMode(DiscardTileNum::Riichi, btnRiichi,
				[](int i, GameTable* tmpStat) -> bool {
					tmpStat->Player.val[tmpStat->CurrentPlayer.Active].Hand[i].tile = NoTile;
					SHANTEN Shanten = utils::calcShanten(tmpStat, tmpStat->CurrentPlayer.Active, ShantenAnalyzer::shantenAll);
					return (Shanten > 0);
				});
			break;
		case btnKan: // カン
			setMode(DiscardTileNum::Ankan, btnKan,
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
		case btnFlower: // 花牌
			setMode(DiscardTileNum::Flower, btnFlower,
				[](int i, GameTable* tmpStat) -> bool {
					return tmpStat->Player.val[tmpStat->CurrentPlayer.Active].Hand[i].tile !=
						(tmpStat->gameType & SanmaX) ? NorthWind : Flower;
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
