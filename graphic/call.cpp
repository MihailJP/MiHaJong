#include "call.h"

namespace mihajong_graphic {
namespace calltext {

namespace {
	CallType callStatus[PLAYERS] = {None, None, None, None,};
}

EXPORT CallType getCall(PLAYER_ID playerID) {
	if ((playerID < 0) || (playerID >= PLAYERS)) // ���E�`�F�b�N
		throw _T("playerID�̎w�肪����������܂���");
	return callStatus[playerID];
}

EXPORT void setCall(PLAYER_ID playerID, CallType callType) {
	if ((playerID < 0) || (playerID >= PLAYERS)) // ���E�`�F�b�N
		throw _T("playerID�̎w�肪����������܂���");
	callStatus[playerID] = callType;
}

}
}
