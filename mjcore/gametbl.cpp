#include "gametbl.h"

#ifdef MJCORE_EXPORTS
#include <algorithm>
#include <cassert>
#endif
#include <regex>
#include "except.h"
#include "func.h"
#include "tileutil.h"
#include "ruletbl.h"
#include "../common/largenum.h"

using namespace mihajong_structs;

GameTable GameStat, StatSandBox;

void calcWareme(GameTable* const gameStat) {
#ifndef GUOBIAO
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	if (RuleData::chkRuleApplied("wareme") || RuleData::chkRuleApplied("kaimenkaze")) {
		if (gameStat->chkGameType(GameTypeID::allSanma)) {
			gameStat->WaremePlayer = ((gameStat->GameRound-(gameStat->GameRound/4))+24
				+gameStat->diceSum()-1) % 3;
		} else {
			gameStat->WaremePlayer = ((gameStat->GameRound % 4)+32+gameStat->diceSum()-1) % 4;
		}
		if (RuleData::chkRule("dice_roll", "roll_twice")) { // 二度振り修正用
			if (gameStat->chkGameType(GameTypeID::allSanma)) {
				if (gameStat->diceSum() + gameStat->diceSum2() > 18)
					gameStat->WaremePlayer = (gameStat->WaremePlayer + 24 - 1) % 3;
			} else if (RuleData::chkRule("flower_tiles", "8tiles")) {
				if (gameStat->diceSum() + gameStat->diceSum2() > 18)
					gameStat->WaremePlayer = (gameStat->WaremePlayer + 32 - 1) % 4;
			} else if (RuleData::chkRule("flower_tiles", "no")) {
				if (gameStat->diceSum() + gameStat->diceSum2() > 17)
					gameStat->WaremePlayer = (gameStat->WaremePlayer + 32 - 1) % 4;
			} else {
				if (gameStat->diceSum() + gameStat->diceSum2() > 18)
					gameStat->WaremePlayer = (gameStat->WaremePlayer + 32 - 1) % 4;
				else if ((gameStat->diceSum() + gameStat->diceSum2() == 18) && (gameStat->WaremePlayer % 2 == 1))
					gameStat->WaremePlayer = (gameStat->WaremePlayer + 32 - 1) % 4;
			}
		}
		if (gameStat->chkGameType(GameTypeID::sanma4))
			gameStat->WaremePlayer = tobePlayed(gameStat, (24+gameStat->diceSum()-1) % 3);
	}
#endif /* GUOBIAO */
	return;
}


/* ---------------------------------------------------------------------
**  初期化処理
** --------------------------------------------------------------------- */

void resetDeclarationFlag(GameTable* const gameStat) {
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	for (int pl = 0; pl < Players; pl++) {
		gameStat->Player[pl].DeclarationFlag.Ron =
			gameStat->Player[pl].DeclarationFlag.Pon =
			gameStat->Player[pl].DeclarationFlag.Kan = false;
		gameStat->Player[pl].DeclarationFlag.Chi = ChiiType::none;
	}
	return;
}

