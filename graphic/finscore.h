#pragma once

#include <array>
#include "../common/mutex.h"
#include "../common/largenum.h"
#include "../common/gametbl.h"
#include "exports.h"

namespace mihajong_graphic {

#ifdef GRAPHIC_EXPORTS
class FinalScoreDat {
private:
	static MUTEXLIB::recursive_mutex myMutex;
	static std::array<mihajong_structs::LargeNum, mihajong_structs::Players> scoreDat;
public:
	static mihajong_structs::LargeNum getData(mihajong_structs::PlayerID player);
	static void setData(mihajong_structs::LargeNum, mihajong_structs::LargeNum, mihajong_structs::LargeNum, mihajong_structs::LargeNum);
public: /* Monostate class: cannot be instantiated */
	FinalScoreDat() = delete;
	FinalScoreDat(const FinalScoreDat&) = delete;
	FinalScoreDat& operator= (const FinalScoreDat&) = delete;
	~FinalScoreDat() = delete;
};
#endif

EXPORT void setFinalScore(mihajong_structs::LargeNum, mihajong_structs::LargeNum, mihajong_structs::LargeNum, mihajong_structs::LargeNum);

}
