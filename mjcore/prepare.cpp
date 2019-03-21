#include "prepare.h"

#include <cstdint>
#include <algorithm>
#include <functional>
#ifndef _WIN32
#include <unistd.h>
#endif /*_WIN32*/
#include "../socket/socket.h"
#include "envtbl.h"
#include "random.h"
#include "haifu.h"
#include "tileutil.h"
#include "yaku/yaku.h"
#include "ai/class.h"
#include "../sound/sound.h"
#include "../common/bgmid.h"
#include "sound.h"
#include "random.h"
#include "../graphic/graphic.h"
#include "ruletbl.h"
#include "../common/sleep.h"

inline unsigned int inittiles(GameTable* const gameStat, UInt8ByTile& tilepos) { // 牌を並べる
	const auto settile = [gameStat, &tilepos](TileCode tCode, unsigned int& pos) {
		tilepos[tCode] = pos;
		for (unsigned int i = 0; i < 4u; ++i) {
			gameStat->Deck[pos++].tile = tCode;
		}
	};
	unsigned int p = 0; // 牌の位置ID
	if (gameStat->chkGameType(SanmaX)) {
		settile(CharacterOne, p); // 萬子
		settile(CharacterNine, p); // 萬子
	} else {
		for (unsigned int k = 1u; k <= 9u; ++k)
			settile(static_cast<TileCode>(TileSuitCharacters + k), p); // 萬子
	}
	for (unsigned int k = 1u; k <= 9u; ++k)
		settile(static_cast<TileCode>(TileSuitCircles + k), p); // 筒子
	if (gameStat->chkGameType(SanmaSeto)) {
		settile(BambooOne, p); // 索子
		settile(BambooNine, p); // 索子
	} else {
		for (unsigned int k = 1u; k <= 9u; ++k)
			settile(static_cast<TileCode>(TileSuitBamboos + k), p); // 索子
	}
	if (!gameStat->chkGameType(SanmaS)) {
		for (unsigned int k = 1u; k <= 7u; ++k)
			settile(static_cast<TileCode>(TileSuitHonors + k), p); // 字牌
#ifndef GUOBIAO
		if (RuleData::chkRule("flower_tiles", "seasons") || RuleData::chkRule("flower_tiles", "8tiles")) {
#endif /* GUOBIAO */
			gameStat->Deck[p++].tile = Spring; gameStat->Deck[p++].tile = Summer;
			gameStat->Deck[p++].tile = Autumn; gameStat->Deck[p++].tile = Winter;
#ifndef GUOBIAO
		}
		if (RuleData::chkRule("flower_tiles", "flowers") || RuleData::chkRule("flower_tiles", "8tiles")) {
#endif /* GUOBIAO */
			gameStat->Deck[p++].tile = Plum; gameStat->Deck[p++].tile = Orchid;
			gameStat->Deck[p++].tile = Chrysanthemum; gameStat->Deck[p++].tile = Bamboo;
#ifndef GUOBIAO
		}
#endif /* GUOBIAO */
	}
	return p;
}

