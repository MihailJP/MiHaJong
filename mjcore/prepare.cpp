#include "prepare.h"

#define settile(tilecode, pos) {tilepos[tilecode] = pos; for (unsigned int i = 0; i < 4u; ++i) {gameStat->Deck[pos++].tile = tilecode;}}
inline unsigned int inittiles(GameTable* const gameStat, UInt8ByTile& tilepos) { // �v����ׂ�
	unsigned int p = 0; // �v�̈ʒuID
	if (chkGameType(gameStat, SanmaX)) {
		settile(CharacterOne, p); // �ݎq
		settile(CharacterNine, p); // �ݎq
	} else {
		for (unsigned int k = 1u; k <= 9u; ++k)
			settile((tileCode)(TILE_SUIT_CHARACTERS + k), p); // �ݎq
	}
	for (unsigned int k = 1u; k <= 9u; ++k)
		settile((tileCode)(TILE_SUIT_CIRCLES + k), p); // ���q
	for (unsigned int k = 1u; k <= 9u; ++k)
		settile((tileCode)(TILE_SUIT_BAMBOOS + k), p); // ���q
	if (!chkGameType(gameStat, SanmaS)) {
		for (unsigned int k = 1u; k <= 7u; ++k)
			settile((tileCode)(TILE_SUIT_HONORS + k), p); // ���v
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

inline void redtiles(GameTable* const gameStat, UInt8ByTile& tilepos) { // �ԃh����ݒ肷��
	{
		const char tileRules[9][16] = {
			"red_one", "red_two", "red_three", "red_four", "red_five",
			"red_six", "red_seven", "red_eight", "red_nine",
		};
		for (unsigned int i = 0; i < 9; ++i) { // ���v�̐ԃh��
			if (RuleData::chkRule(tileRules[i], "character_suit") || // �ݎq
				RuleData::chkRule(tileRules[i], "character_circle_suit") ||
				RuleData::chkRule(tileRules[i], "character_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_CHARACTERS + i + 1]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "circle_suit") || // ���q
				RuleData::chkRule(tileRules[i], "character_circle_suit") ||
				RuleData::chkRule(tileRules[i], "circle_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_CIRCLES + i + 1]].red = AkaDora;
			if (RuleData::chkRule(tileRules[i], "bamboo_suit") || // ���q
				RuleData::chkRule(tileRules[i], "circle_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "character_bamboo_suit") ||
				RuleData::chkRule(tileRules[i], "all_three_suits"))
				gameStat->Deck[tilepos[TILE_SUIT_BAMBOOS + i + 1]].red = AkaDora;
		}
	}
	// 5�̃h���͓���
	if (chkGameType(gameStat, SanmaX)) { // �O��
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
	} else { // �l���E���v�O��
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
		for (unsigned int i = 0; i < 2; ++i) { // ���v�̐ԃh��
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
}

inline void bluetiles(GameTable* const gameStat, UInt8ByTile& tilepos) { // �h����ݒ肷��
	// TODO: �������������
}

inline void shuffletiles(GameTable* const gameStat, UInt8ByTile& tilepos, unsigned int tiles) { // ���v����
	// TODO: �������������
}

__declspec(dllexport) void shuffle(GameTable* const gameStat) { // �v���o�b�t�@�ɕ��ׂāA���v
	UInt8ByTile tilepos;
	unsigned int tiles = inittiles(gameStat, tilepos);
	redtiles(gameStat, tilepos);
	bluetiles(gameStat, tilepos);
	shuffletiles(gameStat, tilepos, tiles);
}
