#include "call.h"

namespace mihajong_graphic {
namespace calltext {

namespace {
	CallType callStatus[PLAYERS] = {None, None, None, None,};
}

EXPORT CallType getCall(PLAYER_ID playerID) {
	if ((playerID < 0) || (playerID >= PLAYERS)) // 境界チェック
		throw _T("playerIDの指定が正しくありません");
	return callStatus[playerID];
}

EXPORT void setCall(PLAYER_ID playerID, CallType callType) {
	if ((playerID < 0) || (playerID >= PLAYERS)) // 境界チェック
		throw _T("playerIDの指定が正しくありません");
	callStatus[playerID] = callType;
}

}
}
