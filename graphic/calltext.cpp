#include "calltext.h"

#include "../common/mutex.h"
namespace mihajong_graphic {

namespace {
	MHJMutex csMutex;
}

namespace calltext {

namespace {
	CallType callStatus[PLAYERS] = {None, None, None, None,};
}

EXPORT CallType getCall(PLAYER_ID playerID) {
	if ((playerID < 0) || (playerID >= PLAYERS)) // 境界チェック
		throw _T("playerIDの指定が正しくありません");
	return csMutex.syncDo<CallType>([playerID]() {
		return callStatus[playerID];
	});
}

EXPORT void setCall(PLAYER_ID playerID, CallType callType) {
	if ((playerID < 0) || (playerID >= PLAYERS)) // 境界チェック
		throw _T("playerIDの指定が正しくありません");
	csMutex.syncDo<void>([playerID, callType]() -> void {
		callStatus[playerID] = callType;
	});
}

}
namespace callvalue {

namespace {
	CallValue callStatus[PLAYERS] = {{0, 0u}, {0, 0u}, {0, 0u}, {0, 0u},};
}

EXPORT CallValue getVal(PLAYER_ID playerID) {
	if ((playerID < 0) || (playerID >= PLAYERS)) // 境界チェック
		throw _T("playerIDの指定が正しくありません");
	return csMutex.syncDo<CallValue>([playerID]() {
		return callStatus[playerID];
	});
}

EXPORT void setVal(PLAYER_ID playerID, signed short mantissa, unsigned short exponent) {
	if ((playerID < 0) || (playerID >= PLAYERS)) // 境界チェック
		throw _T("playerIDの指定が正しくありません");
	csMutex.syncDo<void>([playerID, mantissa, exponent]() -> void {
		callStatus[playerID].Mantissa = mantissa;
		callStatus[playerID].Exponent = exponent;
	});
}

}
}
