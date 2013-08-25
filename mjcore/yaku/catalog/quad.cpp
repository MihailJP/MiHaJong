#include "../catalog.h"
#include "../../../astro/astro.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_quad() {
#ifndef GUOBIAO
	/* ���l�� */
	if (RuleData::chkRuleApplied("chousixi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���l��"), get_yaku_han("chousixi"),
			_T("��l��"), _T("�l�Ȏq"), _T("�΁X�a"), _T("��������"), _T("����菖k"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[WestWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1));
			}
		));
#endif /* GUOBIAO */
	/* �l�Ȏq */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("�l�Y"), yaku::yakuCalculator::Yaku::yval_88,
		_T("\u78b0\u78b0�a"),
#else /* GUOBIAO */
		_T("�l�Ȏq"), get_yaku_han("suukantsu"),
		_T("�΁X�a"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return analysis->TotalKangzi == 4;
		}
	));
#ifndef GUOBIAO
	/* �O�Þ� */
	if (RuleData::chkRuleApplied("sanankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�Þ�"), get_yaku_han("sanankan"),
			_T("�O�Ȏq"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return analysis->TotalAnKangzi == 3;
			}
		));
	/* �勬�J */
	if (RuleData::chkRuleApplied("grand_canyon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�勬�J"), get_yaku_han("grand_canyon"),
			_T("�O�Ȏq"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[CharacterNine] >= 1) &&
					(analysis->KangziCount[CircleNine] >= 1) &&
					(analysis->KangziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[CharacterEight] + analysis->DuiziCount[CircleEight] +
					analysis->DuiziCount[BambooEight] >= 2));
			}
		));
	/* �ቻ�� */
	if (RuleData::chkRuleApplied("yukigeshou"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�ቻ��"), get_yaku_han("yukigeshou"),
			_T("��v�E��"), _T("�O�Ȏq"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[CircleOne] >= 1) &&
					(analysis->KangziCount[CircleNine] >= 1) &&
					(analysis->KangziCount[WhiteDragon] >= 1));
			}
		));
	/* �O�F���� */
	if (RuleData::chkRuleApplied("sanshoku_doukan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O�F����"), get_yaku_han("sanshoku_doukan"),
			_T("�O�F����"), _T("�O�Ȏq"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < 9; i++)
					if ((analysis->KangziCount[TileSuitCharacters + i] >= 1) &&
						(analysis->KangziCount[TileSuitCircles + i] >= 1) &&
						(analysis->KangziCount[TileSuitBamboos + i] >= 1))
						return true;
				return false;
			}
		));
	/* ���O�� */
	if (RuleData::chkRuleApplied("chousangen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���O��"), get_yaku_han("chousangen"),
			_T("��O��"), _T("�O�Ȏq"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[WhiteDragon] >= 1) &&
					(analysis->KangziCount[GreenDragon] >= 1) &&
					(analysis->KangziCount[RedDragon] >= 1));
			}
		));
#endif /* GUOBIAO */
	/* �O�Ȏq */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("�O�Y"), yaku::yakuCalculator::Yaku::yval_32,
#else /* GUOBIAO */
		_T("�O�Ȏq"), get_yaku_han("sankantsu"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return analysis->TotalKangzi == 3;
		}
	));
#ifdef GUOBIAO
	/* �o�ÞY */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�o�ÞY"), yaku::yakuCalculator::Yaku::yval_8,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalKangzi == 2) && (analysis->TotalAnKangzi == 2);
		}
	));
	/* �o���Y */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�o���Y"), yaku::yakuCalculator::Yaku::yval_4,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalKangzi == 2) && (analysis->TotalAnKangzi < 2);
		}
	));
	/* �ÞY */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�ÞY"), yaku::yakuCalculator::Yaku::yval_2,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalKangzi == 1) && (analysis->TotalAnKangzi == 1);
		}
	));
	/* ���Y */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("���Y"), yaku::yakuCalculator::Yaku::yval_1,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->TotalKangzi == 1) && (analysis->TotalAnKangzi == 0);
		}
	));
