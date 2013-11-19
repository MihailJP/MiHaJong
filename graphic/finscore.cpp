#include "finscore.h"

namespace mihajong_graphic {

using namespace mihajong_structs;

MUTEXLIB::recursive_mutex FinalScoreDat::myMutex;
std::array<LargeNum, Players> FinalScoreDat::scoreDat;

LargeNum FinalScoreDat::getData(PlayerID player) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(myMutex);
	return scoreDat[player];
}

void FinalScoreDat::setData(LargeNum score1, LargeNum score2, LargeNum score3, LargeNum score4) {
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(myMutex);
	scoreDat[0] = score1; scoreDat[1] = score2;
	scoreDat[2] = score3; scoreDat[3] = score4;
}

EXPORT void setFinalScore(LargeNum score1, LargeNum score2, LargeNum score3, LargeNum score4) {
	FinalScoreDat::setData(score1, score2, score3, score4);
}

}