#ifndef GUOBIAO
inline void redtiles(GameTable* const gameStat, UInt8ByTile& tilepos) { // 赤ドラを設定する
	{
		constexpr char tileRules[9][16] = {
			"red_one", "red_two", "red_three", "red_four", "red_five",
			"red_six", "red_seven", "red_eight", "red_nine",
		};
		for (unsigned int i = 0; i < 9; ++i) { // 数牌の赤ドラ
			if (RuleData::chkRule(tileRules[i], "character_suit") || // 萬子
				RuleData::chkRule(tileRules[i], "character_circle_suit") ||
				RuleData::chkRule(tileRules[i], "character_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TileSuitCharacters + i + 1]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "circle_suit") || // 筒子
				RuleData::chkRule(tileRules[i], "character_circle_suit") ||
				RuleData::chkRule(tileRules[i], "circle_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TileSuitCircles + i + 1]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "bamboo_suit") || // 索子
				RuleData::chkRule(tileRules[i], "circle_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "character_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TileSuitBamboos + i + 1]].red = AkaDora;
		}
	}
	// 5のドラは特殊
	if (gameStat->chkGameType(SanmaX)) { // 三麻
		if (RuleData::chkRule("red_five", "2tiles")) {
			gameStat->Deck[tilepos[CircleFive]].red = AkaDora;
			gameStat->Deck[tilepos[BambooFive]].red = AkaDora;
		}
		else if (RuleData::chkRule("red_five", "4tiles")) {
			gameStat->Deck[tilepos[CircleFive]].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive] + 1].red = AkaDora;
			gameStat->Deck[tilepos[BambooFive]].red = AkaDora;
			gameStat->Deck[tilepos[BambooFive] + 1].red = AkaDora;
		}
		else if (RuleData::chkRule("red_five", "2_circle_tiles")) {
			gameStat->Deck[tilepos[CircleFive]].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive] + 1].red = AkaDora;
		}
	} else { // 四麻・数牌三麻
		if (RuleData::chkRule("red_five", "3tiles")) {
			gameStat->Deck[tilepos[CharacterFive]].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive]].red = AkaDora;
			gameStat->Deck[tilepos[BambooFive]].red = AkaDora;
		}
		else if (RuleData::chkRule("red_five", "4tiles")) {
			gameStat->Deck[tilepos[CharacterFive]].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive]].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive] + 1].red = AkaDora;
			gameStat->Deck[tilepos[BambooFive]].red = AkaDora;
		}
		else if (RuleData::chkRule("red_five", "6tiles")) {
			gameStat->Deck[tilepos[CharacterFive]].red = AkaDora;
			gameStat->Deck[tilepos[CharacterFive] + 1].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive]].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive] + 1].red = AkaDora;
			gameStat->Deck[tilepos[BambooFive]].red = AkaDora;
			gameStat->Deck[tilepos[BambooFive] + 1].red = AkaDora;
		}
		else if (RuleData::chkRule("red_five", "2_circle_tiles")) {
			gameStat->Deck[tilepos[CircleFive]].red = AkaDora;
			gameStat->Deck[tilepos[CircleFive] + 1].red = AkaDora;
		}
	}
	{
		constexpr char tileRules[2][16] = {"red_west", "red_north",};
		constexpr TileCode tileCodes[2] = {WestWind, NorthWind,};
		for (unsigned int i = 0; i < 2; ++i) { // 字牌の赤ドラ
			if (RuleData::chkRule(tileRules[i], "4tiles") || RuleData::chkRule(tileRules[i], "3tiles") ||
				RuleData::chkRule(tileRules[i], "2tiles") || RuleData::chkRule(tileRules[i], "1tile"))
				gameStat->Deck[tilepos[tileCodes[i]]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "4tiles") || RuleData::chkRule(tileRules[i], "3tiles") ||
				RuleData::chkRule(tileRules[i], "2tiles"))
				gameStat->Deck[tilepos[tileCodes[i]] + 1].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "4tiles") || RuleData::chkRule(tileRules[i], "3tiles"))
				gameStat->Deck[tilepos[tileCodes[i]] + 2].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "4tiles"))
				gameStat->Deck[tilepos[tileCodes[i]] + 3].red = AkaDora;
		}
	}
	{
		if (RuleData::chkRule("haku_potchi", "1tile_dora")) // オールマイティー扱いはせず、赤ドラと同じ扱い
			gameStat->Deck[tilepos[WhiteDragon]].red = AkaDora; // 白ポッチ
	}
}

