#ifndef FUURO_H
#define FUURO_H

#ifdef MJCORE_EXPORTS
#include <sstream>
#include <cassert>
#include "strcode.h"
#include "logging.h"
#include "haifu.h"
#include "ruletbl.h"
#include "tileutil.h"
#include "discard.h"
#endif
#include "gametbl.h"
#include "endtype.h"
#include "discard.h"

enum FuuroType : unsigned { /* 副露処理の種類 */
	FuuroAnkan,     // 暗槓
	FuuroKakan,     // 加槓
	FuuroDaiminkan, // 大明槓
	FuuroNorth,     // 抜き北(三麻)
	FuuroFlower,    // 花牌(四麻)
	FuuroPon,       // ポン
	FuuroChii,      // チー
};

bool fuuroproc(GameTable* const gameStat, EndType* RoundEndType, const DiscardTileNum& DiscardTileIndex, FuuroType Mode);

#endif
