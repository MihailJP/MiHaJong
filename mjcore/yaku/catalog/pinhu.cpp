﻿#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_pinhu()
{
#ifndef GUOBIAO
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("平和"), 1_hanF /* 門前であるかどうかの判定は既に行なっているので不要 */,
		[](const MentsuAnalysis* const analysis) {
			return analysis->isPinfu;
		}
	));
	if (RuleData::chkRuleApplied("kofuku_gekijoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("鼓腹撃壌"), get_yaku_han("kofuku_gekijoh") /* 門前であるかどうかの判定は既に行なっているので不要 */,
			_T("平和"),
			[](const MentsuAnalysis* const analysis) {
				return (analysis->isPinfu &&
					(analysis->TsumoHai().tile == TileCode::circleOne));
			}
		));
#endif /* GUOBIAO */
}
