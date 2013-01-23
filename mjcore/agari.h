#pragma once

#include "gametbl.h"
#include "largenum.h"
#include "rational.h"
#include "endtype.h"

namespace endround {
namespace agari {

// -------------------------------------------------------------------------

typedef Rational<int> Rat;
typedef const Rat& rat;

LNum agaricalc(const LNum& AgariPointRaw, int agariBairitsu, int agariBairitsu2, unsigned agariCount);
LNum agaricalc(const LNum& AgariPointRaw, rat agariBairitsu, rat agariBairitsu2, int agariCount, rat agariBairitsu3, int agariCount2);

inline void deltacalcplus(const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, int agariBairitsu, PLAYER_ID targetPlayer) {
	PointDelta[targetPlayer] += (AgariPointRaw * agariBairitsu + 99) / 100 * 100;
	return;
}
inline void deltacalcminus(const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, int agariBairitsu, PLAYER_ID targetPlayer) {
	PointDelta[targetPlayer] -= (AgariPointRaw * agariBairitsu + 99) / 100 * 100;
	return;
}
inline void deltacalcplus(const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, rat agariBairitsu, PLAYER_ID targetPlayer) {
	PointDelta[targetPlayer] += (AgariPointRaw * agariBairitsu.getNumerator() / agariBairitsu.getDenominator() + 99) / 100 * 100;
	return;
}
inline void deltacalcminus(const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, rat agariBairitsu, PLAYER_ID targetPlayer) {
	PointDelta[targetPlayer] -= (AgariPointRaw * agariBairitsu.getNumerator() / agariBairitsu.getDenominator() + 99) / 100 * 100;
	return;
}

const int CAP_normal = -1;
void calcAgariPoints(const GameTable* gameStat, LNum& agariPoint,
	const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, int Mode = CAP_normal);

// -------------------------------------------------------------------------

void agariproc(EndType& RoundEndType, GameTable* gameStat, bool& tmpUraFlag, bool& tmpAliceFlag, CodeConv::tstring& ResultDesc);
void endround_chonboproc(GameTable* gameStat, CodeConv::tstring& ResultDesc);

// -------------------------------------------------------------------------

}
}
