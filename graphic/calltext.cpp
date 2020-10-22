#include "calltext.h"

#include <mutex>
#include "except.h"

namespace mihajong_graphic {

namespace {
	std::recursive_mutex csMutex;
}

namespace calltext {

namespace {
	CallType callStatus[Players] = {CallType::none, CallType::none, CallType::none, CallType::none,};
}

EXPORT CallType getCall(PlayerID playerID) {
	if ((playerID < 0) || (playerID >= Players)) // 境界チェック
		throw InvalidPlayerCode("playerIDの指定が正しくありません");
	std::unique_lock<std::recursive_mutex> lock(csMutex);
	return callStatus[playerID];
}

EXPORT void setCall(PlayerID playerID, CallType callType) {
	if ((playerID < 0) || (playerID >= Players)) // 境界チェック
		throw InvalidPlayerCode("playerIDの指定が正しくありません");
	std::unique_lock<std::recursive_mutex> lock(csMutex);
	callStatus[playerID] = callType;
}

}
namespace callvalue {

namespace {
	CallValue callStatus[Players] = {{0, 0u}, {0, 0u}, {0, 0u}, {0, 0u},};
}

EXPORT CallValue getVal(PlayerID playerID) {
	if ((playerID < 0) || (playerID >= Players)) // 境界チェック
		throw InvalidPlayerCode("playerIDの指定が正しくありません");
	std::unique_lock<std::recursive_mutex> lock(csMutex);
	return callStatus[playerID];
}

EXPORT void setVal(PlayerID playerID, signed short mantissa, unsigned short exponent) {
	if ((playerID < 0) || (playerID >= Players)) // 境界チェック
		throw InvalidPlayerCode("playerIDの指定が正しくありません");
	std::unique_lock<std::recursive_mutex> lock(csMutex);
	callStatus[playerID].Mantissa = mantissa;
	callStatus[playerID].Exponent = exponent;
}

}
}
