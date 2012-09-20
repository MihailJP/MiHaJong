#include "prepare.h"

#define settile(tilecode, pos) {tilepos[tilecode] = pos; for (unsigned int i = 0; i < 4u; ++i) {gameStat->Deck[pos++].tile = tilecode;}}
inline unsigned int inittiles(GameTable* const gameStat, UInt8ByTile& tilepos) { // 牌を並べる
	unsigned int p = 0; // 牌の位置ID
	if (chkGameType(gameStat, SanmaX)) {
		settile(CharacterOne, p); // 萬子
		settile(CharacterNine, p); // 萬子
	} else {
		for (unsigned int k = 1u; k <= 9u; ++k)
			settile((tileCode)(TILE_SUIT_CHARACTERS + k), p); // 萬子
	}
	for (unsigned int k = 1u; k <= 9u; ++k)
		settile((tileCode)(TILE_SUIT_CIRCLES + k), p); // 筒子
	for (unsigned int k = 1u; k <= 9u; ++k)
		settile((tileCode)(TILE_SUIT_BAMBOOS + k), p); // 索子
	if (!chkGameType(gameStat, SanmaS)) {
		for (unsigned int k = 1u; k <= 7u; ++k)
			settile((tileCode)(TILE_SUIT_HONORS + k), p); // 字牌
		if (RuleData::chkRule("flower_tiles", "seasons") || RuleData::chkRule("flower_tiles", "8tiles")) {
			gameStat->Deck[p++].tile = Spring; gameStat->Deck[p++].tile = Summer;
			gameStat->Deck[p++].tile = Autumn; gameStat->Deck[p++].tile = Winter;
		}
		if (RuleData::chkRule("flower_tiles", "flowers") || RuleData::chkRule("flower_tiles", "8tiles")) {
			gameStat->Deck[p++].tile = Plum; gameStat->Deck[p++].tile = Orchid;
			gameStat->Deck[p++].tile = Chrysanthemum; gameStat->Deck[p++].tile = Bamboo;
		}
	}
	return p;
}

inline void redtiles(GameTable* const gameStat, UInt8ByTile& tilepos) { // 赤ドラを設定する
	{
		const char tileRules[9][16] = {
			"red_one", "red_two", "red_three", "red_four", "red_five",
			"red_six", "red_seven", "red_eight", "red_nine",
		};
		for (unsigned int i = 0; i < 9; ++i) { // 数牌の赤ドラ
			if (RuleData::chkRule(tileRules[i], "character_suit") || // 萬子
				RuleData::chkRule(tileRules[i], "character_circle_suit") ||
				RuleData::chkRule(tileRules[i], "character_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_CHARACTERS + i + 1]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "circle_suit") || // 筒子
				RuleData::chkRule(tileRules[i], "character_circle_suit") ||
				RuleData::chkRule(tileRules[i], "circle_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_CIRCLES + i + 1]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "bamboo_suit") || // 索子
				RuleData::chkRule(tileRules[i], "circle_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "character_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_BAMBOOS + i + 1]].red = AkaDora;
		}
	}
	// 5のドラは特殊
	if (chkGameType(gameStat, SanmaX)) { // 三麻
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
		const char tileRules[2][16] = {"red_west", "red_north",};
		const tileCode tileCodes[2] = {WestWind, NorthWind,};
		for (unsigned int i = 0; i < 2; ++i) { // 字牌の赤ドラ
			if (RuleData::chkRule(tileRules[i], "4tiles") || RuleData::chkRule(tileRules[i], "3tiles") ||
				RuleData::chkRule(tileRules[i], "2tiles") || RuleData::chkRule(tileRules[i], "1tile"))
				gameStat->Deck[tilepos[TILE_SUIT_CHARACTERS + i]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "4tiles") || RuleData::chkRule(tileRules[i], "3tiles") ||
				RuleData::chkRule(tileRules[i], "2tiles"))
				gameStat->Deck[tilepos[TILE_SUIT_CHARACTERS + i + 1]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "4tiles") || RuleData::chkRule(tileRules[i], "3tiles"))
				gameStat->Deck[tilepos[TILE_SUIT_CHARACTERS + i + 2]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "4tiles"))
				gameStat->Deck[tilepos[TILE_SUIT_CHARACTERS + i + 3]].red = AkaDora;
		}
	}
	{
		if (RuleData::chkRule("haku_potchi", "1tile_dora")) // オールマイティー扱いはせず、赤ドラと同じ扱い
			gameStat->Deck[tilepos[WhiteDragon]].red = AkaDora; // 白ポッチ
	}
}

