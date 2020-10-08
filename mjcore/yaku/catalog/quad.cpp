#include "../catalog.h"
#include "../../../astro/astro.h"
#include "../../../common/strcode.h"
#include "../../../common/datetime.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_quad() {
#ifndef GUOBIAO
	/* 超四喜 */
	if (RuleData::chkRuleApplied("chousixi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("超四喜"), get_yaku_han("chousixi"),
			_T("大四喜"), _T("四槓子"), _T("対々和"), _T("声東撃西"), _T("走南闖北"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[WestWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1));
			}
		));
#endif /* GUOBIAO */
	/* 四槓子 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("四杠"), yaku::yakuCalculator::Yaku::yval_88,
		PengPengHu, _T("単調将"),
#else /* GUOBIAO */
		_T("四槓子"), get_yaku_han("suukantsu"),
		_T("対々和"),
#endif /* GUOBIAO */
		[](const MentsuAnalysis* const analysis) -> bool {
			return analysis->TotalKangzi == 4;
		}
	));
#ifndef GUOBIAO
	/* 三暗槓 */
	if (RuleData::chkRuleApplied("sanankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三暗槓"), get_yaku_han("sanankan"),
			_T("三槓子"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return analysis->TotalAnKangzi == 3;
			}
		));
	/* 大峡谷 */
	if (RuleData::chkRuleApplied("grand_canyon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("大峡谷"), get_yaku_han("grand_canyon"),
			_T("三槓子"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KangziCount[CharacterNine] >= 1) &&
					(analysis->KangziCount[CircleNine] >= 1) &&
					(analysis->KangziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[CharacterEight] + analysis->DuiziCount[CircleEight] +
					analysis->DuiziCount[BambooEight] >= 2));
			}
		));
	/* 雪化粧 */
	if (RuleData::chkRuleApplied("yukigeshou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("雪化粧"), get_yaku_han("yukigeshou"),
			_T("役牌・白"), _T("三槓子"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KangziCount[CircleOne] >= 1) &&
					(analysis->KangziCount[CircleNine] >= 1) &&
					(analysis->KangziCount[WhiteDragon] >= 1));
			}
		));
	/* 三色同槓 */
	if (RuleData::chkRuleApplied("sanshoku_doukan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三色同槓"), get_yaku_han("sanshoku_doukan"),
			_T("三色同刻"), _T("三槓子"),
			[](const MentsuAnalysis* const analysis) -> bool {
				for (int i = 1; i < 9; i++)
					if ((analysis->KangziCount[TileSuitCharacters + i] >= 1) &&
						(analysis->KangziCount[TileSuitCircles + i] >= 1) &&
						(analysis->KangziCount[TileSuitBamboos + i] >= 1))
						return true;
				return false;
			}
		));
	/* 超三元 */
	if (RuleData::chkRuleApplied("chousangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("超三元"), get_yaku_han("chousangen"),
			_T("大三元"), _T("三槓子"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KangziCount[WhiteDragon] >= 1) &&
					(analysis->KangziCount[GreenDragon] >= 1) &&
					(analysis->KangziCount[RedDragon] >= 1));
			}
		));
#endif /* GUOBIAO */
	/* 三槓子 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("三杠"), yaku::yakuCalculator::Yaku::yval_32,
#else /* GUOBIAO */
		_T("三槓子"), get_yaku_han("sankantsu"),
#endif /* GUOBIAO */
		[](const MentsuAnalysis* const analysis) -> bool {
			return analysis->TotalKangzi == 3;
		}
	));
