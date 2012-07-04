#ifndef TILEUTIL_H
#define TILEUTIL_H

#ifdef MJCORE_EXPORTS
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cassert>
#include <type_traits>
#endif
#include <cstdint>
#include "mjexport.h"
#include "gametbl.h"
#ifdef MJCORE_EXPORTS
#include "logging.h"
#include "mnzdat.h"
#include "func.h"
#endif

// -------------------------------------------------------------------------

EXPORT_STRUCT MachihaiTileInfo {
	bool MachihaiFlag; // 待ち牌になっているかのフラグ
	int8_t MachihaiCount; // 待ち牌の残り枚数
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<MachihaiTileInfo>::value, "MachihaiTileInfo is not POD");
#endif

EXPORT_STRUCT MachihaiInfo { // 待ち牌とかの情報を格納(chkFuriten関数用)
	bool FuritenFlag; // フリテンかどうかのフラグ
	MachihaiTileInfo Machihai[TILE_NONFLOWER_MAX]; // 待ち牌情報
	int8_t MachihaiTotal; // 待ち牌の合計枚数
	int8_t MachiMen; // 何面待ち？
};
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<MachihaiInfo>::value, "MachihaiInfo is not POD");
#endif

// -------------------------------------------------------------------------

#ifdef MJCORE_EXPORTS
void lipai(GameTable* const gameStat, PLAYER_ID targetPlayer);
__declspec(dllexport) void lipai(GameTable* const gameStat, int targetPlayer);

void chkOpenMachi(GameTable* const gameStat, PLAYER_ID targetPlayer);
__declspec(dllexport) void chkOpenMachi(GameTable* const gameStat, int targetPlayer);
#endif

#endif
