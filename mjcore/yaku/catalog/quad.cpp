#include "../catalog.h"
#include "../../../astro/astro.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_quad() {
	/* 超四喜 */
	if (RuleData::getRule("chousixi") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"超四喜", yaku::yakuCalculator::Yaku::yval_triple_yakuman,
			"大四喜", "四槓子", "対々和", "声東撃西", "走南闖北",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[WestWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1));
			}
		));
	/* 四槓子 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"四槓子", ((RuleData::getRule("suukantsu") == 1) || (RuleData::getRule("suukantsu") == 3)) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman : yaku::yakuCalculator::Yaku::yval_yakuman,
		"対々和",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return analysis->TotalKangzi == 4;
		}
	));
	/* 三暗槓 */
	if (RuleData::getRule("sanankan") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三暗槓", yaku::yakuCalculator::Yaku::yval_yakuman,
			"三槓子",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return analysis->TotalAnKangzi == 3;
			}
		));
	/* 大峡谷 */
	if (RuleData::getRule("grand_canyon") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大峡谷", yaku::yakuCalculator::Yaku::yval_yakuman,
			"三槓子",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[CharacterNine] >= 1) &&
					(analysis->KangziCount[CircleNine] >= 1) &&
					(analysis->KangziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[CharacterEight] + analysis->DuiziCount[CircleEight] +
					analysis->DuiziCount[BambooEight] >= 2));
			}
		));
	/* 雪化粧 */
	if (RuleData::getRule("yukigeshou") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"雪化粧", yaku::yakuCalculator::Yaku::yval_yakuman,
			"役牌・白", "三槓子",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[CircleOne] >= 1) &&
					(analysis->KangziCount[CircleNine] >= 1) &&
					(analysis->KangziCount[WhiteDragon] >= 1));
			}
		));
	/* 三色同槓 */
	if (RuleData::getRule("sanshoku_doukan") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色同槓", (RuleData::getRule("sanshoku_doukan") == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_yakuman :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_5han_kuisagari,
			"三色同刻", "三槓子",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < 9; i++)
					if ((analysis->KangziCount[TILE_SUIT_CHARACTERS + i] >= 1) &&
						(analysis->KangziCount[TILE_SUIT_CIRCLES + i] >= 1) &&
						(analysis->KangziCount[TILE_SUIT_BAMBOOS + i] >= 1))
						return true;
				return false;
			}
		));
	/* 超三元 */
	if (RuleData::getRule("chousangen") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"超三元", yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"大三元", "三槓子",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[WhiteDragon] >= 1) &&
					(analysis->KangziCount[GreenDragon] >= 1) &&
					(analysis->KangziCount[RedDragon] >= 1));
			}
		));
	/* 三槓子 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"三槓子", (RuleData::getRule("sankantsu") == 2) ? yaku::yakuCalculator::Yaku::yval_yakuman :
		((RuleData::getRule("sankantsu") == 1) ? yaku::yakuCalculator::Yaku::yval_3han :
		yaku::yakuCalculator::Yaku::yval_2han),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return analysis->TotalKangzi == 3;
		}
	));
	/* 二暗槓 */
	if (RuleData::getRule("ryanankan") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"二暗槓", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return analysis->TotalAnKangzi == 2;
			}
		));
	/* 声東撃西 */
	if (RuleData::getRule("shengdong_jixi") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"声東撃西", yaku::yakuCalculator::Yaku::yval_6han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[WestWind] >= 1));
			}
		));
	/* 走南闖北 */
	if (RuleData::getRule("zaonan_chuangbei") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"走南闖北", yaku::yakuCalculator::Yaku::yval_6han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1));
			}
		));
	/* 暗中模索 */
	if (RuleData::getRule("anchumosaku") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"暗中模索", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->AnKangziCount[RedDragon] >= 1) &&
					(analysis->TsumoHai->tile / TILE_SUIT_STEP == TILE_SUIT_BAMBOOS / TILE_SUIT_STEP) &&
					(*analysis->TsumoAgariFlag));
			}
		));
	/* 戦車 */
	if (RuleData::getRule("tank") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"戦車", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[BambooSeven] >= 1);
			}
		));
	/* 真田六文銭 */
	if (RuleData::getRule("sanadacoin") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"真田六文銭", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[CircleSix] >= 1);
			}
		));
	/* 三矢の誓い */
	if (RuleData::getRule("three_arrows") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三矢の誓い", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[BambooThree] >= 1);
			}
		));
	/* 鬼は外 */
	if (RuleData::getRule("setsubun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"鬼は外", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				TIME_ZONE_INFORMATION Zeitzone; SYSTEMTIME Zeit; // 宣言。こんな変数名付ける私は厨二病かもしれない
				GetTimeZoneInformation(&Zeitzone); // タイムゾーンを取得する
				GetLocalTime(&Zeit); // WINAPIを使ってローカル時刻を取得
				SYSTEMTIME HeuteMitternacht = {Zeit.wYear, Zeit.wMonth, Zeit.wDayOfWeek, Zeit.wDay, 0, 0, 0, 0}; // 当日0時のSYSTEMTIME
				double JulianischeDatum = systime_to_julian(&HeuteMitternacht) - ((double)Zeitzone.Bias / 1440.0); // ローカル時間で当日0時のユリウス日
				return /* 翌日が立春か判定する。立春とは太陽が黄経315度の子午線を通過する日である。 */
					(sun_ecliptic_longitude(JulianischeDatum + 1.0) <= 315.0) &&
					(sun_ecliptic_longitude(JulianischeDatum + 2.0) >= 315.0) &&
					(analysis->KangziCount[CircleNine] >= 1); /* 手牌自体は九筒の槓子だけが条件 */
			}
		));
	/* 草加 */
	if (RuleData::getRule("souka") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"草加", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < 9; i++)
					if (analysis->KaKangziCount[TILE_SUIT_BAMBOOS + i] >= 1)
						return true;
				return false;
			}
		));
}