inline void bluetiles(GameTable* const gameStat, UInt8ByTile& tilepos) { // 青ドラを設定する
	{
		constexpr char tileRules[3][16] = {"blue_one", "blue_five", "blue_nine",};
		constexpr unsigned int tileNum[3] = {1, 5, 9,};
		for (unsigned int i = 0; i < 3; ++i) { // 数牌の青ドラ
			if (RuleData::chkRule(tileRules[i], "character_suit") || // 萬子
				RuleData::chkRule(tileRules[i], "character_circle_suit") ||
				RuleData::chkRule(tileRules[i], "character_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TileSuitCharacters + tileNum[i]] + 3].red = AoDora;
			if (RuleData::chkRule(tileRules[i], "circle_suit") || // 筒子
				RuleData::chkRule(tileRules[i], "character_circle_suit") ||
				RuleData::chkRule(tileRules[i], "circle_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TileSuitCircles + tileNum[i]] + 3].red = AoDora;
			if (RuleData::chkRule(tileRules[i], "bamboo_suit") || // 索子
				RuleData::chkRule(tileRules[i], "circle_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "character_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TileSuitBamboos + tileNum[i]] + 3].red = AoDora;
		}
	}
	{
		if (RuleData::chkRule("blue_lvfa", "yes"))
			gameStat->Deck[tilepos[GreenDragon] + 3].red = AoDora;
	}
}
#endif /* GUOBIAO */

inline void shuffletiles(GameTable* const gameStat, UInt8ByTile& tilepos, unsigned int tiles) { // 洗牌する
	if (EnvTable::Instantiate()->GameMode != EnvTable::Client) {
		for (unsigned int i = 0; i < tiles; ++i) { // 洗牌する
			unsigned int tmppos = i + RndNum::rnd(tiles - i);
			Tile tmptile = gameStat->Deck[tmppos];
			gameStat->Deck[tmppos] = gameStat->Deck[i];
			gameStat->Deck[i] = tmptile;
		}
	}
}

void shuffle(GameTable* const gameStat) { // 牌をバッファに並べて、洗牌
	UInt8ByTile tilepos;
	unsigned int tiles = inittiles(gameStat, tilepos);
#ifndef GUOBIAO
	redtiles(gameStat, tilepos);
	bluetiles(gameStat, tilepos);
#endif /* GUOBIAO */
	shuffletiles(gameStat, tilepos, tiles);
}

#ifndef GUOBIAO
#define nagatadora(TileCode) {++gameStat->DoraFlag.Omote[TileCode]; haifu::haifurecdora(TileCode);}
inline void DoraAdding(GameTable* const gameStat) {
	setdora(gameStat, 0); // 表ドラを設定する
	if (RuleData::chkRuleApplied("uradora"))
		setdora(gameStat, 1); // 裏ドラを設定する
}

void initdora(GameTable* const gameStat) { // ドラの設定
	if (gameStat->chkGameType(AllSanma))
		gameStat->DoraPointer = 102 - gameStat->ExtraRinshan; // ドラ表示牌のポインタ
	else gameStat->DoraPointer = 130; // ドラ表示牌のポインタ
	if (RuleData::chkRuleApplied("nagatacho")) { // 永田町ルール
		nagatadora(BambooSeven); // 七索は常にドラ
		for (int i = 0; i <= (RuleData::chkRule("dice_roll", "roll_twice") ? 0 : 2); i += 2) {
			unsigned int dice = gameStat->Dice[i + 0].Number + gameStat->Dice[i + 1].Number;
			if (dice <= 8) { // 2〜8はその数牌がドラ　三麻では萬子がないので別処理
				if (!gameStat->chkGameType(SanmaX))
					nagatadora(static_cast<TileCode>(TileSuitCharacters + dice));
				nagatadora(    static_cast<TileCode>(TileSuitCircles    + dice));
				nagatadora(    static_cast<TileCode>(TileSuitBamboos    + dice));
			} else if (dice == 9) { // 9はそのまま9がドラ
				nagatadora(CharacterNine); nagatadora(CircleNine); nagatadora(BambooNine);
			} else if (dice == 10) { // 10は三元牌がドラ
				nagatadora(WhiteDragon); nagatadora(GreenDragon); nagatadora(RedDragon);
			} else if (dice == 11) { // 11は数牌の1がドラ
				nagatadora(CharacterOne); nagatadora(CircleOne); nagatadora(BambooOne);
			} else if (dice == 12) { // 12は風牌全てドラ
				nagatadora(EastWind); nagatadora(SouthWind); nagatadora(WestWind); nagatadora(NorthWind);
			}
		}
	}
	DoraAdding(gameStat);
	if (RuleData::chkRule("dora_twice", "yes") ||
		(RuleData::chkRule("dora_twice", "only_when_doublets") && (gameStat->Dice[0].Number == gameStat->Dice[1].Number))) {
			gameStat->DoraPointer -= 2; /*ドラドラ卓*/
			DoraAdding(gameStat);
	}
}
#endif /* GUOBIAO */

// -------------------------------------------------------------------------

void SeatShuffler::shuffleSeat () {
	// 退避
	InfoByPlayer<EnvTable::PlayerLabel> TmpPlayerDat;
	for (PlayerID i = 0; i < Players; i++) {
		TmpPlayerDat[i].PlayerName = EnvTable::Instantiate()->PlayerDat[i].PlayerName;
		TmpPlayerDat[i].RemotePlayerFlag = EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag;
	}
	std::vector<PlayerID> TmpPosition;
	for (PlayerID i = 0; i < ACTUAL_PLAYERS; i++) TmpPosition.push_back(i);
	// 場決め
	if (EnvTable::Instantiate()->GameMode != EnvTable::Client) {
		// 場決め処理
		RndNum::shuffle(TmpPosition);
		// サーバーであれば結果を送信
		if (EnvTable::Instantiate()->GameMode == EnvTable::Server)
			for (PlayerID i = 0; i < ACTUAL_PLAYERS; i++)
				mihajong_socket::server::send(TmpPosition[i]);
	} else {
		// クライアントであれば受信する
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; i++) {
			int receivedByte;
			while ((receivedByte = mihajong_socket::getc(0)) == -1) // 受信待ち
				threadYield();
			TmpPosition[i] = receivedByte;
		}
	}
	// シャッフル結果を書き込み
	for (PlayerID i = 0; i < ACTUAL_PLAYERS; i++) {
		EnvTable::Instantiate()->PlayerDat[TmpPosition[i]].PlayerName = TmpPlayerDat[i].PlayerName;
		EnvTable::Instantiate()->PlayerDat[TmpPosition[i]].RemotePlayerFlag = TmpPlayerDat[i].RemotePlayerFlag;
		posarry[i] = TmpPosition[i];
	}

	// リモートとしてマーク
	/*PlayerID tmpPlayer = TmpPosition[ClientNumber];
	if (EnvTable::Instantiate()->GameMode == EnvTable::Client)
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; i++)
			EnvTable::Instantiate()->PlayerDat[TmpPosition[i]].RemotePlayerFlag =
			(i != tmpPlayer) ? 1 : 0;*/

	{
		CodeConv::tostringstream o;
		o << _T("ClientNumber [") << static_cast<int>(ClientNumber) << _T("]");
		debug(o.str().c_str());
	}
	{
		CodeConv::tostringstream o; o << _T("TmpPosition ");
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; i++)
			o << (i ? _T(" ") : _T("[")) << static_cast<int>(TmpPosition[i]);
		o << _T("]"); debug(o.str().c_str());
	}
	{
		CodeConv::tostringstream o; o << _T("Remote? ");
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; i++)
			o << (i ? _T(" ") : _T("[")) << static_cast<int>(EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag);
		o << _T("]"); debug(o.str().c_str());
	}
	{
		CodeConv::tostringstream o; o << _T("Name ");
		for (PlayerID i = 0; i < ACTUAL_PLAYERS; i++)
			o << (i ? _T(" [") : _T("[")) << EnvTable::Instantiate()->PlayerDat[i].PlayerName << _T("]");
		debug(o.str().c_str());
	}

	return;
}
unsigned SeatShuffler::ClientNumber;
int* SeatShuffler::posarry = nullptr;
std::array<int, Players> SeatShuffler::shuffle (unsigned cNumber) {
	std::array<int, Players> posArry;
	posarry = &posArry[0];
	ClientNumber = cNumber;
	shuffleSeat();
	return posArry;
}

// -------------------------------------------------------------------------

/* 半荘の初期化 */
void gameinit(GameTable* gameStat, GameTypeID gameType, const std::string& ServerAddress, const std::array<int, 4>& PositionArray, unsigned ClientNumber) {
	gameStat = initializeGameTable(gameType);
	gameStat->PlayerID = PositionArray[ClientNumber];
	haifu::haifubufinit();
	yaku::yakuCalculator::init(); // 役カタログの初期化
	aiscript::initscript(); // AIの初期化
	return;
}

/* 卓の初期化 */
namespace {
	void skippableWait(unsigned milliseconds) {
		static bool reallyWait = true;
		if (milliseconds == 0) // フラグをリセット
			reallyWait = true;
		if ((milliseconds) && (reallyWait) && (mihajong_graphic::ui::WaitUIWithTimeout(milliseconds) != 0xffffffff)) // 待機する
			reallyWait = false;
	}
	void init_ai(const GameTable* const gameStat) {
		aiscript::initephemeral(); // AIのephemeralテーブルを初期化
		for (PlayerID i = 0; i < Players; i++)
			aiscript::initcall(makesandBox(gameStat, i), i);
	}
	void statsync(GameTable* const gameStat, std::uint8_t serverMsg, std::function<bool (GameTable* const, int)> f) {
		if (EnvTable::Instantiate()->GameMode == EnvTable::Server) {
			mihajong_socket::server::send(serverMsg);
		}
		else if (EnvTable::Instantiate()->GameMode == EnvTable::Client) {
			volatile int ClientReceived = 0;
			int ReceivedMsg = 1023;
			while (true) {
				mihajong_socket::client::receive(&ClientReceived, &ReceivedMsg);
				if (ClientReceived == 1)
					if (f(gameStat, ReceivedMsg))
						break;
				threadYield();
			}
		}
	}
	void syncTableStat(GameTable* const gameStat) {
		// 場風のデータを送信。とりあえず１６周目の北四局まで対応
		statsync(gameStat, gameStat->LoopRound * roundLoopRate() + gameStat->GameRound,
			[](GameTable* const gameStat, int ReceivedMsg) -> bool {
				gameStat->LoopRound = ReceivedMsg / roundLoopRate();
				gameStat->GameRound = ReceivedMsg % roundLoopRate();
				return true;
			});
		// 積み棒のデータを送信。とりあえず２５５本場まで対応
		statsync(gameStat, gameStat->Honba,
			[](GameTable* const gameStat, int ReceivedMsg) -> bool {
				gameStat->Honba = ReceivedMsg;
				return true;
			});
		// 供託本数のデータを送信。とりあえず２５５０００点まで対応
		statsync(gameStat, gameStat->Deposit,
			[](GameTable* const gameStat, int ReceivedMsg) -> bool {
				gameStat->Deposit = ReceivedMsg;
				return true;
			});
		// 八連荘の判定に使うデータを送信
		statsync(gameStat, ((gameStat->LastAgariPlayer + 1) << 4) + gameStat->AgariChain,
			[](GameTable* const gameStat, int ReceivedMsg) -> bool {
				gameStat->LastAgariPlayer = (ReceivedMsg + 1) / 16 - 1;
				gameStat->AgariChain = (ReceivedMsg + 1) % 16 - 1;
				return true;
			});
		// 持ち点のデータを送信。符号込みで33バイトずつ・スモールエンディアン
		for (PlayerID player = 0; player < Players; player++) {
			for (int i = 0; i < DigitGroups; i++)
				for (int j = 0; j < 4; j++)
					statsync(gameStat, (gameStat->Player[player].PlayerScore.digitGroup[i] >> (j * 8)) & 0xff,
						[player, i, j](GameTable* const gameStat, int ReceivedMsg) -> bool {
							if (!j) gameStat->Player[player].PlayerScore.digitGroup[i] = 0;
							gameStat->Player[player].PlayerScore.digitGroup[i] |= static_cast<int>(ReceivedMsg) << (j * 8);
							return true;
						});
			statsync(gameStat, (gameStat->Player[player].PlayerScore < 0) ? 0x01 : 0x00,
				[player](GameTable* const gameStat, int ReceivedMsg) -> bool {
					if (ReceivedMsg) gameStat->Player[player].PlayerScore *= -1;
					return true;
				});
		}
		// 焼鳥のデータを送信
		statsync(gameStat, (gameStat->Player[0].YakitoriFlag ? 8 : 0) + (gameStat->Player[1].YakitoriFlag ? 4 : 0) + (gameStat->Player[2].YakitoriFlag ? 2 : 0) + (gameStat->Player[3].YakitoriFlag ? 1 : 0),
			[](GameTable* const gameStat, int ReceivedMsg) -> bool {
				gameStat->Player[0].YakitoriFlag = ReceivedMsg & 0x08;
				gameStat->Player[1].YakitoriFlag = ReceivedMsg & 0x04;
				gameStat->Player[2].YakitoriFlag = ReceivedMsg & 0x02;
				gameStat->Player[3].YakitoriFlag = ReceivedMsg & 0x01;
				return true;
			});
		// チップのデータを送信。一応±１２７まで対応
		for (PlayerID player = 0; player < Players; player++)
			/* Excess-128 */
			statsync(gameStat, gameStat->Player[player].playerChip + 128,
				[player](GameTable* const gameStat, int ReceivedMsg) -> bool {
					gameStat->Player[player].playerChip = static_cast<int>(ReceivedMsg) - 128;
					return true;
				});
		// 四馬路解禁フラグを送信
		statsync(gameStat, (gameStat->Player[0].SumaroFlag ? 8 : 0) + (gameStat->Player[1].SumaroFlag ? 4 : 0) + (gameStat->Player[2].SumaroFlag ? 2 : 0) + (gameStat->Player[3].SumaroFlag ? 1 : 0),
			[](GameTable* const gameStat, int ReceivedMsg) -> bool {
				gameStat->Player[0].SumaroFlag = ReceivedMsg & 0x08;
				gameStat->Player[1].SumaroFlag = ReceivedMsg & 0x04;
				gameStat->Player[2].SumaroFlag = ReceivedMsg & 0x02;
				gameStat->Player[3].SumaroFlag = ReceivedMsg & 0x01;
				return true;
			});
	}
	void choosebgm(const GameTable* const gameStat) { // BGMを鳴らす
		if (((gameStat->LoopRound * roundLoopRate() + gameStat->GameRound) == gameStat->GameLength) &&
			(gameStat->GameLength > 0))
			sound::util::bgmplay(sound::IDs::musFinal); // オーラスだけ特別
#ifndef GUOBIAO
		else if ((gameStat->Honba >= 5) && RuleData::chkRule("ryanshiba", "from_5honba"))
			sound::util::bgmplay(sound::IDs::musShibari); // リャンシバ時専用BGM
		else if ((gameStat->Honba >= 4) && RuleData::chkRule("ryanshiba", "from_4honba"))
			sound::util::bgmplay(sound::IDs::musShibari); // リャンシバ時専用BGM
#endif /* GUOBIAO */
		else
			sound::util::bgmplay(sound::IDs::BgmStart + gameStat->GameRound);
	}
	void screen(const GameTable* const gameStat) {
		skippableWait(0);
		CodeConv::tstring tmpStatus;
		if (gameStat->Honba) {
			CodeConv::tostringstream o;
			o << roundName(gameStat->GameRound, gameStat) << gameStat->Honba << _T("本場");
			tmpStatus = o.str();
		} else {
			tmpStatus = roundName(gameStat->GameRound, gameStat);
		}
		for (PlayerID i = 0; i < Players; ++i)
			mihajong_graphic::calltext::setCall(i, mihajong_graphic::calltext::None); /* 発声文字列を消去 */
		EnvTable* env = EnvTable::Instantiate();
		if (gameStat->LoopRound % 2 == 0) {
			switch (gameStat->GameRound / Players) {
			case 0:
				if (gameStat->LoopRound == 0) {
					env->bgColorR =   0; env->bgColorG = 160; env->bgColorB =   0;
				} else {
					env->bgColorR =  80; env->bgColorG = 160; env->bgColorB =   0;
				}
				break;
			case 1:
				if (gameStat->LoopRound == 0) {
					env->bgColorR =   0; env->bgColorG = 160; env->bgColorB = 160;
				} else {
					env->bgColorR = 160; env->bgColorG =  80; env->bgColorB =   0;
				}
				break;
			case 2:
				env->bgColorR =   0; env->bgColorG =  80; env->bgColorB = 160;
				break;
			case 3:
				env->bgColorR = 120; env->bgColorG =   0; env->bgColorB = 160;
				break;
			}
		} else {
			switch (gameStat->GameRound / Players) {
			case 0:
				env->bgColorR = 160; env->bgColorG = 160; env->bgColorB = 160;
				break;
			case 1:
				env->bgColorR = 160; env->bgColorG = 160; env->bgColorB =   0;
				break;
			case 2:
				env->bgColorR = 160; env->bgColorG =   0; env->bgColorB =   0;
				break;
			case 3:
				env->bgColorR =  80; env->bgColorG =  80; env->bgColorB =  80;
				break;
			}
		}
		switch (gameStat->GameRound / Players) {
		case 4:
			env->bgColorR = 120; env->bgColorG = 120; env->bgColorB = 160;
			break;
		case 5:
			env->bgColorR = 120; env->bgColorG = 160; env->bgColorB = 120;
			break;
		case 6:
			env->bgColorR = 160; env->bgColorG = 120; env->bgColorB = 120;
			break;
		}
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		mihajong_graphic::Transit(mihajong_graphic::sceneGameTable);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneBeginning);
	}
	void tileshuffle(GameTable* const gameStat) {
		shuffle(gameStat); unsigned tmpNumberOfTiles;
#ifdef GUOBIAO
		tmpNumberOfTiles = 144;
#else /* GUOBIAO */
		if (gameStat->chkGameType(AllSanma))
			tmpNumberOfTiles = 108;
		else if (RuleData::chkRule("flower_tiles", "no"))
			tmpNumberOfTiles = 136;
		else if (RuleData::chkRule("flower_tiles", "8tiles"))
			tmpNumberOfTiles = 144;
		else
			tmpNumberOfTiles = 140;
#endif /* GUOBIAO */
		for (unsigned i = 0; i < tmpNumberOfTiles; i++) // サーバーの場合、牌山のデータを送信
			statsync(gameStat, gameStat->Deck[i].tile + (gameStat->Deck[i].red * TileNonflowerMax) + mihajong_socket::protocol::StartRound_Tile_Excess,
				[i](GameTable* const gameStat, int ReceivedMsg) -> bool { // クライアントの場合、データを受信
					const auto recvTile = ReceivedMsg - mihajong_socket::protocol::StartRound_Tile_Excess;
					if ((recvTile > TileNonflowerMax) && (recvTile < TileSuitFlowers)) {
							gameStat->Deck[i] = Tile(TileCode(recvTile % TileNonflowerMax), doraCol(recvTile / TileNonflowerMax));
					} else {
						gameStat->Deck[i] = Tile(TileCode(recvTile));
					}
					return true;
				});
	}
	bool rolldice(GameTable* const gameStat, bool doraAlreadyAdded, unsigned diceOffset) {
		uint8_t tmpDeadTiles = gameStat->DeadTiles;
		// 賽を振る
		for (unsigned i = 0; i < 2; i++) {
			gameStat->Dice[i + diceOffset].Number = RndNum::dice();
			gameStat->Dice[i + diceOffset].Direction = RndNum::rnd(4);
		}
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		for (unsigned k = 0; k < 10; k++) { // 賽を振る
			for (unsigned i = 0; i < 2; i++) {
				gameStat->Dice[i + diceOffset].Number = RndNum::dice();
				gameStat->Dice[i + diceOffset].Direction = RndNum::rnd(4);
			}
			sound::Play(sound::IDs::sndSaikoro);
			mihajong_graphic::GameStatus::updateGameStat(gameStat); skippableWait(80);
		}
		sound::Play(sound::IDs::sndSaikoro);
		/* サイコロの出目を送信 */
		for (unsigned i = 0; i < 2; i++)
			statsync(gameStat, gameStat->Dice[i + diceOffset].Number + mihajong_socket::protocol::StartRound_Dice_Excess,
				[i, diceOffset](GameTable* const gameStat, int ReceivedMsg) -> bool {
					gameStat->Dice[i + diceOffset].Number = ReceivedMsg - mihajong_socket::protocol::StartRound_Dice_Excess;
					gameStat->Dice[i + diceOffset].Direction = RndNum::rnd(4);
					return true;
				});
		// --
#ifndef GUOBIAO
		if ((!doraAlreadyAdded) && (RuleData::chkRule("dora_twice", "yes") ||
			(RuleData::chkRule("dora_twice", "only_when_doublets") &&
			(gameStat->Dice[0 + diceOffset].Number == gameStat->Dice[1 + diceOffset].Number))))
			gameStat->DeadTiles += 2; /* ドラドラ卓なら王牌の数を増やす */
		calcWareme(gameStat); // 割れ目
		if (RuleData::chkRule("dice_roll", "roll_twice")) {
#endif /* GUOBIAO */
			mihajong_graphic::GameStatus::updateGameStat(gameStat);
			skippableWait(500);
#ifndef GUOBIAO
		}
#endif /* GUOBIAO */
		return tmpDeadTiles != gameStat->DeadTiles;
	}
	void haipai(GameTable* const gameStat) { // 配牌
		for (int i = 0; i < (gameStat->chkGameType(AllSanma) ? 36 : 48); i++) { // ２幢ずつを３回
			unsigned handIndex = i % 4 + (i / (gameStat->chkGameType(AllSanma) ? 12 : 16)) * 4;
			PlayerID player;
			if (gameStat->chkGameType(Sanma4))
				player = ((i % 12 / 4) + gameStat->GameRound) % 4;
			else if (gameStat->chkGameType(SanmaT))
				player = ((i % 12 / 4) + (gameStat->GameRound - (gameStat->GameRound / 4))) % 3;
			else
				player = ((i % 16 / 4) + gameStat->GameRound) % 4;
			gameStat->Player[player].Hand[handIndex] = gameStat->Deck[gameStat->TilePointer];
			++gameStat->TilePointer;
			if ((i == (gameStat->chkGameType(AllSanma) ? 24 : 18)) && (gameStat->Honba > 0))
				mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneHonba);
			if (i % 4 == 3) {
				calcdoukasen(gameStat);
				sound::Play(sound::IDs::sndTsumo);
				mihajong_graphic::GameStatus::updateGameStat(gameStat); skippableWait(250);
			}
		}
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneNone);
		for (int i = 0; i < (gameStat->chkGameType(AllSanma) ? 4 : 5); i++) { // １枚ずつを１回、親のチョンチョン
			unsigned handIndex = i / (gameStat->chkGameType(AllSanma) ? 3 : 4) + 12;
			PlayerID player;
			if (gameStat->chkGameType(Sanma4))
				player = (i % 3 + gameStat->GameRound) % 4;
			else if (gameStat->chkGameType(SanmaT))
				player = (i + (gameStat->GameRound - (gameStat->GameRound / 4))) % 3;
			else
				player = (i + gameStat->GameRound) % 4;
			gameStat->Player[player].Hand[handIndex] = gameStat->Deck[gameStat->TilePointer];
			++gameStat->TilePointer;
			calcdoukasen(gameStat);
			sound::Play(sound::IDs::sndTsumo);
			mihajong_graphic::GameStatus::updateGameStat(gameStat); skippableWait(250);
		}

