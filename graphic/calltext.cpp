#include "calltext.h"

#include "../common/mutex.h"
namespace mihajong_graphic {

namespace {
	MUTEXLIB::recursive_mutex csMutex;
}

namespace calltext {

namespace {
	CallType callStatus[Players] = {None, None, None, None,};
}

EXPORT CallType getCall(PlayerID playerID) {
	if ((playerID < 0) || (playerID >= Players)) // 境界チェック
		throw _T("playerIDの指定が正しくありません");
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(csMutex);
	return callStatus[playerID];
}

EXPORT void setCall(PlayerID playerID, CallType callType) {
	if ((playerID < 0) || (playerID >= Players)) // 境界チェック
		throw _T("playerIDの指定が正しくありません");
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(csMutex);
	callStatus[playerID] = callType;
}

}
namespace callvalue {

namespace {
	CallValue callStatus[Players] = {{0, 0u}, {0, 0u}, {0, 0u}, {0, 0u},};
}

EXPORT CallValue getVal(PlayerID playerID) {
	if ((playerID < 0) || (playerID >= Players)) // 境界チェック
		throw _T("playerIDの指定が正しくありません");
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(csMutex);
	return callStatus[playerID];
}

EXPORT void setVal(PlayerID playerID, signed short mantissa, unsigned short exponent) {
	if ((playerID < 0) || (playerID >= Players)) // 境界チェック
		throw _T("playerIDの指定が正しくありません");
	MUTEXLIB::unique_lock<MUTEXLIB::recursive_mutex> lock(csMutex);
	callStatus[playerID].Mantissa = mantissa;
	callStatus[playerID].Exponent = exponent;
}

}
}