void inittable(GameTable* const gameStat) { /* 局単位での初期化 */
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
#ifdef GUOBIAO
	gameStat->ShibariFlag = false;
#else /* GUOBIAO */
	gameStat->ShibariFlag = //二飜縛り
		((gameStat->Honba >= 5)&&(RuleData::chkRule("ryanshiba", "from_5honba"))) ||
		((gameStat->Honba >= 4)&&(RuleData::chkRule("ryanshiba", "from_4honba")));
#endif /* GUOBIAO */

	for (int i = 0; i < PaoYakuPages; i++) // 包フラグ（-1…なし、0〜3…該当プレイヤー）
		gameStat->PaoFlag[i].agariPlayer = gameStat->PaoFlag[i].paoPlayer = -1;

	for (int i = 0; i < SizeOfDeckBuf; i++) { // ちゃんと初期化してあげましょうね
		gameStat->Deck[i] = Tile();
	}

#ifdef GUOBIAO
	gameStat->DeadTiles = gameStat->ExtraRinshan = 0; // 王牌なんてあると思った？　残念！
#else /* GUOBIAO */
	if (gameStat->chkGameType(GameTypeID::allSanma)) {
		gameStat->DeadTiles = 14; // 王牌の数
		gameStat->ExtraRinshan = RuleData::chkRuleApplied("flower_tiles") ? 4 : 0;
	} else {
		if (RuleData::chkRule("flower_tiles", "no")) gameStat->DeadTiles = 14; // 王牌の数
		else if (RuleData::chkRule("flower_tiles", "8tiles")) gameStat->DeadTiles = 22; // 王牌の数(花牌を入れる時は特別に２２枚残しとする)
		else gameStat->DeadTiles = 18; // 王牌の数
	}
#endif /* GUOBIAO */

	for (int i = 0; i < TileNonflowerMax; i++) // プンリーの待ち牌(ＣＯＭに意図的な放銃を起こさせないために使用)
		gameStat->OpenRichiWait[i] = false;
	gameStat->KangFlag.kangFlag = gameStat->KangFlag.topFlag = false; // 嶺上開花；頭槓和；連開花と槓振り；搶槓の判定に使う
	gameStat->KangFlag.chainFlag = 0;
	gameStat->KangFlag.chankanFlag = ChankanStat::none;
	gameStat->TurnRound =  // 現在の巡目
		gameStat->KangNum = 0; // 四槓流局、四槓子などの判定に使う
	gameStat->RichiCounter =
		gameStat->DoubleRichiCounter = false; // リーチをカウンター(宣言牌をロン)
	gameStat->WaremePlayer = // 割れ目の位置(-1で割れ目なし)
		gameStat->DoukasenPlayer = -1; // 導火線の位置(-1で導火線なし)
	gameStat->DoraPointer = 999;
	gameStat->Dice[0].Number = gameStat->Dice[1].Number =
		gameStat->Dice[2].Number = gameStat->Dice[3].Number = 0;
	gameStat->Dice[0].Direction = gameStat->Dice[1].Direction = 
		gameStat->Dice[2].Direction = gameStat->Dice[3].Direction = 0;
	gameStat->TilePointer = 0;

#ifdef GUOBIAO
	gameStat->RinshanPointer = 143;
#else /* GUOBIAO */
	if (gameStat->chkGameType(GameTypeID::allSanma)) {
		gameStat->RinshanPointer = 107;
	} else {
		if (RuleData::chkRule("flower_tiles", "no")) gameStat->RinshanPointer = 135;
		else if ((RuleData::chkRule("flower_tiles", "seasons")) || (RuleData::chkRule("flower_tiles", "flowers")))
			gameStat->RinshanPointer = 139;
		else if (RuleData::chkRule("flower_tiles", "8tiles")) gameStat->RinshanPointer = 143;
		else {
			error(_T("flower_tiles異常。花牌無しルールとみなして処理します。"));
			gameStat->RinshanPointer = 135; // 設定異常時のフォールバック
		}
	}
#endif /* GUOBIAO */

	gameStat->TianHuFlag = true; // 親の第一打牌がまだ（天和の判定などに使う）
	gameStat->PreviousMeld.Discard = // 先ほど鳴いた牌（喰い替えの判定に使う）
		gameStat->PreviousMeld.Stepped = NoTile;
	for (int i = 0; i < TileNonflowerMax; i++) // ドラ判定の配列
		gameStat->DoraFlag.Omote[i] = gameStat->DoraFlag.Ura[i] = 0;
	gameStat->TsumoAgariFlag = false;
	gameStat->AgariSpecialStat = 0;
	resetDeclarationFlag(gameStat);
	gameStat->CurrentDiscard = Tile();
	gameStat->CurrentPlayer.Active = gameStat->CurrentPlayer.Passive =
		gameStat->CurrentPlayer.Agari = gameStat->CurrentPlayer.Furikomi = (PlayerID)-1;

	for (int pl = 0; pl < Players; pl++) {
		gameStat->Player[pl].ConnectionLost = false; // 回線切断による和了り放棄
		for (int i = 0; i < NumOfTilesInHand; i++) { // 手牌の配列(４人分)
			gameStat->Player[pl].Hand[i] = Tile();
		}
		gameStat->Player[pl].DiscardPointer = 0; // ちゃんとリセットしてあげましょうね
		for (int i = 0; i < SizeOfDiscardBuffer; i++) {
			// 捨牌の配列(４人分)
			gameStat->Player[pl].Discard[i].tcode = Tile();
			gameStat->Player[pl].Discard[i].dstat = DiscardStat::normal;
			gameStat->Player[pl].Discard[i].isDiscardThrough = false;
		}
		gameStat->Player[pl].MenzenFlag = true; // 門前フラグ
		gameStat->Player[pl].HandStat = HandStatCode::upright; // 手牌の状態（立てる・見せる・伏せる）
		gameStat->Player[pl].MeldPointer = 0; // 最初変な数字が入ってたりするんで……
		for (int i = 0; i < SizeOfMeldBuffer; i++) {
			// 鳴き面子を格納
			gameStat->Player[pl].Meld[i].tile = NoTile;
			for (int j = 0; j < 4; j++) gameStat->Player[pl].Meld[i].red[j] = DoraCol::normal;
			gameStat->Player[pl].Meld[i].mstat = static_cast<MeldStat>(0);
		}
		gameStat->Player[pl].NumberOfQuads = 0; // 槓子の数（四槓流局、三槓子、四槓子などの判定に使う）
		gameStat->Player[pl].RichiFlag.RichiFlag = // リーチしているかどうか
			gameStat->Player[pl].RichiFlag.IppatsuFlag =
			gameStat->Player[pl].RichiFlag.DoubleFlag =
			gameStat->Player[pl].RichiFlag.OpenFlag = false;
		gameStat->Player[pl].FirstDrawFlag = true; // １巡目である（地和、ダブル立直の判定に使う）
		gameStat->Player[pl].DoujunFuriten = // 同順振聴である
			gameStat->Player[pl].AgariHouki = false; // 和了り放棄の罰則中かどうか
		gameStat->Player[pl].FlowerFlag.Spring = // 晒している花牌を格納するフラグ
			gameStat->Player[pl].FlowerFlag.Summer =
			gameStat->Player[pl].FlowerFlag.Autumn =
			gameStat->Player[pl].FlowerFlag.Winter =
			gameStat->Player[pl].FlowerFlag.Plum =
			gameStat->Player[pl].FlowerFlag.Orchid =
			gameStat->Player[pl].FlowerFlag.Chrys =
			gameStat->Player[pl].FlowerFlag.Bamboo = false;
		gameStat->Player[pl].NorthFlag = 0; // 晒している北風牌を格納するフラグ
		gameStat->Player[pl].shokanFlag = // 初槓フラグ
			gameStat->Player[pl].kansanjunFlag = false; // 槓三巡フラグ
		gameStat->Player[pl].renpaiTenhohStat = 0; // 戻牌天和判定用
	}
	assert(gameStat->Player[0].DiscardPointer == 0); // 初期化できてるかチェック（デバッグ用）
}

