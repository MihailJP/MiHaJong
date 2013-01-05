#pragma once

#include "endtype.h"
#include "gametbl.h"
#include "ruletbl.h"
#include "func.h"
#include "logging.h"
#include <functional>
#include <array>
#include "../sound/sound.h"
#include "bgmid.h"
#include "../graphic/graphic.h"
#include "tileutil.h"
#include "haifu.h"

namespace endround {
	EndType checkroundabort(GameTable* gameStat); // ‹ÇI—¹ğŒ‚Ì”»’è
	void endround(const GameTable* gameStat, EndType roundEndType, unsigned OrigTurn, unsigned OrigHonba);
}
