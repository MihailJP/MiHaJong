#pragma once

#include "exports.h"
#include "../common/yakurslt.h"
#include "../common/mutex.h"

namespace mihajong_graphic {

class YakuResult {
private:
	static MHJMutex myMutex;
	static mihajong_structs::YakuResult myStat;
	static mihajong_structs::LargeNum agariScore;
	static int chipVal;
public:
	static mihajong_structs::YakuResult getYakuStat();
	static mihajong_structs::LargeNum getAgariScore();
	static int getChipVal();
	EXPORT static void setYakuStat(const mihajong_structs::YakuResult*, mihajong_structs::LargeNum, int);
};

}
