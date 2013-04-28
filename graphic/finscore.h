#pragma once

#include <array>
#include "../common/largenum.h"
#include "../common/gametbl.h"
#include "../common/mutex.h"
#include "exports.h"

namespace mihajong_graphic {

#ifdef GRAPHIC_EXPORTS
class FinalScoreDat {
private:
	static MHJMutex myMutex;
	static std::array<mihajong_structs::LargeNum, mihajong_structs::Players> scoreDat;
public:
	static mihajong_structs::LargeNum getData(mihajong_structs::PlayerID player);
	static void setData(mihajong_structs::LargeNum, mihajong_structs::LargeNum, mihajong_structs::LargeNum, mihajong_structs::LargeNum);
};
#endif

EXPORT void setFinalScore(mihajong_structs::LargeNum, mihajong_structs::LargeNum, mihajong_structs::LargeNum, mihajong_structs::LargeNum);

}
