#include "../catalog.h"
#include "../../../astro/astro.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_quad() {
	/* ���l�� */
	if (RuleData::getRule("chousixi") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���l��", yaku::yakuCalculator::Yaku::yval_triple_yakuman,
			"��l��", "�l�Ȏq", "�΁X�a", "��������", "����菖k",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[WestWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1));
			}
		));
	/* �l�Ȏq */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�l�Ȏq", ((RuleData::getRule("suukantsu") == 1) || (RuleData::getRule("suukantsu") == 3)) ?
		yaku::yakuCalculator::Yaku::yval_double_yakuman : yaku::yakuCalculator::Yaku::yval_yakuman,
		"�΁X�a",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return analysis->TotalKangzi == 4;
		}
	));
	/* �O�Þ� */
	if (RuleData::getRule("sanankan") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�Þ�", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�O�Ȏq",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return analysis->TotalAnKangzi == 3;
			}
		));
	/* �勬�J */
	if (RuleData::getRule("grand_canyon") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�勬�J", yaku::yakuCalculator::Yaku::yval_yakuman,
			"�O�Ȏq",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[CharacterNine] >= 1) &&
					(analysis->KangziCount[CircleNine] >= 1) &&
					(analysis->KangziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[CharacterEight] + analysis->DuiziCount[CircleEight] +
					analysis->DuiziCount[BambooEight] >= 2));
			}
		));
	/* �ቻ�� */
	if (RuleData::getRule("yukigeshou") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ቻ��", yaku::yakuCalculator::Yaku::yval_yakuman,
			"��v�E��", "�O�Ȏq",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[CircleOne] >= 1) &&
					(analysis->KangziCount[CircleNine] >= 1) &&
					(analysis->KangziCount[WhiteDragon] >= 1));
			}
		));
	/* �O�F���� */
	if (RuleData::getRule("sanshoku_doukan") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O�F����", (RuleData::getRule("sanshoku_doukan") == 2) ?
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_yakuman :
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_5han_kuisagari,
			"�O�F����", "�O�Ȏq",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < 9; i++)
					if ((analysis->KangziCount[TILE_SUIT_CHARACTERS + i] >= 1) &&
						(analysis->KangziCount[TILE_SUIT_CIRCLES + i] >= 1) &&
						(analysis->KangziCount[TILE_SUIT_BAMBOOS + i] >= 1))
						return true;
				return false;
			}
		));
	/* ���O�� */
	if (RuleData::getRule("chousangen") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���O��", yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"��O��", "�O�Ȏq",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[WhiteDragon] >= 1) &&
					(analysis->KangziCount[GreenDragon] >= 1) &&
					(analysis->KangziCount[RedDragon] >= 1));
			}
		));
	/* �O�Ȏq */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�O�Ȏq", (RuleData::getRule("sankantsu") == 2) ? yaku::yakuCalculator::Yaku::yval_yakuman :
		((RuleData::getRule("sankantsu") == 1) ? yaku::yakuCalculator::Yaku::yval_3han :
		yaku::yakuCalculator::Yaku::yval_2han),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return analysis->TotalKangzi == 3;
		}
	));
	/* ��Þ� */
	if (RuleData::getRule("ryanankan") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��Þ�", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return analysis->TotalAnKangzi == 2;
			}
		));
	/* �������� */
	if (RuleData::getRule("shengdong_jixi") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��������", yaku::yakuCalculator::Yaku::yval_6han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[WestWind] >= 1));
			}
		));
	/* ����菖k */
	if (RuleData::getRule("zaonan_chuangbei") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����菖k", yaku::yakuCalculator::Yaku::yval_6han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1));
			}
		));
	/* �Ò��͍� */
	if (RuleData::getRule("anchumosaku") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�Ò��͍�", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->AnKangziCount[RedDragon] >= 1) &&
					(analysis->TsumoHai->tile / TILE_SUIT_STEP == TILE_SUIT_BAMBOOS / TILE_SUIT_STEP) &&
					(*analysis->TsumoAgariFlag));
			}
		));
	/* ��� */
	if (RuleData::getRule("tank") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[BambooSeven] >= 1);
			}
		));
	/* �^�c�Z���K */
	if (RuleData::getRule("sanadacoin") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�^�c�Z���K", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[CircleSix] >= 1);
			}
		));
	/* �O��̐��� */
	if (RuleData::getRule("three_arrows") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�O��̐���", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[BambooThree] >= 1);
			}
		));
	/* �S�͊O */
	if (RuleData::getRule("setsubun") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�S�͊O", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				TIME_ZONE_INFORMATION Zeitzone; SYSTEMTIME Zeit; // �錾�B����ȕϐ����t���鎄�͐~��a��������Ȃ�
				GetTimeZoneInformation(&Zeitzone); // �^�C���]�[�����擾����
				GetLocalTime(&Zeit); // WINAPI���g���ă��[�J���������擾
				SYSTEMTIME HeuteMitternacht = {Zeit.wYear, Zeit.wMonth, Zeit.wDayOfWeek, Zeit.wDay, 0, 0, 0, 0}; // ����0����SYSTEMTIME
				double JulianischeDatum = systime_to_julian(&HeuteMitternacht) - ((double)Zeitzone.Bias / 1440.0); // ���[�J�����Ԃœ���0���̃����E�X��
				return /* ���������t�����肷��B���t�Ƃ͑��z�����o315�x�̎q�ߐ���ʉ߂�����ł���B */
					(sun_ecliptic_longitude(JulianischeDatum + 1.0) <= 315.0) &&
					(sun_ecliptic_longitude(JulianischeDatum + 2.0) >= 315.0) &&
					(analysis->KangziCount[CircleNine] >= 1); /* ��v���̂͋㓛�̞Ȏq���������� */
			}
		));
	/* ���� */
	if (RuleData::getRule("souka") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"����", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < 9; i++)
					if (analysis->KaKangziCount[TILE_SUIT_BAMBOOS + i] >= 1)
						return true;
				return false;
			}
		));
}
