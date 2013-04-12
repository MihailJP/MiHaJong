#pragma once

#include <cstdint>
#include "mjexport.h"
#include "gametbl.h"
#include "../common/shanten.h"

#define SHANTEN_PAGES mihajong_structs::ShantenPages

void LoadFileInResource(int name, int type, DWORD& size, const char*& data);

#ifdef MJCORE_EXPORTS
MJCORE Int8ByTile countTilesInHand(const GameTable* const gameStat, PlayerID playerID); // External
#endif

class ShantenAnalyzer {
private:
	static uint8_t* mentsuAnalysisDat;

public:
#ifdef MJCORE_EXPORTS
	static void initMentsuAnalysisDat();
#endif

	static MJCORE Shanten calcShanten(const GameTable* const gameStat, PlayerID playerID, ShantenType mode);
#ifdef MJCORE_EXPORTS
private:
	static void setQixingTilePattern(TileCode* const QixingPai, unsigned int pattern);
	static unsigned int chkMianzi(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount, unsigned limit);
	static Shanten calcShantenRegular(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount);
	static Shanten calcShantenChiitoi(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount);
	static Shanten calcShantenKokushi(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount);
	static Shanten calcShantenStellar(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount, bool qixing);
	static Shanten calcShantenCivilWar(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount);
	static Shanten calcShantenSyzygy(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount);
	static Shanten calcShantenSevenup(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount);
	static Shanten calcShantenZuhelong(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount);
#endif

};