void doInitializeGameTable(GameTable* const gameStat, GameTypeID gameType) { // 半荘単位の初期化処理
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	/* 内部処理用でエクスポートしない */
	memset(gameStat, 0, sizeof(GameTable));
	gameStat->gameType = gameType;

#ifdef GUOBIAO
	for (int i = 0; i < Players; i++)
		gameStat->Player[i].PlayerScore = 500; // 持ち点500点とする
	gameStat->GameLength = 15; // 中国ルールは一荘行う
#else /* GUOBIAO */
	LargeNum initialPoints; /* 初期点数 */
	if (RuleData::chkRule("starting_point", "custom")) {
		initialPoints = // 仮数部
			std::atoi(RuleData::chkRule("starting_point_mantissa_tens")) * 10 +
			std::atoi(RuleData::chkRule("starting_point_mantissa_ones"));
		/* 指数部の処理 */
		std::smatch matchDat; int exponent = 0;
		std::string expConf(RuleData::chkRule("starting_point_exponent"));
		if (std::regex_match(expConf, matchDat, std::regex("exp_(\\d+)")))
			exponent = atoi(matchDat[1].str().c_str()); // ルール設定文字列から整数を抽出
		for (int j = 0; j < exponent; ++j)
			initialPoints *= 10;
	} else {
		initialPoints = static_cast<LargeNum>(std::atoi(RuleData::chkRule("starting_point")));
	}

	for (int i = 0; i < Players; i++) {
		if (i < ACTUAL_PLAYERS)
			gameStat->Player[i].PlayerScore = initialPoints;
		else
			gameStat->Player[i].PlayerScore = 0;
	}

	if (RuleData::chkRule("game_length", "east_south_game"))
		gameStat->GameLength = GameStat.chkGameType(GameTypeID::sanmaT) ? 6 : 7;
	else if (RuleData::chkRule("game_length", "east_wind_game") ||
		RuleData::chkRule("game_length", "east_only_game"))
		gameStat->GameLength = GameStat.chkGameType(GameTypeID::sanmaT) ? 2 : 3;
	else if (RuleData::chkRule("game_length", "full_round_game") ||
		RuleData::chkRule("game_length", "east_north_game"))
		gameStat->GameLength = GameStat.chkGameType(GameTypeID::sanmaT) ? 14 : 15;
	else if (RuleData::chkRule("game_length", "single_round_game"))
		gameStat->GameLength = 0;
	else if (RuleData::chkRule("game_length", "twice_east_game"))
		gameStat->GameLength = GameStat.chkGameType(GameTypeID::sanmaT) ? 18 : 19;
	else if (RuleData::chkRule("game_length", "east_south_west_game") ||
		RuleData::chkRule("game_length", "east_west_game"))
		gameStat->GameLength = GameStat.chkGameType(GameTypeID::sanmaT) ? 10 : 11;
	else {
		error(_T("game_length異常値。半荘戦とみなします。"));
		gameStat->GameLength = GameStat.chkGameType(GameTypeID::sanmaT) ? 6 : 7;
	}
#endif /* GUOBIAO */
	gameStat->GameRound = gameStat->Honba = gameStat->PlayerID =
		gameStat->Deposit = gameStat->LoopRound = gameStat->AgariChain = 0;
	gameStat->LastAgariPlayer = -1;
	for (int i = 0; i < Players; i++) {
		gameStat->Player[i].SumaroFlag = false;
#ifdef GUOBIAO
		gameStat->Player[i].YakitoriFlag = false;
#else /* GUOBIAO */
		gameStat->Player[i].YakitoriFlag = RuleData::chkRuleApplied("yakitori");
#endif /* GUOBIAO */
		gameStat->Player[i].playerChip = 0;
	}

	inittable(gameStat); // 局ごとの初期化も行う
}

