#pragma once

#ifdef MJCORE_EXPORTS
#include <cassert>
#include "strcode.h"
#include "logging.h"
#include "haifu.h"
#include "envtbl.h"
#include "ai/class.h"
#include "remote.h"
#include "../socket/socket.h"
#include "yaku/ykclass.h"
#include "yaku/yaku.h"
#include "../sound/sound.h"
#include "bgmid.h"
#include "fuuro.h"
#endif
#include "gametbl.h"
#include "endtype.h"
#include "discard.h"

DiscardTileNum getdahai(GameTable* const gameStat);
EndType procdahai(GameTable* const gameStat, DiscardTileNum& DiscardTileIndex);
void tsumoproc(GameTable* const gameStat);