inline void bluetiles(GameTable* const gameStat, UInt8ByTile& tilepos) { // 青ドラを設定する
	{
		const char tileRules[3][16] = {"blue_one", "blue_five", "blue_nine",};
		const unsigned int tileNum[3] = {1, 5, 9,};
		for (unsigned int i = 0; i < 3; ++i) { // 数牌の青ドラ
			if (RuleData::chkRule(tileRules[i], "character_suit") || // 萬子
				RuleData::chkRule(tileRules[i], "character_circle_suit") ||
				RuleData::chkRule(tileRules[i], "character_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_CHARACTERS + tileNum[i]] + 3].red = AoDora;
			if (RuleData::chkRule(tileRules[i], "circle_suit") || // 筒子
				RuleData::chkRule(tileRules[i], "character_circle_suit") ||
				RuleData::chkRule(tileRules[i], "circle_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_CIRCLES + tileNum[i]] + 3].red = AoDora;
			if (RuleData::chkRule(tileRules[i], "bamboo_suit") || // 索子
				RuleData::chkRule(tileRules[i], "circle_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "character_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_BAMBOOS + tileNum[i]] + 3].red = AoDora;
		}
	}
	{
		if (RuleData::chkRule("blue_lvfa", "yes"))
			gameStat->Deck[tilepos[GreenDragon] + 3].red = AoDora;
	}
}

inline void shuffletiles(GameTable* const gameStat, UInt8ByTile& tilepos, unsigned int tiles) { // 洗牌する
	if (EnvTable::Instantiate()->GameMode != EnvTable::Client) {
		for (unsigned int i = 0; i < tiles; ++i) { // 洗牌する
			unsigned int tmppos = i + RndNum::rnd(tiles - i);
			TILE tmptile = gameStat->Deck[tmppos];
			gameStat->Deck[tmppos] = gameStat->Deck[i];
			gameStat->Deck[i] = tmptile;
		}
	}
}

__declspec(dllexport) void shuffle(GameTable* const gameStat) { // 牌をバッファに並べて、洗牌
	UInt8ByTile tilepos;
	unsigned int tiles = inittiles(gameStat, tilepos);
	redtiles(gameStat, tilepos);
	bluetiles(gameStat, tilepos);
	shuffletiles(gameStat, tilepos, tiles);
}

#define nagatadora(tilecode) {++gameStat->DoraFlag.Omote[tilecode]; haifu::haifurecdora(tilecode);}
inline void DoraAdding(GameTable* const gameStat) {
	setdora(gameStat, 0); // 表ドラを設定する
	if (RuleData::chkRuleApplied("uradora"))
		setdora(gameStat, 1); // 裏ドラを設定する
	haifu::haifurecdorap();
}

__declspec(dllexport) void initdora(GameTable* const gameStat) { // ドラの設定
	if (chkGameType(gameStat, AllSanma))
		gameStat->DoraPointer = 102 - gameStat->ExtraRinshan; // ドラ表示牌のポインタ
	else gameStat->DoraPointer = 130; // ドラ表示牌のポインタ
	if (RuleData::chkRuleApplied("nagatacho")) { // 永田町ルール
		nagatadora(BambooSeven); // 七索は常にドラ
		unsigned int dice = gameStat->Dice[0].Number + gameStat->Dice[1].Number;
		if (dice <= 8) { // 2～8はその数牌がドラ　三麻では萬子がないので別処理
			if (!chkGameType(gameStat, SanmaX)) nagatadora(TILE_SUIT_CHARACTERS + dice);
			nagatadora(TILE_SUIT_CIRCLES + dice); nagatadora(TILE_SUIT_BAMBOOS + dice);
		} else if (dice == 9) { // 9はそのまま9がドラ
			nagatadora(CharacterNine); nagatadora(CircleNine); nagatadora(BambooNine);
		} else if (dice == 10) { // 10は三元牌がドラ
			nagatadora(WhiteDragon); nagatadora(GreenDragon); nagatadora(RedDragon);
		} else if (dice == 11) { // 11は数牌の1がドラ
			nagatadora(CharacterOne); nagatadora(CircleOne); nagatadora(BambooOne);
		} else if (dice == 12) { // 12は風牌全てドラ
			nagatadora(EastWind); nagatadora(SouthWind); nagatadora(WestWind); nagatadora(NorthWind);
		}
		haifu::haifurecdorap();
	}
	DoraAdding(gameStat);
	if (RuleData::chkRule("dora_twice", "yes") ||
		(RuleData::chkRule("dora_twice", "only_when_doublets") && (gameStat->Dice[0].Number == gameStat->Dice[1].Number))) {
			gameStat->DoraPointer -= 2; /*ドラドラ卓*/
			DoraAdding(gameStat);
	}
}