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
	EndType checkroundabort(GameTable* gameStat); // �ǏI�������̔���
	void endround(GameTable* gameStat, EndType roundEndType, unsigned OrigTurn, unsigned OrigHonba);
	bool nextRound(GameTable* gameStat, EndType RoundEndType, unsigned int OrigTurn); // ���̋ǂ�(�I������ꍇ��true)

	namespace transfer {
		void resetDelta();
		void addDelta(PLAYER_ID player, LargeNum& deltaVal);
		void transferPoints(GameTable* gameStat, unsigned subscene, unsigned wait);
	}
}
