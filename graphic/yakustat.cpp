#include "yakustat.h"
#include <cstring>

namespace mihajong_graphic {

MHJMutex YakuResult::myMutex;
mihajong_structs::YakuResult YakuResult::myStat;
mihajong_structs::LargeNum YakuResult::agariScore;
int YakuResult::chipVal;

mihajong_structs::YakuResult YakuResult::getYakuStat() {
	return myMutex.syncDo<mihajong_structs::YakuResult>([]() {return myStat;});
}

mihajong_structs::LargeNum YakuResult::getAgariScore() {
	return myMutex.syncDo<mihajong_structs::LargeNum>([]() {return agariScore;});
}

int YakuResult::getChipVal() {
	return myMutex.syncDo<int>([]() {return chipVal;});
}

void YakuResult::setYakuStat(const mihajong_structs::YakuResult* res, const mihajong_structs::LargeNum score, int chip) {
	myMutex.syncDo<void>([res, &score, chip]() -> void {
		memcpy(&myStat, res, sizeof (mihajong_structs::YakuResult));
		memcpy(&agariScore, &score, sizeof (mihajong_structs::LargeNum));
		chipVal = chip;
	});
}

}
