#pragma once

#include "gametbl.h"
#include "endtype.h"
#include "discard.h"

enum class FuuroType : unsigned { /* 副露処理の種類 */
	ankan,     // 暗槓
	kakan,     // 加槓
	daiminkan, // 大明槓
	north,     // 抜き北(三麻)
	flower,    // 花牌(四麻)
	pon,       // ポン
	chii,      // チー
};

bool fuuroproc(GameTable* const gameStat, EndType* RoundEndType, const DiscardTileNum& DiscardTileIndex, FuuroType Mode);
void checkpao(GameTable* const gameStat);
void askReaction(GameTable* const gameStat);
EndType ronhuproc(GameTable* const gameStat);
bool executeFuuro(GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex);