#ifndef GUOBIAO
		initdora(gameStat); // ドラをめくる

		sound::Play(sound::IDs::sndMekuri);
#endif /* GUOBIAO */
		haifu::haifurechaipai(gameStat);
		for (PlayerID i = 0; i < Players; i++)
			lipai(gameStat, i);
		sound::Play(sound::IDs::sndBell);
	}
}
void tableinit(GameTable* const gameStat) {
	for (int i = 0; i < Players; ++i) { // 代走させる処理
		if (gameStat->Player[i].ConnectionLost) {
			EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag = 0;
			gameStat->Player[i].ConnectionLost = false;
		}
	}
	init_ai(gameStat);
	inittable(gameStat);
	// 局の開始で同期する。1.7系列まではこのとき落ち戻りが可能(落ち戻り機能は1.8で廃止されました)
	statsync(gameStat, mihajong_socket::protocol::Server_StartRound_Signature,
		[](GameTable* const gameStat, int ReceivedMsg) -> bool {
			if (ReceivedMsg == mihajong_socket::protocol::Server_StartRound_Signature) {
				return true;
			} else if (ReceivedMsg == 1023) { // 回線が切れてたら
				EnvTable::Instantiate()->GameMode = EnvTable::Standalone;
				for (int i = 0; i < Players; ++i)
					EnvTable::Instantiate()->PlayerDat[i].RemotePlayerFlag = 0;
				return true;
			} else {
				return false;
			}
		});
	// 牌譜バッファの初期化
	haifu::haifuinit();
	/* 卓の情報を同期 */
	syncTableStat(gameStat);
	// BGMを鳴らす
	choosebgm(gameStat);
	// 画面の準備
	screen(gameStat);
	// 洗牌
	tileshuffle(gameStat);
	// 賽を振る
	bool doraFlag = rolldice(gameStat, false, 0);
#ifndef GUOBIAO
	if (RuleData::chkRule("dice_roll", "roll_twice"))
#endif /* GUOBIAO */
		(void)rolldice(gameStat, doraFlag, 2); // 二度振りの2回目
	// 配牌
	haipai(gameStat);
	return;
}