#ifdef GUOBIAO
	/* 双暗杠 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("双暗杠"), yaku::yakuCalculator::Yaku::yval_8,
		[](const MentsuAnalysis* const analysis) -> bool {
			return (analysis->TotalKangzi >= 2) && (analysis->TotalAnKangzi == 2);
		}
	));
	/* 双明杠 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("双明杠"), yaku::yakuCalculator::Yaku::yval_4,
		[](const MentsuAnalysis* const analysis) -> bool {
			return (analysis->TotalKangzi == 2) && (analysis->TotalAnKangzi < 2);
		}
	));
	/* 暗杠 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("暗杠"), yaku::yakuCalculator::Yaku::yval_2,
		[](const MentsuAnalysis* const analysis) -> bool {
			return (analysis->TotalKangzi >= 1) && (analysis->TotalAnKangzi == 1);
		}
	));
	/* 明杠 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("明杠"), yaku::yakuCalculator::Yaku::yval_1,
		[](const MentsuAnalysis* const analysis) -> bool {
			return (analysis->TotalKangzi == 1) && (analysis->TotalAnKangzi == 0);
		}
	));
#else /* GUOBIAO */
	/* 二暗槓 */
	if (RuleData::chkRuleApplied("ryanankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二暗槓"), get_yaku_han("ryanankan"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return analysis->TotalAnKangzi == 2;
			}
		));
	/* 声東撃西 */
	if (RuleData::chkRuleApplied("shengdong_jixi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("声東撃西"), get_yaku_han("shengdong_jixi"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[WestWind] >= 1));
			}
		));
	/* 走南闖北 */
	if (RuleData::chkRuleApplied("zaonan_chuangbei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("走南闖北"), get_yaku_han("zaonan_chuangbei"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1));
			}
		));
	/* 暗中模索 */
	if (RuleData::chkRuleApplied("anchumosaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("暗中模索"), get_yaku_han("anchumosaku"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return ((analysis->AnKangziCount[RedDragon] >= 1) &&
					(analysis->TsumoHai->tile / TileSuitStep == TileSuitBamboos / TileSuitStep) &&
					(*analysis->TsumoAgariFlag));
			}
		));
	/* 戦車 */
	if (RuleData::chkRuleApplied("tank"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("戦車"), get_yaku_han("tank"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KangziCount[BambooSeven] >= 1);
			}
		));
	/* 真田六文銭 */
	if (RuleData::chkRuleApplied("sanadacoin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("真田六文銭"), get_yaku_han("sanadacoin"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KangziCount[CircleSix] >= 1);
			}
		));
	/* 三矢の誓い */
	if (RuleData::chkRuleApplied("three_arrows"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三矢の誓い"), get_yaku_han("three_arrows"),
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KangziCount[BambooThree] >= 1);
			}
		));
	/* 鬼は外 */
	if (RuleData::chkRuleApplied("setsubun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("鬼は外"), get_yaku_han("setsubun"),
			[](const MentsuAnalysis* const analysis) -> bool {
				using namespace DateTime;
				const Time Zeit(localTime());
				int Zeitzone(timeZone());
				const Time HeuteMitternacht = {Zeit.year, Zeit.month, Zeit.day, Zeit.dayOfWeek, 0, 0, 0, 0}; // 当日0時のTime::Time
				const double JulianischeDatum = getJulian(HeuteMitternacht) - (static_cast<double>(Zeitzone) / 1440.0);
				return /* 翌日が立春か判定する。立春とは太陽が黄経315度の子午線を通過する日である。 */
					(sun_ecliptic_longitude(JulianischeDatum + 1.0) <= 315.0) &&
					(sun_ecliptic_longitude(JulianischeDatum + 2.0) >= 315.0) &&
					(analysis->KangziCount[CircleNine] >= 1); /* 手牌自体は九筒の槓子だけが条件 */
			}
		));
	/* 草加 */
	if (RuleData::chkRuleApplied("souka"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("草加"), get_yaku_han("souka"),
			[](const MentsuAnalysis* const analysis) -> bool {
				for (int i = 1; i < 9; i++)
					if (analysis->KaKangziCount[TileSuitBamboos + i] >= 1)
						return true;
				return false;
			}
		));
	/* 合格祈願 */
	if (RuleData::chkRuleApplied("goukaku_kigan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("合格祈願"), get_yaku_han("goukaku_kigan"),
			[](const MentsuAnalysis* const analysis) -> bool {
				bool flag1 = false; bool flag2 = false;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
					if (analysis->KangziCount[i + 5] >= 1) flag1 = true;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
					if (analysis->KangziCount[i + 9] >= 1) flag2 = true;
				return flag1 && flag2;
			}
		));
	/* 新越谷 */
	if (RuleData::chkRuleApplied("shinkoshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("新越谷"), get_yaku_han("shinkoshigaya"),
			_T("越谷"), /* 三槓子と複合 */
			[](const MentsuAnalysis* const analysis) -> bool {
				return (analysis->KangziCount[CharacterFive] + analysis->KangziCount[CircleFive] + analysis->KangziCount[BambooFive] >= 1) &&
					(analysis->KangziCount[CharacterFour] + analysis->KangziCount[CircleFour] + analysis->KangziCount[BambooFour] >= 1) &&
					(analysis->KangziCount[CharacterEight] + analysis->KangziCount[CircleEight] + analysis->KangziCount[BambooEight] >= 1);
			}
		));
#endif /* GUOBIAO */
}
