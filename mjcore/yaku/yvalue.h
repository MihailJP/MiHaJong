#ifndef YVALUE_H
#define YVALUE_H

#include "yaku.h"

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

const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_none =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_1han =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_2han =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_3han =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_3han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_4han =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_4han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_5han =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_5han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_6han =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_6han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);

const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_mangan =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_mangan,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_baiman =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_baiman,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_yakuman =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_double_yakuman =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_triple_yakuman =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_triple_yakuman,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_quad_yakuman =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_quad_yakuman,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);

const yaku::yakuCalculator::Yaku::MenzenHan yaku::yakuCalculator::Yaku::yval_1han_menzen =
	yaku::yakuCalculator::Yaku::MenzenHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::MenzenHan yaku::yakuCalculator::Yaku::yval_2han_menzen =
	yaku::yakuCalculator::Yaku::MenzenHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::MenzenHan yaku::yakuCalculator::Yaku::yval_3han_menzen =
	yaku::yakuCalculator::Yaku::MenzenHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_3han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::MenzenHan yaku::yakuCalculator::Yaku::yval_4han_menzen =
	yaku::yakuCalculator::Yaku::MenzenHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_4han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::MenzenHan yaku::yakuCalculator::Yaku::yval_5han_menzen =
	yaku::yakuCalculator::Yaku::MenzenHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_5han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::MenzenHan yaku::yakuCalculator::Yaku::yval_6han_menzen =
	yaku::yakuCalculator::Yaku::MenzenHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_6han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);

const yaku::yakuCalculator::Yaku::MenzenHan yaku::yakuCalculator::Yaku::yval_mangan_menzen =
	yaku::yakuCalculator::Yaku::MenzenHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_mangan,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::MenzenHan yaku::yakuCalculator::Yaku::yval_baiman_menzen =
	yaku::yakuCalculator::Yaku::MenzenHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_baiman,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::MenzenHan yaku::yakuCalculator::Yaku::yval_yakuman_menzen =
	yaku::yakuCalculator::Yaku::MenzenHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::MenzenHan yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen =
	yaku::yakuCalculator::Yaku::MenzenHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);

const yaku::yakuCalculator::Yaku::KuisagariHan yaku::yakuCalculator::Yaku::yval_1han_kuisagari =
	yaku::yakuCalculator::Yaku::KuisagariHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::KuisagariHan yaku::yakuCalculator::Yaku::yval_2han_kuisagari =
	yaku::yakuCalculator::Yaku::KuisagariHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::KuisagariHan yaku::yakuCalculator::Yaku::yval_3han_kuisagari =
	yaku::yakuCalculator::Yaku::KuisagariHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_3han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::KuisagariHan yaku::yakuCalculator::Yaku::yval_4han_kuisagari =
	yaku::yakuCalculator::Yaku::KuisagariHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_4han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::KuisagariHan yaku::yakuCalculator::Yaku::yval_5han_kuisagari =
	yaku::yakuCalculator::Yaku::KuisagariHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_5han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);
const yaku::yakuCalculator::Yaku::KuisagariHan yaku::yakuCalculator::Yaku::yval_6han_kuisagari =
	yaku::yakuCalculator::Yaku::KuisagariHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_6han,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null);

const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_1han_dependent =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han);
const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_2han_dependent =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han);
const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_4han_dependent =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_4han);
const yaku::yakuCalculator::Yaku::FixedHan yaku::yakuCalculator::Yaku::yval_yakuman_dependent =
	yaku::yakuCalculator::Yaku::FixedHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);

const yaku::yakuCalculator::Yaku::MenzenHan yaku::yakuCalculator::Yaku::yval_1han_menzen_dependent =
	yaku::yakuCalculator::Yaku::MenzenHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han);
const yaku::yakuCalculator::Yaku::MenzenHan yaku::yakuCalculator::Yaku::yval_2han_menzen_dependent =
	yaku::yakuCalculator::Yaku::MenzenHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han);
const yaku::yakuCalculator::Yaku::MenzenHan yaku::yakuCalculator::Yaku::yval_yakuman_menzen_dependent =
	yaku::yakuCalculator::Yaku::MenzenHan(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
	yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);

#endif
