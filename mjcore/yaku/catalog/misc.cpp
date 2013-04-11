#include "../catalog.h"

extern GameTable GameStat;

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_misc() {
	auto chktiles = // �ʎq��E���Ύq���p�̔���֐��I�u�W�F�N�g
		[](const MENTSU_ANALYSIS* const analysis, const TileCode* const targetKez, int numOfKez,
		const TileCode* const targetShunz, int numOfShunz, bool noDui) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentzWithDup
					(analysis->MianziDat, targetKez, numOfKez, targetShunz, numOfShunz, noDui)
					== SizeOfMeldBuffer - (noDui ? 1 : 0));
			else if (analysis->shanten[shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, targetKez, numOfKez)
					== NumOfTilesInHand / 2);
			else return false;
		};
	auto chktiles_nodup = // �ʎq��E���Ύq���p�̔���֐��I�u�W�F�N�g
		[](const MENTSU_ANALYSIS* const analysis, const TileCode* const targetKez, int numOfKez,
		const TileCode* const targetShunz, int numOfShunz, bool noDui) -> bool {
			if (analysis->shanten[shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz
					(analysis->MianziDat, targetKez, numOfKez, targetShunz, numOfShunz, noDui)
					== SizeOfMeldBuffer - (noDui ? 1 : 0));
			else if (analysis->shanten[shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, targetKez, numOfKez)
					== NumOfTilesInHand / 2);
			else return false;
		};

	// ---------------------------------------------------------------------

	/* �Έ�F */
	{
		auto allgrean =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight, GreenDragon};
				return chktiles(analysis, kezi, 6, kezi, 1, false);
			};
		if (RuleData::chkRuleApplied("dragonless_all_green"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�Έ�F"), yaku::yakuCalculator::Yaku::yval_yakuman,
				_T("����F"), _T("����F"), _T("�f�g�a"), _T("���f�g"),
				[allgrean](const MENTSU_ANALYSIS* const analysis) -> bool {
					return allgrean(analysis) && (analysis->TileCount[GreenDragon] > 0);
				}
			));
		else if (GameStat.chkGameType(SanmaS))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�Έ�F"), yaku::yakuCalculator::Yaku::yval_yakuman,
				_T("����F"), _T("����F"), _T("����F"), _T("�f�g�a"), _T("���f�g"),
				allgrean
			));
		else // ᢂȂ��ł��ǂ����[��
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�Έ�F"), yaku::yakuCalculator::Yaku::yval_yakuman,
				_T("����F"), _T("����F"), _T("�f�g�a"), _T("���f�g"), _T("���f�g"), // ����F�͕�����
				allgrean
			));
		if (RuleData::chkRule("dragonless_all_green", "double_yakuman")) // ᢂȂ����_�u���𖞂ɂȂ郋�[��
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				GameStat.chkGameType(SanmaS) ? _T("���Έ�F") : _T("�Έ�F"), yaku::yakuCalculator::Yaku::yval_double_yakuman,
				_T("����F"), _T("����F"), _T("�f�g�a"), _T("���f�g"),
				[allgrean](const MENTSU_ANALYSIS* const analysis) -> bool {
					return allgrean(analysis) && (analysis->TileCount[GreenDragon] == 0);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* ���P�R�P�ʂƂ������[�J���� */
	{
		auto chkHadakaTanki =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int count = 0;
				for (int i = 1; i < SizeOfMeldBuffer; i++)
					if ((analysis->MianziDat[i].mstat != meldSequenceConcealed) &&
						(analysis->MianziDat[i].mstat != meldTripletConcealed) &&
						(analysis->MianziDat[i].mstat != meldQuadConcealed)) ++count;
				return (count == SizeOfMeldBuffer - 1);
			};
		if (RuleData::chkRuleApplied("shiiaru_raotai")) {
			if (!RuleData::chkRule("shiiaru_raotai", "1han_ron_only"))
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					_T("�S�|��"), yaku::yakuCalculator::Yaku::yval_1han,
					[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
						return chkHadakaTanki(analysis) && analysis->GameStat->TsumoAgariFlag;
					}
				));
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�S���l"), yaku::yakuCalculator::Yaku::yval_1han,
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (!analysis->GameStat->TsumoAgariFlag);
				}
			));
		}
		// ���{�Ɨ��c����҂�
		if (RuleData::chkRuleApplied("kinkei_dokuritsu"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("���{�Ɨ�"), get_yaku_han("kinkei_dokuritsu"),
				_T("�S�|��"), _T("�S���l"),
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (analysis->TsumoHai->tile == BambooOne);
				}
			));
		// �ƒފ��]��c���҂�
		if (RuleData::chkRuleApplied("kankou_no_yuki"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�ƒފ��]��"), get_yaku_han("kankou_no_yuki"),
				_T("�S�|��"), _T("�S���l"),
				[chkHadakaTanki](const MENTSU_ANALYSIS* const analysis) -> bool {
					return chkHadakaTanki(analysis) && (analysis->TsumoHai->tile == WhiteDragon);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* ���s�| */
	if (RuleData::chkRuleApplied("toipuutao")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���s�|"), get_yaku_han("toipuutao"),
			_T("����"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CircleOne, CircleTwo, CircleThree, BambooFour, CircleFour, CircleFive, CircleEight,
					CircleNine, BambooTwo, BambooFive, BambooSix, BambooEight, BambooNine, WhiteDragon,
				};
				return chktiles(analysis, kezi, 14, kezi, 4, false);
			}
		));
	}
	/* �g�E�� */
	if (RuleData::chkRuleApplied("benikujaku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�g�E��"), get_yaku_han("benikujaku"),
			_T("����F"), _T("�΁X�a"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					BambooOne, BambooFive, BambooSeven, BambooNine, RedDragon,
				};
				return chktiles(analysis, kezi, 5, nullptr, 0, false);
			}
		));
	/* �ݗΑp����_�g */
	if (RuleData::chkRuleApplied("kouitten"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�g��_"), get_yaku_han("kouitten"),
			_T("����F"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight, RedDragon,
				};
				return chktiles(analysis, kezi, 6, kezi, 1, false) &&
					(analysis->TileCount[RedDragon] > 0);
			}
		));
	/* ���L��Έ�F */
	if (RuleData::chkRuleApplied("all_green_with_white")) {
		auto allgreen_white =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight, WhiteDragon,
				};
				return chktiles(analysis, kezi, 6, kezi, 1, false) &&
					(analysis->TileCount[WhiteDragon] > 0);
			};
		if (RuleData::chkRule("all_green_with_white", "2han"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("����F"), yaku::yakuCalculator::Yaku::yval_2han,
				allgreen_white
			));
		else if (RuleData::chkRule("all_green_with_white", "yakuman"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("���L��Έ�F"), yaku::yakuCalculator::Yaku::yval_yakuman,
				_T("����F"),
				allgreen_white
			));
	}
	/* �Έ�F�� */
	if (RuleData::chkRuleApplied("ryuuiisohrin")) {
		auto allgreen_five =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
				};
				return chktiles(analysis, kezi, 5, kezi, 1, true) &&
					(analysis->MianziDat[0].tile == BambooFive);
			};
		if (GameStat.chkGameType(SanmaS))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�Έ�F��"), get_yaku_han("ryuuiisohrin"),
				_T("����F"),
				allgreen_five
			));
		else
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�Έ�F��"), get_yaku_han("ryuuiisohrin"),
				allgreen_five
			));
	}
	/* �呐�� */
	if (RuleData::chkRuleApplied("daisougen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�呐��"), get_yaku_han("daisougen"),
			_T("����F"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive,
					BambooSix, BambooSeven, BambooNine,
				};
				return chktiles(analysis, kezi, 7, kezi, 4, false);
			}
		));
	/* ��呐�� */
	if (RuleData::chkRuleApplied("south_daisougen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��呐��"), get_yaku_han("south_daisougen"),
			_T("����F"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooFive,
					BambooSix, BambooSeven, BambooNine, SouthWind,
				};
				return chktiles(analysis, kezi, 8, kezi, 4, false) &&
					(analysis->TileCount[SouthWind] > 0);
			}
		));
	/* ����F */
	if (RuleData::chkRuleApplied("taoyise"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����F"), get_yaku_han("taoyise"),
			_T("����F"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->shanten[shantenRegular] == -1) && (analysis->KeziCount[RedDragon] >= 1))
					yakuFlag = true;
				else if ((analysis->shanten[shantenPairs] == -1) && (analysis->TileCount[RedDragon] >= 2))
					yakuFlag = true;
				const TileCode kezi[] = {
					CircleFive, CircleOne, CircleThree, CircleSix,
					CircleSeven, CircleNine, RedDragon,
				};
				return chktiles(analysis, kezi, 7, kezi, 1, false) &&
					((countRedTilesInHand(analysis->GameStat, analysis->player, AkaDora))[CircleFive] > 0) &&
					yakuFlag;
			}
		));
	/* ����F */
	if (RuleData::chkRuleApplied("chayise"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����F"), get_yaku_han("chayise"),
			_T("����F"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
					EastWind, SouthWind, WestWind, NorthWind,
					WhiteDragon, GreenDragon,
				};
				return chktiles(analysis, kezi, 11, kezi, 1, false);
			}
		));
	/* ����F */
	if (RuleData::chkRuleApplied("heiiisoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����F"), get_yaku_han("heiiisoh"),
			_T("����F"), _T("�΁X�a"), _T("����"), _T("�f�g�a"), _T("���f�g"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CircleTwo, CircleFour, CircleEight,
					EastWind, SouthWind, WestWind, NorthWind,
				};
				return chktiles(analysis, kezi, 7, nullptr, 0, false);
			}
		));
	/* ���� */
	if (RuleData::chkRuleApplied("ao_no_domon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����"), get_yaku_han("ao_no_domon"),
			_T("����F"), _T("�΁X�a"), _T("�f�g�a"), _T("���f�g"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[shantenRegular] == -1) {
					const TileCode kezi[] = {
						EastWind, SouthWind, WestWind, NorthWind,
						CircleTwo, CircleFour, CircleEight,
						GreenDragon,
					};
					return (yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, kezi, 8, nullptr, 0, false) == SizeOfMeldBuffer) &&
						(yaku::countingFacility::countSpecMentz
						(analysis->MianziDat, kezi, 4, nullptr, 0, false) == 1);
				}
				else return false;
			}
		));
	/* �f�g�a */
	if (RuleData::chkRuleApplied("no_red")) {
		auto hontanhon =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
					CircleTwo, CircleFour, CircleEight,
					EastWind, SouthWind, WestWind, NorthWind,
					WhiteDragon, GreenDragon,
				};
				return chktiles(analysis, kezi, 14, kezi, 1, false);
			};
		auto chintanhon =
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					BambooTwo, BambooThree, BambooFour, BambooSix, BambooEight,
					CircleTwo, CircleFour, CircleEight,
				};
				return chktiles(analysis, kezi, 8, kezi, 1, false);
			};
		if (RuleData::chkRule("no_red", "hon3han_chin6han") || RuleData::chkRule("no_red", "6han")) {
			if (!GameStat.chkGameType(SanmaS))
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					_T("���f�g"), yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
					hontanhon
				));
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				GameStat.chkGameType(SanmaS) ? _T("���f�g") : _T("�f�g�a"), yaku::yakuCalculator::Yaku::yval_6han_kuisagari,
				_T("���f�g"),
				chintanhon
			));
		} else {
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�f�g�a"), get_yaku_han("no_red"),
				hontanhon
			));
		}
	}
	/* �g���\�� */
	if (RuleData::chkRuleApplied("kohaku_manju"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�g���\��"), get_yaku_han("kohaku_manju"),
			_T("�g������"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CharacterOne, CharacterTwo, CharacterThree, CharacterFour,
					CharacterFive, CharacterSix, CharacterSeven, CharacterEight,
					CharacterNine, WhiteDragon, RedDragon,
				};
				return chktiles(analysis, kezi, 11, kezi, 7, false) &&
					(analysis->TileCount[WhiteDragon] > 0) &&
					(analysis->TileCount[RedDragon] > 0);
			}
		));
	/* �f���� (triplet2.cpp���ړ�) */
	if (RuleData::chkRuleApplied("prime_number"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�f����"), get_yaku_han("prime_number"),
			_T("�΁X�a"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CharacterTwo, CharacterThree, CharacterFive, CharacterSeven,
					CircleTwo, CircleThree, CircleFive, CircleSeven,
					BambooTwo, BambooThree, BambooFive, BambooSeven,
				};
				return chktiles(analysis, kezi, 12, nullptr, 0, false);
			}
		));
	/* ����F (triplet2.cpp���ړ�) */
	if (RuleData::chkRuleApplied("hakuiisoo"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����F"), get_yaku_han("hakuiisoo"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CharacterEight, CharacterNine, CircleEight, CircleNine,
					BambooEight, BambooNine, WhiteDragon,
				};
				return chktiles(analysis, kezi, 7, nullptr, 0, false);
			}
		));

	// ---------------------------------------------------------------------

	// �l�A�l���݂ōl������p�^�[���Ɣv���\�L
	// 123 123 123 123: �v��44440 ������͈�F�l��
	// 123 123 123 234: �v��34410
	// 123 123 123 345: �v��33411
	// 123 123 234 234: �v��24420
	// 123 123 234 345: �v��23421
	// 123 123 345 345: �v��22422
	// 123 234 234 234: �v��14430
	// 123 234 234 345: �v��13431
	// 123 234 345 345: �v��12432
	// 123 345 345 345: �v��11433

	/* ��l�A�l */
	if (RuleData::chkRuleApplied("great_four_into_four"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��l�A�l"), get_yaku_han("great_four_into_four"),
			_T("�d�l�A�l"), _T("�l�A�l"), _T("�l�A�O��"), _T("�l�A�O"), _T("���l�A��"), _T("�Վl�A��"), _T("���l�A��"), _T("�l�A��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < TileSuitHonors; i++) {
					// 123 123 234 234: �v��24420
					if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+1] >= 2) &&
						((analysis->MianziDat[0].tile == (TileCode)i) ||
						(analysis->MianziDat[0].tile == (TileCode)(i + 3))))
						return true;
					// 123 123 234 345: �v��23421
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+1] >= 1) &&
						(analysis->ShunziCount[i+2] >= 1) &&
						((analysis->MianziDat[0].tile == (TileCode)i) ||
						(analysis->MianziDat[0].tile == (TileCode)(i + 3))))
						return true;
					// 123 123 345 345: �v��22422
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+2] >= 2) &&
						((analysis->MianziDat[0].tile == (TileCode)i) ||
						(analysis->MianziDat[0].tile == (TileCode)(i + 1)) ||
						(analysis->MianziDat[0].tile == (TileCode)(i + 3)) ||
						(analysis->MianziDat[0].tile == (TileCode)(i + 4))))
						return true;
					// 123 234 345 345: �v��12432
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+1] >= 1) &&
						(analysis->ShunziCount[i+2] >= 2) &&
						((analysis->MianziDat[0].tile == (TileCode)i) ||
						(analysis->MianziDat[0].tile == (TileCode)(i + 3))))
						return true;
				}
				return false;
			}
		));
	/* �d�l�A�l */
	if (RuleData::chkRuleApplied("double_four_into_four"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�d�l�A�l"), get_yaku_han("double_four_into_four"),
			_T("�l�A�l"), _T("�l�A�O��"), _T("�l�A�O"), _T("���l�A��"), _T("�Վl�A��"), _T("���l�A��"), _T("�l�A��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < TileSuitHonors; i++) {
					// 123 123 123 234: �v��34410
					if ((analysis->ShunziCount[i] >= 3) && (analysis->ShunziCount[i+1] >= 1))
						return true;
					// 123 123 234 234: �v��24420
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+1] >= 2))
						return true;
					// 123 234 234 234: �v��14430
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+1] >= 3))
						return true;
				}
				return false;
			}
		));
	/* �l�A�l */
	if (RuleData::chkRuleApplied("four_into_four"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�l�A�l"), get_yaku_han("four_into_four"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < TileSuitHonors; i++) {
					// 123 123 123 345: �v��33411
					if ((analysis->ShunziCount[i] >= 3) && (analysis->ShunziCount[i+2] >= 1))
						return true;
					// 123 123 234 345: �v��23421
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+1] >= 1) &&
						(analysis->ShunziCount[i+2] >= 1))
						return true;
					// 123 123 345 345: �v��22422
					else if ((analysis->ShunziCount[i] >= 2) && (analysis->ShunziCount[i+2] >= 2))
						return true;
					// 123 234 234 345: �v��13431
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+1] >= 2) &&
						(analysis->ShunziCount[i+2] >= 1))
						return true;
					// 123 234 345 345: �v��12432
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+1] >= 1) &&
						(analysis->ShunziCount[i+2] >= 2))
						return true;
					// 123 345 345 345: �v��11433
					else if ((analysis->ShunziCount[i] >= 1) && (analysis->ShunziCount[i+2] >= 3))
						return true;
				}
				return false;
			}
		));

	// ---------------------------------------------------------------------

	{
		auto suukuisan =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 1; i < TileSuitHonors; i++) {
					if (analysis->MianziDat[0].tile == (TileCode)i) {
						if (analysis->ShunziCount[i] >= 2)
							return true;
						else if (i < 1) continue;
						else if ((analysis->ShunziCount[i-1] >= 1) && (analysis->ShunziCount[i] >= 1))
							return true;
						else if (analysis->ShunziCount[i-1] >= 2)
							return true;
						else if (i < 2) continue;
						else if ((analysis->ShunziCount[i-2] >= 1) && (analysis->ShunziCount[i] >= 1))
							return true;
						else if ((analysis->ShunziCount[i-2] >= 1) && (analysis->ShunziCount[i-1] >= 1))
							return true;
						else if (analysis->ShunziCount[i-2] >= 2)
							return true;
					}
				}
				return false;
			};
		/* �l�A�O�� */
		if (RuleData::chkRuleApplied("four_into_three_one"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�l�A�O��"), get_yaku_han("four_into_three_one"),
				_T("�l�A�O"), _T("�l�A��"),
				[suukuisan](const MENTSU_ANALYSIS* const analysis) -> bool {
					bool yakuFlag = false;
					for (int i = 1; i < TileSuitHonors; i++)
						if ((analysis->ShunziCount[i] >= 1) &&
							((analysis->KeziCount[i] >= 1) ||
							(analysis->KeziCount[i+1] >= 1) ||
							(analysis->KeziCount[i+2] >= 1))) yakuFlag = true;
					return suukuisan(analysis) && yakuFlag;
				}
			));
		/* �l�A�O */
		if (RuleData::chkRuleApplied("four_into_three"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�l�A�O"), get_yaku_han("four_into_three"),
				_T("�l�A��"),
				suukuisan
			));
	}

	// ---------------------------------------------------------------------

	/* ���l�A�� */
	if (RuleData::chkRuleApplied("dragon_four_into_one"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���l�A��"), get_yaku_han("dragon_four_into_one"),
			_T("�O�A��"), _T("�Վl�A��"), _T("���l�A��"), _T("�l�A��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TileSuitHonors; i++)
					if ((analysis->ShunziCount[i] >= 1) &&
						(analysis->KeziCount[i] >= 1) &&
						(analysis->KeziCount[i+1] >= 1) &&
						(analysis->KeziCount[i+2] >= 1)) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* �Վl�A�� */
	if (RuleData::chkRuleApplied("tiger_four_into_one"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�Վl�A��"), get_yaku_han("tiger_four_into_one"),
			_T("���l�A��"), _T("�l�A��"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i < TileSuitHonors; i++) {
					if (analysis->ShunziCount[i] >= 1) {
						if ((analysis->KeziCount[i] >= 1) && (analysis->KeziCount[i+1] >= 1)) yakuFlag = true;
						else if ((analysis->KeziCount[i] >= 1) && (analysis->KeziCount[i+2] >= 1)) yakuFlag = true;
						else if ((analysis->KeziCount[i+1] >= 1) && (analysis->KeziCount[i+2] >= 1)) yakuFlag = true;
					}
				}
				return yakuFlag;
			}
		));
	{
		auto suukuiyii =
			[](const MENTSU_ANALYSIS* const analysis) -> int {
				int count = 0;
				for (int i = 1; i < TileSuitHonors; i++) {
					if (analysis->ShunziCount[i] >= 1) {
						if (analysis->KeziCount[i] >= 1) ++count;
						else if (analysis->KeziCount[i+1] >= 1) ++count;
						else if (analysis->KeziCount[i+2] >= 1) ++count;
					}
				}
				return count;
			};
		/* ���l�A�� */
		if (RuleData::chkRuleApplied("double_four_into_one"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("���l�A��"), get_yaku_han("double_four_into_one"),
				_T("�l�A��"),
				[suukuiyii](const MENTSU_ANALYSIS* const analysis) -> bool {
					return (suukuiyii(analysis) == 2);
				}
			));
		/* �l�A�� */
		if (RuleData::chkRuleApplied("four_into_one"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("�l�A��"), get_yaku_han("four_into_one"),
				[suukuiyii](const MENTSU_ANALYSIS* const analysis) -> bool {
					return (suukuiyii(analysis) == 1);
				}
			));
	}

	// ---------------------------------------------------------------------

	/* ��@�ԓ� */
	if (RuleData::chkRuleApplied("chuuren_hwaton"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��@�ԓ�"), get_yaku_han("chuuren_hwaton"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int count = 0;
				for (int i = 1; i <= 9; i++)
					if (analysis->TileCount[TileSuitCharacters + i] +
						analysis->TileCount[TileSuitCircles + i] +
						analysis->TileCount[TileSuitBamboos + i] >=
						(((i == 1)||(i == 9)) ? 3 : 1) )
						++count;
				return (count == 9);
			}
		));
	/* ����A�� */
	if (RuleData::chkRuleApplied("exposed_chuuren"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("��@�ɓ�"), get_yaku_han("exposed_chuuren"),
			/* ���`���C�c���������{���ȏオ�m�� */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				Int8ByTile tiles = analysis->TileCount; bool yakuFlag = false;
				// ���ʎq���v�Z �Ȏq�͖���
				for (int i = 1; i < SizeOfMeldBuffer; i++) {
					switch (analysis->MianziDat[i].mstat) {
					case meldSequenceExposedLower: case meldSequenceExposedMiddle:
					case meldSequenceExposedUpper:
						++tiles[analysis->MianziDat[i].tile];
						++tiles[analysis->MianziDat[i].tile + 1];
						++tiles[analysis->MianziDat[i].tile + 2];
						break;
					case meldTripletExposedLeft: case meldTripletExposedCenter:
					case meldTripletExposedRight:
						tiles[analysis->MianziDat[i].tile] += 3;
						break;
					}
				}
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
					if ((tiles[i + 1] >= 3) && (tiles[i + 2] >= 1) && (tiles[i + 3] >= 1) &&
						(tiles[i + 4] >= 1) && (tiles[i + 5] >= 1) && (tiles[i + 6] >= 1) &&
						(tiles[i + 7] >= 1) && (tiles[i + 8] >= 1) && (tiles[i + 9] >= 3))
						yakuFlag = true;
				return yakuFlag && (!(*analysis->MenzenFlag));
			}
		));

	// ---------------------------------------------------------------------

	/* 䠍��|��(��΁X) */
	if (RuleData::chkRuleApplied("uikyou_shuntsu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("䠍��|�� "), // �΁X�`�Ƌ�ʂ��邽�߂Ɍ��ɃX�y�[�X������
			get_yaku_han("uikyou_shuntsu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 1; i <= 9; i++) {
					if ((analysis->KeziCount[TileSuitCharacters + i] >= 1) &&
						(analysis->KeziCount[TileSuitCircles + i] >= 1) &&
						(analysis->KeziCount[TileSuitBamboos + i] >= 1)) {
							if (i <= 5)
								for (int k = 0; k < TileSuitHonors; k += TileSuitStep)
									if ((analysis->ShunziCount[k + i + 1] >= 1) &&
										(analysis->MianziDat[0].tile == k + i + 4))
										yakuFlag = true;
							/* �����ɂ�else�����Ă͂��� */
							if (i >= 5)
								for (int k = 0; k < TileSuitHonors; k += TileSuitStep)
									if ((analysis->ShunziCount[k + i - 3] >= 1) &&
										(analysis->MianziDat[0].tile == k + i - 4))
										yakuFlag = true;
					}
				}
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* �����Y���Ė� */
	if (RuleData::chkRuleApplied("uutaaran"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����Y���Ė�"), get_yaku_han("uutaaran"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CharacterFive, CircleFive, BambooFive,
				};
				const TileCode shunzi[] = {
					CharacterThree, CharacterFour, CharacterFive,
					CircleThree, CircleFour, CircleFive,
					BambooThree, BambooFour, BambooFive,
				};
				return chktiles(analysis, kezi, 3, shunzi, 9, true) &&
					(analysis->MianziDat[0].tile == CircleOne);
			}
		));
	/* �𗔓{��I�| */
	if (RuleData::chkRuleApplied("grand_cross"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�𗔓{��I�|"), yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(analysis->TsumoHai->tile == CircleFive) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);
			}),
			_T("����F"),
			[chktiles_nodup](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CircleFour, CircleSix,
				};
				const TileCode shunzi[] = {
					CircleOne, CircleSeven,
				};
				return chktiles_nodup(analysis, kezi, 2, shunzi, 2, true) &&
					(analysis->MianziDat[0].tile == CircleFive) &&
					(*analysis->MenzenFlag);
			}
		));
	/* ���F�~�~ */
	if (RuleData::chkRuleApplied("xique_naomei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���F�~�~"), get_yaku_han("xique_naomei"),
			[chktiles_nodup](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CircleFive, BambooOne,
				};
				const TileCode shunzi[] = {
					CharacterOne, CircleOne,
				};
				return chktiles_nodup(analysis, kezi, 2, shunzi, 2, true) &&
					(analysis->MianziDat[0].tile == BambooFive);
			}
		));
	/* �Ԉ�F */
	if (RuleData::chkRuleApplied("huaiisoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�Ԉ�F"), get_yaku_han("huaiisoh"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CharacterOne, CharacterTwo, CharacterThree, CharacterFour, CharacterFive,
					CharacterSix, CharacterSeven, CharacterEight, CharacterNine, RedDragon,
					CircleOne, CircleThree, CircleFive, CircleSix, CircleSeven,
					CircleNine, BambooOne, BambooFive, BambooSeven, BambooNine,
				};
				const TileCode shunzi[] = {
					CharacterOne, CharacterTwo, CharacterThree, CharacterFour, CharacterFive,
					CharacterSix, CharacterSeven, CircleFive,
				};
				return (chktiles(analysis, kezi, 20, shunzi, 8, false)) &&
					(!chktiles(analysis, kezi, 10, shunzi, 7, false));
			}
		));
	/* �Ԉ�F */
	if (RuleData::chkRuleApplied("akaiisoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�Ԉ�F"), get_yaku_han("akaiisoh"),
			_T("�Ԉ�F"), _T("����"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CircleFive, CircleOne, CircleThree, CircleSix, CircleSeven,
					CircleNine, BambooOne, BambooFive, BambooSeven, BambooNine,
				};
				return chktiles(analysis, kezi, 10, kezi, 1, false);
			}
		));
	/* �Ԋ�� */
	if (RuleData::chkRuleApplied("sekigan_seiryuu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�Ԋ��"), get_yaku_han("sekigan_seiryuu"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->ShunziCount[CircleTwo] == 1) &&
					(analysis->KeziCount[WhiteDragon] + analysis->KeziCount[EastWind] == 1) &&
					(analysis->DuiziCount[CircleTwo] + analysis->DuiziCount[CircleFour] +
					analysis->DuiziCount[CircleEight] >= 3));
			}
		));

	// ---------------------------------------------------------------------

	/* ������ */
	{
		const char ruleCodeList[10][16] = { "",
			"all_one", "all_two", "all_three", "all_four", "all_five",
			"all_six", "all_seven", "all_eight", "all_nine"};
		const TCHAR nameList[10][16] = { _T(""),
			_T("�S�ш�"), _T("�S�ѓ�"), _T("�S�юO"), _T("�S�юl"), _T("�S�ь�"), _T("�S�јZ"), _T("�S�ю�"), _T("�S�є�"), _T("�S�ы�")};
		for (int i = 1; i <= 9; i++) {
			if (RuleData::chkRuleApplied(ruleCodeList[i])) {
				auto f =
					[chktiles, i](const MENTSU_ANALYSIS* const analysis) -> bool {
						const TileCode kezi[] = {
							(TileCode)(i+TileSuitCharacters),
							(TileCode)(i+TileSuitCircles),
							(TileCode)(i+TileSuitBamboos),
						};
						const TileCode shunzi[] = {
							(TileCode)(i+TileSuitCharacters), (TileCode)(i+TileSuitCircles),
							(TileCode)(i+TileSuitBamboos),
							(TileCode)(i+TileSuitCharacters-1), (TileCode)(i+TileSuitCircles-1),
							(TileCode)(i+TileSuitBamboos-1),
							(TileCode)(i+TileSuitCharacters-2), (TileCode)(i+TileSuitCircles-2),
							(TileCode)(i+TileSuitBamboos-2),
						};
						return chktiles(analysis, kezi, 3,
							&shunzi[i <= 7 ? 0 : 3 * (i - 7)],
							i > 7 ? 3 * (10 - i) : (i < 3 ? i * 3 : 9),
							false);
					};
				switch (i) {
				case 4: case 5: case 6:
					yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
						nameList[i], get_yaku_han(ruleCodeList[i]), _T("�f���"), f));
					break;
				case 1: case 9:
					yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
						nameList[i], get_yaku_han(ruleCodeList[i]), _T("���S�ћ��"), f));
					break;
				default:
					yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
						nameList[i], get_yaku_han(ruleCodeList[i]), f));
					break;
				}
			}
		}
	}

	// ---------------------------------------------------------------------

	/* ������ */
	if (RuleData::chkRuleApplied("xiaoyuwu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("������"), get_yaku_han("xiaoyuwu"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CharacterOne, CircleOne, BambooOne,
					CharacterTwo, CircleTwo, BambooTwo,
					CharacterThree, CircleThree, BambooThree,
					CharacterFour, CircleFour, BambooFour,
				};
				return chktiles(analysis, kezi, 12, kezi, 6, false);
			}
		));
	/* �剗�� */
	if (RuleData::chkRuleApplied("dayuwu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�剗��"), get_yaku_han("dayuwu"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CharacterSix, CircleSix, BambooSix,
					CharacterSeven, CircleSeven, BambooSeven,
					CharacterEight, CircleEight, BambooEight,
					CharacterNine, CircleNine, BambooNine,
				};
				return chktiles(analysis, kezi, 12, kezi, 6, false);
			}
		));
	/* �S�� */
	if (RuleData::chkRuleApplied("all_small"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�S��"), get_yaku_han("all_small"),
			_T("������"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CharacterOne, CircleOne, BambooOne,
					CharacterTwo, CircleTwo, BambooTwo,
					CharacterThree, CircleThree, BambooThree,
				};
				return chktiles(analysis, kezi, 9, kezi, 3, false);
			}
		));
	/* �S�� */
	if (RuleData::chkRuleApplied("all_middle"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�S��"), get_yaku_han("all_middle"),
			_T("�f���"), _T("�㋉�f���"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CharacterFour, CircleFour, BambooFour,
					CharacterFive, CircleFive, BambooFive,
					CharacterSix, CircleSix, BambooSix,
				};
				return chktiles(analysis, kezi, 9, kezi, 3, false);
			}
		));
	/* �S�� */
	if (RuleData::chkRuleApplied("all_large"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�S��"), get_yaku_han("all_large"),
			_T("�剗��"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CharacterSeven, CircleSeven, BambooSeven,
					CharacterEight, CircleEight, BambooEight,
					CharacterNine, CircleNine, BambooNine,
				};
				return chktiles(analysis, kezi, 9, kezi, 3, false);
			}
		));

	// ---------------------------------------------------------------------

	/* ���ߋR�m */
	if (RuleData::chkRuleApplied("blackjack"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���ߋR�m"), get_yaku_han("blackjack"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[shantenRegular] == -1)
					return yaku::countingFacility::countMentzNumerals(analysis->MianziDat) == 21;
				else if (analysis->shanten[shantenPairs] == -1)
					return yaku::countingFacility::countTileNumerals(analysis->TileCount) == 21;
				else return false;
			}
		));
	/* ���ߋR�m */
	if (RuleData::chkRuleApplied("white_knight"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���ߋR�m"), get_yaku_han("white_knight"),
			_T("���ߋR�m"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->KeziCount[WhiteDragon] >= 1)
					return yaku::countingFacility::countMentzNumerals(analysis->MianziDat) == 21;
				else return false;
			}
		));

	// ---------------------------------------------------------------------

	/* ����n */
	{
		const int rules = 4;
		const char ruleCodeList[rules][16] = {"univ_tokyo", "univ_hokkaido", "univ_chuo", "univ_hakuoh"};
		const TileCode tileCodeList[rules] = {EastWind, NorthWind, RedDragon, WhiteDragon};
		const TCHAR yakuNameList[rules][16] = {_T("�����"), _T("�k���"), _T("�����"), _T("���c�w�@")};
		for (int i = 0; i < rules; i++) {
			const TileCode targetTile = tileCodeList[i];
			if (RuleData::chkRuleApplied(ruleCodeList[i]))
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					yakuNameList[i], get_yaku_han(ruleCodeList[i]),
					[targetTile](const MENTSU_ANALYSIS* const analysis) -> bool {
						return ((analysis->KangziCount[targetTile] >= 1) &&
							(analysis->Machi == yaku::yakuCalculator::machiRyanmen) &&
							(analysis->TotalKezi == 1) && (*analysis->MenzenFlag));
					}
				));
		}
	}
	/* ���k�� */
	if (RuleData::chkRuleApplied("univ_tohoku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���k���"), get_yaku_han("univ_tohoku"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[EastWind] >= 1) &&
					(analysis->KangziCount[NorthWind] >= 1) &&
					(analysis->Machi == yaku::yakuCalculator::machiRyanmen) &&
					(analysis->TotalKezi == 2) && (*analysis->MenzenFlag));
			}
		));
	/* ����� */
	if (RuleData::chkRuleApplied("univ_nagoya"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�����"), get_yaku_han("univ_nagoya"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
					for (int j = 0; j < TileSuitHonors; j += TileSuitStep)
						if ((analysis->KangziCount[i + 5] >= 1) &&
							(analysis->KangziCount[j + 8] >= 1))
							yakuFlag = true;
				return ((yakuFlag) &&
					(analysis->Machi == yaku::yakuCalculator::machiRyanmen) &&
					(analysis->TotalKezi == 2) && (*analysis->MenzenFlag));
			}
		));

	// ---------------------------------------------------------------------

	/* �~�̃\�i�^ */
	if (RuleData::chkRuleApplied("fuyusona"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�~�̃\�i�^"), get_yaku_han("fuyusona"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->ShunziCount[CharacterOne] >= 1) &&
					(analysis->ShunziCount[CircleOne] >= 1) && (analysis->ShunziCount[BambooOne] >= 1))
					yakuFlag = true;
				else if ((analysis->ShunziCount[CharacterSeven] >= 1) &&
					(analysis->ShunziCount[CircleSeven] >= 1) && (analysis->ShunziCount[BambooSeven] >= 1))
					yakuFlag = true;
				return (yakuFlag && (analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->MianziDat[0].tile == NorthWind));
			}
		));
	/* �~�̃\�i�^�Q */
	if (RuleData::chkRuleApplied("fuyusona2"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�~�̃\�i�^�Q"), get_yaku_han("fuyusona2"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->ShunziCount[CharacterOne] >= 1) && (analysis->ShunziCount[CircleOne] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[CharacterOne] >= 1) && (analysis->ShunziCount[BambooOne] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[CircleOne] >= 1) && (analysis->ShunziCount[BambooOne] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[CharacterSeven] >= 1) && (analysis->ShunziCount[CircleSeven] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[CharacterSeven] >= 1) && (analysis->ShunziCount[BambooSeven] >= 1)) yakuFlag = true;
				else if ((analysis->ShunziCount[CircleSeven] >= 1) && (analysis->ShunziCount[BambooSeven] >= 1)) yakuFlag = true;
				return (yakuFlag &&
					(analysis->KeziCount[CharacterTwo] + analysis->KeziCount[CircleTwo] + analysis->KeziCount[BambooTwo] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->MianziDat[0].tile == NorthWind));
			}
		));

	// ---------------------------------------------------------------------

	/* �㋉�f��� */
	if (RuleData::chkRuleApplied("high_tanyao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�㋉�f���"), get_yaku_han("high_tanyao"),
			_T("�f���"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CharacterThree, CircleThree, BambooThree,
					CharacterFour, CircleFour, BambooFour,
					CharacterFive, CircleFive, BambooFive,
					CharacterSix, CircleSix, BambooSix,
					CharacterSeven, CircleSeven, BambooSeven,
				};
				return chktiles(analysis, kezi, 15, kezi, 9, false);
			}
		));
	/* ����F */
	if (RuleData::chkRuleApplied("allmoney"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("����F"), get_yaku_han("allmoney"),
			_T("����"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				const TileCode kezi[] = {
					CircleOne, CircleTwo, CircleThree, CircleFour, CircleFive,
					CircleSix, CircleSeven, CircleEight, CircleNine, CharacterOne,
				};
				return chktiles(analysis, kezi, 10, kezi, 7, false) &&
					(analysis->TileCount[CharacterOne] > 0);
			}
		));
	/* �P�q�� */
	if (RuleData::chkRuleApplied("fengzike"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("�P�q��"), get_yaku_han("fengzike"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
					if ((analysis->KeziCount[i + 1] >= 1) &&
						(analysis->ShunziCount[i + 4] >= 1) &&
						(analysis->KeziCount[i + 9] >= 1)) return true;
				return false;
			}
		));
	/* ���q�� */
	if (RuleData::chkRuleApplied("longzike"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("���q��"), get_yaku_han("longzike"),
			[chktiles](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < TileSuitHonors; i += TileSuitStep)
					if ((analysis->ShunziCount[i + 1] >= 1) &&
						(analysis->KeziCount[i + 5] >= 1) &&
						(analysis->ShunziCount[i + 7] >= 1)) return true;
				return false;
			}
		));
}
