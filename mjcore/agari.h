#pragma once

#include "gametbl.h"
#include "largenum.h"
#include "rational.h"
#include "endtype.h"
#include "yaku/yaku.h"

namespace endround {
namespace agari {

// -------------------------------------------------------------------------

typedef Rational<int> Rat;
typedef const Rat& rat;

LNum agaricalc(const LNum& AgariPointRaw, int agariBairitsu, int agariBairitsu2, unsigned agariCount);
#ifndef GUOBIAO
LNum agaricalc(const LNum& AgariPointRaw, rat agariBairitsu, rat agariBairitsu2, int agariCount, rat agariBairitsu3, int agariCount2);

inline void deltacalcplus(const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, int agariBairitsu, PlayerID targetPlayer) {
	PointDelta[targetPlayer] += (AgariPointRaw * agariBairitsu + 99) / 100 * 100;
	return;
}
inline void deltacalcminus(const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, int agariBairitsu, PlayerID targetPlayer) {
	PointDelta[targetPlayer] -= (AgariPointRaw * agariBairitsu + 99) / 100 * 100;
	return;
}
inline void deltacalcplus(const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, rat agariBairitsu, PlayerID targetPlayer) {
	PointDelta[targetPlayer] += (AgariPointRaw * agariBairitsu.getNumerator() / agariBairitsu.getDenominator() + 99) / 100 * 100;
	return;
}
inline void deltacalcminus(const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, rat agariBairitsu, PlayerID targetPlayer) {
	PointDelta[targetPlayer] -= (AgariPointRaw * agariBairitsu.getNumerator() / agariBairitsu.getDenominator() + 99) / 100 * 100;
	return;
}
#endif /* GUOBIAO */

const int CAP_normal = -1;
void calcAgariPoints(const GameTable* gameStat, LNum& agariPoint,
	const LNum& AgariPointRaw, InfoByPlayer<LNum>& PointDelta, int Mode = CAP_normal);

// -------------------------------------------------------------------------

void agariproc(EndType& RoundEndType, GameTable* gameStat, bool& tmpUraFlag, bool& tmpAliceFlag, CodeConv::tstring& ResultDesc);
void endround_agariproc(GameTable* gameStat, CodeConv::tstring& ResultDesc, PlayerID& AgariPlayerPriority,
	std::uint16_t origDoraPointer, const yaku::YAKUSTAT& yakuInfo, bool& tmpUraFlag, bool& tmpAliceFlag, int& OyaAgari);
void endround_chonboproc(GameTable* gameStat, CodeConv::tstring& ResultDesc);

// -------------------------------------------------------------------------

}
}
