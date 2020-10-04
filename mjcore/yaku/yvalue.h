#pragma once

#include "yaku.h"

#ifdef GUOBIAO

#define DEF_YAKU_VAL(i) const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_##i = \
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(i); \
	const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_##i = \
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_##i, \
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN());

DEF_YAKU_VAL(1);
DEF_YAKU_VAL(2);
DEF_YAKU_VAL(3);
DEF_YAKU_VAL(4);
DEF_YAKU_VAL(6);
DEF_YAKU_VAL(8);
DEF_YAKU_VAL(12);
DEF_YAKU_VAL(16);
DEF_YAKU_VAL(24);
DEF_YAKU_VAL(32);
DEF_YAKU_VAL(48);
DEF_YAKU_VAL(64);
DEF_YAKU_VAL(88);

#else /* GUOBIAO */

const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN();
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(1, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(2, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_3han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(3, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_4han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(4, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_5han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(5, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_6han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(6, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_7han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(7, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_8han =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(8, Han);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_mangan =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(2, SemiMangan);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_haneman =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(3, SemiMangan);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_baiman =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(4, SemiMangan);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_3baiman =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(6, SemiMangan);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(1, Yakuman);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(2, Yakuman);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_triple_yakuman =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(3, Yakuman);
const yaku::yakuCalculator::Yaku::YAKU_HAN::HAN yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_quad_yakuman =
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN(4, Yakuman);

#endif /* GUOBIAO */
