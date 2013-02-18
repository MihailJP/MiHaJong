#pragma once

#include "exports.h"
#include "../common/yakurslt.h"
#include "../common/mutex.h"

namespace mihajong_graphic {

class YakuResult {
private:
	static mihajong_structs::YakuResult myStat;
	static MHJMutex myMutex;
public:
	static mihajong_structs::YakuResult getYakuStat();
	EXPORT static void setYakuStat(const mihajong_structs::YakuResult*);
};

}
