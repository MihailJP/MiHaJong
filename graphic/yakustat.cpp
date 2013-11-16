#include "yakustat.h"
#include <cstring>

namespace mihajong_graphic {

std::recursive_mutex YakuResult::myMutex;
mihajong_structs::YakuResult YakuResult::myStat;
mihajong_structs::LargeNum YakuResult::agariScore;
int YakuResult::chipVal;

mihajong_structs::YakuResult YakuResult::getYakuStat() {
	std::unique_lock<std::recursive_mutex> lock(myMutex);
	return myStat;
}

mihajong_structs::LargeNum YakuResult::getAgariScore() {
	std::unique_lock<std::recursive_mutex> lock(myMutex);
	return agariScore;
}

int YakuResult::getChipVal() {
	std::unique_lock<std::recursive_mutex> lock(myMutex);
	return chipVal;
}

void YakuResult::setYakuStat(const mihajong_structs::YakuResult* res, const mihajong_structs::LargeNum score, int chip) {
	std::unique_lock<std::recursive_mutex> lock(myMutex);
	memcpy(&myStat, res, sizeof (mihajong_structs::YakuResult));
	memcpy(&agariScore, &score, sizeof (mihajong_structs::LargeNum));
	chipVal = chip;
}

}