#else /* GUOBIAO */
	/* ��Þ� */
	if (RuleData::chkRuleApplied("ryanankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��Þ�"), get_yaku_han("ryanankan"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return analysis->TotalAnKangzi == 2;
			}
		));
	/* �������� */
	if (RuleData::chkRuleApplied("shengdong_jixi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��������"), get_yaku_han("shengdong_jixi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[WestWind] >= 1));
			}
		));
	/* ����菖k */
	if (RuleData::chkRuleApplied("zaonan_chuangbei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����菖k"), get_yaku_han("zaonan_chuangbei"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1));
			}
		));
	/* �Ò��͍� */
	if (RuleData::chkRuleApplied("anchumosaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�Ò��͍�"), get_yaku_han("anchumosaku"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->AnKangziCount[RedDragon] >= 1) &&
					(analysis->TsumoHai->tile / TileSuitStep == TileSuitBamboos / TileSuitStep) &&
					(*analysis->TsumoAgariFlag));
			}
		));
	/* ��� */
	if (RuleData::chkRuleApplied("tank"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���"), get_yaku_han("tank"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[BambooSeven] >= 1);
			}
		));
	/* �^�c�Z���K */
	if (RuleData::chkRuleApplied("sanadacoin"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�^�c�Z���K"), get_yaku_han("sanadacoin"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[CircleSix] >= 1);
			}
		));
	/* �O��̐��� */
	if (RuleData::chkRuleApplied("three_arrows"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�O��̐���"), get_yaku_han("three_arrows"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[BambooThree] >= 1);
			}
		));
	/* �S�͊O */
	if (RuleData::chkRuleApplied("setsubun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�S�͊O"), get_yaku_han("setsubun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
#ifdef _WIN32
				TIME_ZONE_INFORMATION Zeitzone; SYSTEMTIME Zeit; // �錾�B����ȕϐ����t���鎄�͐~��a��������Ȃ�
				GetTimeZoneInformation(&Zeitzone); // �^�C���]�[�����擾����
				GetLocalTime(&Zeit); // WINAPI���g���ă��[�J���������擾
				SYSTEMTIME HeuteMitternacht = {Zeit.wYear, Zeit.wMonth, Zeit.wDayOfWeek, Zeit.wDay, 0, 0, 0, 0}; // ����0����SYSTEMTIME
				double JulianischeDatum = systime_to_julian(&HeuteMitternacht) - ((double)Zeitzone.Bias / 1440.0); // ���[�J�����Ԃœ���0���̃����E�X��
#else /*_WIN32*/
				const signed long Zeitzone = []() -> signed long {
					time_t t1 = 86400; // GNU C�͂����ł͂Ȃ����Atime_t��unsigned�̏����n���������Ƃ�����̂�86400�Ƃ���
					tm* tmDat = gmtime(&t1); // ���萢�E�����Z�o
					time_t t2 = mktime(tmDat); // �킴�ƒn�����Ɖ��߂��邱�ƂŎ��������߂�
					return t1 - t2; // �b�P�ʂŎ�����Ԃ��B���{���Ԃ�������32400�ƂȂ�
				}();
				timespec Zeitzahl; tm Zeit, ZeitMitternacht;
				clock_gettime(CLOCK_REALTIME, &Zeitzahl);
				memcpy(&Zeit, localtime(&Zeitzahl.tv_sec), sizeof (tm));
				memcpy(&ZeitMitternacht, localtime(&Zeitzahl.tv_sec), sizeof (tm));
				timespec HeuteMitternacht; memset (&HeuteMitternacht, 0, sizeof (timespec));
				ZeitMitternacht.tm_year = Zeit.tm_year;
				ZeitMitternacht.tm_mon = Zeit.tm_mon;
				ZeitMitternacht.tm_mday = Zeit.tm_mday;
				HeuteMitternacht.tv_sec = mktime(&ZeitMitternacht);
				double JulianischeDatum = systime_to_julian(&HeuteMitternacht) - ((double)(Zeitzone / 60) / 1440.0);
#endif /*_WIN32*/
				return /* ���������t�����肷��B���t�Ƃ͑��z�����o315�x�̎q�ߐ���ʉ߂�����ł���B */
					(sun_ecliptic_longitude(JulianischeDatum + 1.0) <= 315.0) &&
					(sun_ecliptic_longitude(JulianischeDatum + 2.0) >= 315.0) &&
					(analysis->KangziCount[CircleNine] >= 1); /* ��v���̂͋㓛�̞Ȏq���������� */
			}
		));
	/* ���� */
	if (RuleData::chkRuleApplied("souka"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����"), get_yaku_han("souka"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < 9; i++)
					if (analysis->KaKangziCount[TileSuitBamboos + i] >= 1)
						return true;
				return false;
			}
		));
	/* ���i�F�� */
	if (RuleData::chkRuleApplied("goukaku_kigan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���i�F��"), get_yaku_han("goukaku_kigan"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool flag1 = false; bool flag2 = false;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
					if (analysis->KangziCount[i + 5] >= 1) flag1 = true;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
					if (analysis->KangziCount[i + 9] >= 1) flag2 = true;
				return flag1 && flag2;
			}
		));
	/* �V�z�J */
	if (RuleData::chkRuleApplied("shinkoshigaya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�V�z�J"), get_yaku_han("shinkoshigaya"),
			_T("�z�J"), /* �O�Ȏq�ƕ��� */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KangziCount[CharacterFive] + analysis->KangziCount[CircleFive] + analysis->KangziCount[BambooFive] >= 1) &&
					(analysis->KangziCount[CharacterFour] + analysis->KangziCount[CircleFour] + analysis->KangziCount[BambooFour] >= 1) &&
					(analysis->KangziCount[CharacterEight] + analysis->KangziCount[CircleEight] + analysis->KangziCount[BambooEight] >= 1);
			}
		));
#endif /* GUOBIAO */
}
