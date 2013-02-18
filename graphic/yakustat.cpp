#include "yakustat.h"
#include <cstring>

namespace mihajong_graphic {

mihajong_structs::YakuResult YakuResult::myStat;
MHJMutex YakuResult::myMutex;

mihajong_structs::YakuResult YakuResult::getYakuStat() {
	return myMutex.syncDo<mihajong_structs::YakuResult>([]() {return myStat;});
}

void YakuResult::setYakuStat(const mihajong_structs::YakuResult* res) {
	myMutex.syncDo<void>([res]() -> void {
		memcpy(&myStat, res, sizeof (mihajong_structs::YakuResult));
	});
}

}
