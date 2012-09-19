#include "prepare.h"

#define settile(tilecode, pos) {tilepos[tilecode] = pos; for (unsigned int i = 0; i < 4u; ++i) {gameStat->Deck[pos++].tile = tilecode;}}
__declspec(dllexport) void shuffle(GameTable* const gameStat) { // 牌をバッファに並べて、洗牌
	UInt8ByTile tilepos;
	{
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
	}
}
