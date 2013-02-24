#pragma once

#include "exports.h"
#include "../common/yakurslt.h"
#include "../common/mutex.h"

namespace mihajong_graphic {

class YakuResult {
private:
	static mihajong_structs::YakuResult myStat;
	static MHJMutex myMutex;
	static mihajong_structs::LargeNum agariScore;
public:
	static mihajong_structs::YakuResult getYakuStat();
	static mihajong_structs::LargeNum getAgariScore();
	EXPORT static void setYakuStat(const mihajong_structs::YakuResult*, mihajong_structs::LargeNum);
};

}
