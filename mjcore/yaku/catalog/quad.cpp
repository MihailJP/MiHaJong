#include "../catalog.h"
#include "../../../astro/astro.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_quad() {
	/* 超四喜 */
	if (RuleData::chkRuleApplied("chousixi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"超四喜", get_yaku_han("chousixi"),
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
		"四槓子", get_yaku_han("suukantsu"),
		"対々和",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return analysis->TotalKangzi == 4;
		}
	));
	/* 三暗槓 */
	if (RuleData::chkRuleApplied("sanankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三暗槓", get_yaku_han("sanankan"),
			"三槓子",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return analysis->TotalAnKangzi == 3;
			}
		));
	/* 大峡谷 */
	if (RuleData::chkRuleApplied("grand_canyon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"大峡谷", get_yaku_han("grand_canyon"),
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
	if (RuleData::chkRuleApplied("yukigeshou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"雪化粧", get_yaku_han("yukigeshou"),
			"役牌・白", "三槓子",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[CircleOne] >= 1) &&
					(analysis->KangziCount[CircleNine] >= 1) &&
					(analysis->KangziCount[WhiteDragon] >= 1));
			}
		));
	/* 三色同槓 */
	if (RuleData::chkRuleApplied("sanshoku_doukan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三色同槓", get_yaku_han("sanshoku_doukan"),
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
	if (RuleData::chkRuleApplied("chousangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"超三元", get_yaku_han("chousangen"),
			"大三元", "三槓子",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[WhiteDragon] >= 1) &&
					(analysis->KangziCount[GreenDragon] >= 1) &&
					(analysis->KangziCount[RedDragon] >= 1));
			}
		));
	/* 三槓子 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"三槓子", get_yaku_han("sankantsu"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return analysis->TotalKangzi == 3;
		}
	));
	/* 二暗槓 */
	if (RuleData::chkRuleApplied("ryanankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"二暗槓", get_yaku_han("ryanankan"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return analysis->TotalAnKangzi == 2;
			}
		));
	/* 声東撃西 */
	if (RuleData::chkRuleApplied("shengdong_jixi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"声東撃西", get_yaku_han("shengdong_jixi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[WestWind] >= 1));
			}
		));
	/* 走南闖北 */
	if (RuleData::chkRuleApplied("zaonan_chuangbei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"走南闖北", get_yaku_han("zaonan_chuangbei"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1));
			}
		));
	/* 暗中模索 */
	if (RuleData::chkRuleApplied("anchumosaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"暗中模索", get_yaku_han("anchumosaku"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->AnKangziCount[RedDragon] >= 1) &&
					(analysis->TsumoHai->tile / TILE_SUIT_STEP == TILE_SUIT_BAMBOOS / TILE_SUIT_STEP) &&
					(*analysis->TsumoAgariFlag));
			}
		));
	/* 戦車 */
	if (RuleData::chkRuleApplied("tank"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"戦車", get_yaku_han("tank"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[BambooSeven] >= 1);
			}
		));
	/* 真田六文銭 */
	if (RuleData::chkRuleApplied("sanadacoin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"真田六文銭", get_yaku_han("sanadacoin"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[CircleSix] >= 1);
			}
		));
	/* 三矢の誓い */
	if (RuleData::chkRuleApplied("three_arrows"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三矢の誓い", get_yaku_han("three_arrows"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[BambooThree] >= 1);
			}
		));
	/* 鬼は外 */
	if (RuleData::chkRuleApplied("setsubun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"鬼は外", get_yaku_han("setsubun"),
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
	if (RuleData::chkRuleApplied("souka"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"草加", get_yaku_han("souka"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < 9; i++)
					if (analysis->KaKangziCount[TILE_SUIT_BAMBOOS + i] >= 1)
						return true;
				return false;
			}
		));
	/* 合格祈願 */
	if (RuleData::chkRuleApplied("goukaku_kigan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"合格祈願", get_yaku_han("goukaku_kigan"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag1 = false; bool flag2 = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if (analysis->KangziCount[i + 5] >= 1) flag1 = true;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					if (analysis->KangziCount[i + 9] >= 1) flag2 = true;
				return flag1 && flag2;
			}
		));
}
