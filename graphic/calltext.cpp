#include "calltext.h"

#include <Windows.h>
namespace mihajong_graphic {

namespace {
	class CallStatusAccessorMutex {
	private:
		CRITICAL_SECTION myCS;
	public:
		CallStatusAccessorMutex() {InitializeCriticalSection(&myCS);}
		~CallStatusAccessorMutex() {DeleteCriticalSection(&myCS);}
		void acquire() {EnterCriticalSection(&myCS);}
		void release() {LeaveCriticalSection(&myCS);}
	};
	CallStatusAccessorMutex csMutex;
}

namespace calltext {

namespace {
	CallType callStatus[PLAYERS] = {None, None, None, None,};
}

EXPORT CallType getCall(PLAYER_ID playerID) {
	if ((playerID < 0) || (playerID >= PLAYERS)) // 境界チェック
		throw _T("playerIDの指定が正しくありません");
	csMutex.acquire();
	CallType ans = callStatus[playerID];
	csMutex.release();
	return ans;
}

EXPORT void setCall(PLAYER_ID playerID, CallType callType) {
	if ((playerID < 0) || (playerID >= PLAYERS)) // 境界チェック
		throw _T("playerIDの指定が正しくありません");
	csMutex.acquire();
	callStatus[playerID] = callType;
	csMutex.release();
}

}
namespace callvalue {

namespace {
	CallValue callStatus[PLAYERS] = {{0, 0u}, {0, 0u}, {0, 0u}, {0, 0u},};
}

EXPORT CallValue getVal(PLAYER_ID playerID) {
	if ((playerID < 0) || (playerID >= PLAYERS)) // 境界チェック
		throw _T("playerIDの指定が正しくありません");
	csMutex.acquire();
	CallValue ans = callStatus[playerID];
	csMutex.release();
	return ans;
}

EXPORT void setVal(PLAYER_ID playerID, signed short mantissa, unsigned short exponent) {
	if ((playerID < 0) || (playerID >= PLAYERS)) // 境界チェック
		throw _T("playerIDの指定が正しくありません");
	csMutex.acquire();
	callStatus[playerID].Mantissa = mantissa;
	callStatus[playerID].Exponent = exponent;
	csMutex.release();
}

}
}
