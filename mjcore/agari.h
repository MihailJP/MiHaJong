#pragma once

#include "../common/largenum.h"
#include "gametbl.h"
#include "rational.h"
#include "endtype.h"
#include "yaku/yaku.h"

namespace endround {
namespace agari {

// -------------------------------------------------------------------------

typedef Rational<int> Rat;
typedef const Rat& rat;

LargeNum agaricalc(const LargeNum& AgariPointRaw, int agariBairitsu, int agariBairitsu2, unsigned agariCount);
#ifndef GUOBIAO
LargeNum agaricalc(const LargeNum& AgariPointRaw, rat agariBairitsu, rat agariBairitsu2, int agariCount, rat agariBairitsu3, int agariCount2);

inline void deltacalcplus(const LargeNum& AgariPointRaw, InfoByPlayer<LargeNum>& PointDelta, int agariBairitsu, PlayerID targetPlayer) {
	PointDelta[targetPlayer] += (AgariPointRaw * agariBairitsu + 99) / 100 * 100;
	return;
}
inline void deltacalcminus(const LargeNum& AgariPointRaw, InfoByPlayer<LargeNum>& PointDelta, int agariBairitsu, PlayerID targetPlayer) {
	PointDelta[targetPlayer] -= (AgariPointRaw * agariBairitsu + 99) / 100 * 100;
	return;
}
inline void deltacalcplus(const LargeNum& AgariPointRaw, InfoByPlayer<LargeNum>& PointDelta, rat agariBairitsu, PlayerID targetPlayer) {
	PointDelta[targetPlayer] += (AgariPointRaw * agariBairitsu.getNumerator() / agariBairitsu.getDenominator() + 99) / 100 * 100;
	return;
}
inline void deltacalcminus(const LargeNum& AgariPointRaw, InfoByPlayer<LargeNum>& PointDelta, rat agariBairitsu, PlayerID targetPlayer) {
	PointDelta[targetPlayer] -= (AgariPointRaw * agariBairitsu.getNumerator() / agariBairitsu.getDenominator() + 99) / 100 * 100;
	return;
}
#endif /* GUOBIAO */

const int CAP_normal = -1;
void calcAgariPoints(const GameTable* gameStat, LargeNum& agariPoint,
	const LargeNum& AgariPointRaw, InfoByPlayer<LargeNum>& PointDelta, int Mode = CAP_normal);

// -------------------------------------------------------------------------

void agariproc(EndType& RoundEndType, GameTable* gameStat, bool& tmpUraFlag, bool& tmpAliceFlag, CodeConv::tstring& ResultDesc);
void endround_agariproc(GameTable* gameStat, CodeConv::tstring& ResultDesc, PlayerID& AgariPlayerPriority,
	std::uint16_t origDoraPointer, const yaku::YAKUSTAT& yakuInfo, bool& tmpUraFlag, bool& tmpAliceFlag, int& OyaAgari);
void endround_chonboproc(GameTable* gameStat, CodeConv::tstring& ResultDesc);

// -------------------------------------------------------------------------

}
}
