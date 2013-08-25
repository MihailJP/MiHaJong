#include "../catalog.h"

static const TileCode YaojiuShunCode[] = {
	CharacterOne, CharacterSeven, CircleOne, CircleSeven, BambooOne, BambooSeven,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
};
static const TileCode OneCode[] = {CharacterOne, CircleOne, BambooOne,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon};
static const TileCode SevenCode[] = {CharacterSeven, CircleSeven, BambooSeven};
static const TileCode NineCode[] = {CharacterNine, CircleNine, BambooNine,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon};

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_yaojiu()
{
	/* �^�����I */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("�f��"), yaku::yakuCalculator::Yaku::yval_2,
		_T("�َ�"),
#else /* GUOBIAO */
		_T("�f���"), (RuleData::chkRule("kuitan") == "yes_but_unbound") ? /* ��O�Ȃ甛��𖞂����A�򂢒f�͔���𖞂����Ȃ����[�� */
		yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return *analysis->MenzenFlag ?
					yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null) :
					yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han);
			}) :
			((RuleData::chkRule("kuitan") == "no") ? (yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han_menzen : // �򂢒f�Ȃ�
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han), // �򂢒f����
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles(), 13, YaojiuShunCode, 13, false) == 0);
			else if (analysis->shanten[shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles(), 13) == 0);
			else return false;
		}
	));
	/* �`�����^ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("�S�ћ�"), yaku::yakuCalculator::Yaku::yval_4,
#else /* GUOBIAO */
		_T("���S�ћ��"), yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, Honor_Major_Tiles(), 13, YaojiuShunCode, 13, false) == SizeOfMeldBuffer);
			else return false;
		}
	));
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("henchantaiyao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�ΑS�ћ��"), yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			_T("���S�ћ��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[shantenRegular] == -1)
					return ( (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, OneCode, 10, OneCode, 3, false) == SizeOfMeldBuffer) ||
					(yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, NineCode, 10, SevenCode, 3, false) == SizeOfMeldBuffer) );
				else return false;
			}
		));
	/* ���`���� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("���S�ћ��"), yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
		_T("���S�ћ��"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, Honor_Major_Tiles(), 6, YaojiuShunCode, 6, false) == SizeOfMeldBuffer);
			else return false;
		}
	));
	if (RuleData::chkRuleApplied("henchantaiyao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���ΑS�ћ��"), yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			_T("���S�ћ��"), _T("�ΑS�ћ��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[shantenRegular] == -1)
					return ( (yaku::countingFacility::countSpecMentz(analysis->MianziDat, OneCode, 3, OneCode, 3, false) == SizeOfMeldBuffer) ||
					(yaku::countingFacility::countSpecMentz(analysis->MianziDat, NineCode, 3, SevenCode, 3, false) == SizeOfMeldBuffer) );
				else return false;
			}
		));
#endif /* GUOBIAO */
	/* �z�����[ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("�����"), yaku::yakuCalculator::Yaku::yval_32,
		_T("��㍏"), _T("��㍏x2"), _T("��㍏x3"), _T("��㍏x4"), _T("\u78b0\u78b0�a"), _T("�S�ћ�"),
#else /* GUOBIAO */
		_T("���V��"), get_yaku_han("honroutou"),
		_T("���S�ћ��"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles(), 13, nullptr, 0, false) == SizeOfMeldBuffer);
			else if (analysis->shanten[shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles(), 13) == NumOfTilesInHand / 2);
			else return false;
		}
	));
	/* �`�����[ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("�����"), yaku::yakuCalculator::Yaku::yval_32,
		_T("�َ�"), _T("�o����"), _T("�O����"), _T("��㍏"), _T("��㍏x2"), _T("��㍏x3"), _T("��㍏x4"), _T("\u78b0\u78b0�a"), _T("�S�ћ�"),
#else /* GUOBIAO */
		_T("���V��"), yaku::yakuCalculator::Yaku::yval_yakuman,
		_T("���S�ћ��"), _T("���S�ћ��"), _T("���V��"), _T("�΁X�a"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles(), 6, nullptr, 0, false) == SizeOfMeldBuffer);
			else return false;
		}
	));
	/* �c�[�C�[�\�[ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("����F"), yaku::yakuCalculator::Yaku::yval_64,
		_T("��㍏"), _T("��㍏x2"), _T("��㍏x3"), _T("��㍏x4"), _T("\u78b0\u78b0�a"), _T("�S�ћ�"),
#else /* GUOBIAO */
		_T("����F"), yaku::yakuCalculator::Yaku::yval_yakuman,
		_T("���S�ћ��"), _T("���S�ћ��"), _T("���V��"), _T("����F"), _T("�l����"), _T("�΁X�a"), _T("����F"), _T("�O�����Ύq"), _T("�l�쎵�Ύq"), _T("�g������"),
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles()+6, 7, nullptr, 0, false) == SizeOfMeldBuffer);
			else if (analysis->shanten[shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles()+6, 7) == NumOfTilesInHand / 2);
			else return false;
		}
	));
#ifndef GUOBIAO
	if (RuleData::chkRuleApplied("four_honors"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�l����"), get_yaku_han("four_honors"),
			_T("���S�ћ��"), _T("���S�ћ��"), _T("���V��"), _T("����F"), _T("����F"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[shantenRegular] == -1)
					return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles()+6, 7, nullptr, 0, true) == (SizeOfMeldBuffer - 1));
				else return false;
			}
		));
#endif /* GUOBIAO */
#ifdef GUOBIAO
	/* �َ� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("�َ�"), yaku::yakuCalculator::Yaku::yval_1,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles()+6, 7, nullptr, 0, false) == 0);
			else if (analysis->shanten[shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles()+6, 7) == 0);
			else return false;
		}
	));
#endif /* GUOBIAO */
}