GameTable* initializeGameTable(GameTypeID gameType) { // 半荘単位の初期化処理
	doInitializeGameTable(&GameStat, gameType);
	return &GameStat;
}

// ---------------------------------------------------------------------

GameTable* makesandBox(const GameTable* const gameStat, PlayerID targetPlayer) {
	/* 卓の状態のサンドボックスを作る */
	GameTable* const sandbox = &StatSandBox;
	doInitializeGameTable(sandbox, gameStat->gameType);
	for (int p = 0; p < Players; p++) {
		sandbox->Player[p].PlayerScore = gameStat->Player[p].PlayerScore;
		sandbox->Player[p].playerChip = gameStat->Player[p].playerChip;
		sandbox->Player[p].SumaroFlag = gameStat->Player[p].SumaroFlag;
		sandbox->Player[p].shokanFlag = gameStat->Player[p].shokanFlag;
		sandbox->Player[p].kansanjunFlag = gameStat->Player[p].kansanjunFlag;
		sandbox->Player[p].YakitoriFlag = gameStat->Player[p].YakitoriFlag;
		if ((gameStat->Player[p].RichiFlag.OpenFlag)||(p == targetPlayer)) {
			for (int i = 0; i < NumOfTilesInHand; i++) {
				sandbox->Player[p].Hand[i] = gameStat->Player[p].Hand[i];
			}
			sandbox->Player[p].renpaiTenhohStat = gameStat->Player[p].renpaiTenhohStat;
		}
		sandbox->Player[p].RichiFlag = gameStat->Player[p].RichiFlag;
		for (int i = 0; i < SizeOfDiscardBuffer; i++) {
			sandbox->Player[p].Discard[i].isDiscardThrough = gameStat->Player[p].Discard[i].isDiscardThrough;
			sandbox->Player[p].Discard[i].dstat = gameStat->Player[p].Discard[i].dstat;
			sandbox->Player[p].Discard[i].tcode = gameStat->Player[p].Discard[i].tcode;
		}
		sandbox->Player[p].DiscardPointer = gameStat->Player[p].DiscardPointer;
		for (int i = 0; i < SizeOfMeldBuffer; i++) {
			sandbox->Player[p].Meld[i].mstat = gameStat->Player[p].Meld[i].mstat;
			sandbox->Player[p].Meld[i].tile = gameStat->Player[p].Meld[i].tile;
			for (int j = 0; j < 4; j++)
				sandbox->Player[p].Meld[i].red[j] = gameStat->Player[p].Meld[i].red[j];
		}
		sandbox->Player[p].MeldPointer = gameStat->Player[p].MeldPointer;
		sandbox->Player[p].MenzenFlag = gameStat->Player[p].MenzenFlag;
		sandbox->Player[p].NumberOfQuads = gameStat->Player[p].NumberOfQuads;
		sandbox->Player[p].RichiFlag.RichiFlag = gameStat->Player[p].RichiFlag.RichiFlag;
		sandbox->Player[p].RichiFlag.IppatsuFlag = gameStat->Player[p].RichiFlag.IppatsuFlag;
		sandbox->Player[p].RichiFlag.DoubleFlag = gameStat->Player[p].RichiFlag.DoubleFlag;
		sandbox->Player[p].RichiFlag.OpenFlag = gameStat->Player[p].RichiFlag.OpenFlag;
		sandbox->Player[p].FirstDrawFlag = gameStat->Player[p].FirstDrawFlag;
		sandbox->Player[p].DoujunFuriten = gameStat->Player[p].DoujunFuriten;
		sandbox->Player[p].AgariHouki = gameStat->Player[p].AgariHouki;
		sandbox->Player[p].FlowerFlag = gameStat->Player[p].FlowerFlag;
		sandbox->Player[p].NorthFlag = gameStat->Player[p].NorthFlag;
		sandbox->Player[p].ConnectionLost = gameStat->Player[p].ConnectionLost;
	}
	sandbox->PlayerID = gameStat->PlayerID;
	sandbox->GameRound = gameStat->GameRound;
	sandbox->LoopRound = gameStat->LoopRound;
	sandbox->Honba = gameStat->Honba;
	sandbox->TurnRound = gameStat->TurnRound;
	sandbox->Deposit = gameStat->Deposit;
	sandbox->AgariChain = gameStat->AgariChain;
	sandbox->LastAgariPlayer = gameStat->LastAgariPlayer;
	for (int i = 0; i < TileNonflowerMax; i++)
		sandbox->OpenRichiWait[i] = gameStat->OpenRichiWait[i];
	sandbox->KangFlag.kangFlag = gameStat->KangFlag.kangFlag;
	sandbox->KangFlag.chainFlag = gameStat->KangFlag.chainFlag;
	sandbox->KangFlag.topFlag = gameStat->KangFlag.topFlag;
	sandbox->KangFlag.chankanFlag = gameStat->KangFlag.chankanFlag;
	sandbox->KangNum = gameStat->KangNum;
	sandbox->RichiCounter = gameStat->RichiCounter;
	sandbox->DoubleRichiCounter = gameStat->DoubleRichiCounter;
	sandbox->DoukasenPlayer = gameStat->DoukasenPlayer;
	for (int i = 0; i < PaoYakuPages; i++) {
		sandbox->PaoFlag[i].paoPlayer = gameStat->PaoFlag[i].paoPlayer;
		sandbox->PaoFlag[i].agariPlayer = gameStat->PaoFlag[i].agariPlayer;
	}
	for (int i = 0; i < 4; i++) {
		sandbox->Dice[i].Number = gameStat->Dice[i].Number;
		sandbox->Dice[i].Direction = gameStat->Dice[i].Direction;
	}
	for (int i = 0; i < 6; i++) {
		if (gameStat->chkGameType(GameTypeID::allSanma)) {
			if (gameStat->DoraPointer <= (102 - gameStat->ExtraRinshan - i * 2))
				sandbox->Deck[102 - gameStat->ExtraRinshan - i * 2] =
				gameStat->Deck[102 - gameStat->ExtraRinshan - i * 2];
		} else {
			if (gameStat->DoraPointer <= (130 - i * 2))
				sandbox->Deck[130 - i * 2] = gameStat->Deck[130 - i * 2];
		}
	}
	sandbox->TilePointer = gameStat->TilePointer;
	sandbox->DoraPointer = gameStat->DoraPointer;
	sandbox->RinshanPointer = gameStat->RinshanPointer;
	sandbox->TianHuFlag = gameStat->TianHuFlag;
	sandbox->PreviousMeld.Discard = gameStat->PreviousMeld.Discard;
	sandbox->PreviousMeld.Stepped = gameStat->PreviousMeld.Stepped;
	sandbox->Deposit = gameStat->Deposit;
	sandbox->Deposit = gameStat->Deposit;
	sandbox->Deposit = gameStat->Deposit;
	sandbox->Deposit = gameStat->Deposit;
	for (int i = 0; i < TileNonflowerMax; i++) {
		sandbox->DoraFlag.Omote[i] = gameStat->DoraFlag.Omote[i];
		sandbox->DoraFlag.Ura[i] = gameStat->DoraFlag.Ura[i];
	}
	sandbox->CurrentDiscard = gameStat->CurrentDiscard;
	sandbox->CurrentPlayer.Active = gameStat->CurrentPlayer.Active;
	sandbox->CurrentPlayer.Passive = gameStat->CurrentPlayer.Passive;
	sandbox->CurrentPlayer.Agari = gameStat->CurrentPlayer.Agari;
	sandbox->CurrentPlayer.Furikomi = gameStat->CurrentPlayer.Furikomi;

	return sandbox;
}

// ---------------------------------------------------------------------

MJCORE GameTable* setGameType(GameTypeID gameType) {
	GameStat.gameType = gameType;
	return &GameStat;
}
