#include "yakustat.h"
#include <cstring>

namespace mihajong_graphic {

mihajong_structs::YakuResult YakuResult::myStat;
MHJMutex YakuResult::myMutex;
mihajong_structs::LargeNum YakuResult::agariScore;

mihajong_structs::YakuResult YakuResult::getYakuStat() {
	return myMutex.syncDo<mihajong_structs::YakuResult>([]() {return myStat;});
}

mihajong_structs::LargeNum YakuResult::getAgariScore() {
	return myMutex.syncDo<mihajong_structs::LargeNum>([]() {return agariScore;});
}

void YakuResult::setYakuStat(const mihajong_structs::YakuResult* res, const mihajong_structs::LargeNum score) {
	myMutex.syncDo<void>([res, &score]() -> void {
		memcpy(&myStat, res, sizeof (mihajong_structs::YakuResult));
		memcpy(&agariScore, &score, sizeof (mihajong_structs::LargeNum));
	});
}

}
