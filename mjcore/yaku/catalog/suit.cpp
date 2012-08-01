#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_suit() {
	/* �`���C�c */
	auto isshoku =
		[](const MENTSU_ANALYSIS* const analysis, bool chin_itsu) -> bool {
			bool flag[TILE_SUIT_HONORS / TILE_SUIT_STEP]; bool yakuFlag = true;
			for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP); i++) flag[i] = true;
			for (int k = 1; k < (chin_itsu ? TILE_NONFLOWER_MAX : TILE_SUIT_HONORS); k++)
				if (analysis->TileCount[k] > 0)
					for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP); i++)
						if ((k / TILE_SUIT_STEP) != i) flag[i] = false;
			for (int i = 0; i < (TILE_SUIT_HONORS / TILE_SUIT_STEP); i++)
				yakuFlag &= flag[i];
			return yakuFlag;
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"����F", (getRule(RULE_CHINIISOH) != 0) ?
		yaku::yakuCalculator::Yaku::yval_4han_kuisagari : yaku::yakuCalculator::Yaku::yval_6han_kuisagari,
		"����F", "����C�ʊёS�ћ��",
		[isshoku](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return isshoku(analysis, true);
		}
	));
	/* �a�������玀�� */
	auto chkHaishiki =
		[](const MENTSU_ANALYSIS* const analysis, const char* const haishiki) -> bool {
			bool yakuFlag = false;
			for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
				if ((analysis->TileCount[i + 1] >= (int)(haishiki[0] - '0')) &&
					(analysis->TileCount[i + 2] >= (int)(haishiki[1] - '0')) &&
					(analysis->TileCount[i + 3] >= (int)(haishiki[2] - '0')) &&
					(analysis->TileCount[i + 4] >= (int)(haishiki[3] - '0')) &&
					(analysis->TileCount[i + 5] >= (int)(haishiki[4] - '0')) &&
					(analysis->TileCount[i + 6] >= (int)(haishiki[5] - '0')) &&
					(analysis->TileCount[i + 7] >= (int)(haishiki[6] - '0')) &&
					(analysis->TileCount[i + 8] >= (int)(haishiki[7] - '0')) &&
					(analysis->TileCount[i + 9] >= (int)(haishiki[8] - '0')) ) yakuFlag = true;
			return yakuFlag;
		};
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"��@��", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"����F",
		[chkHaishiki](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return chkHaishiki(analysis, "311111113");
		}
	));
	if (getRule(RULE_DOUBLE_YAKUMAN) == 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"������@��", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"��@��", "����F",
			[chkHaishiki](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				const tileCode* tsumoTile = &(gameStat->Player[analysis->player].Hand[NUM_OF_TILES_IN_HAND - 1].tile);
				if ((*tsumoTile % 10 == 1) || (*tsumoTile % 10 == 9)) {
					if (analysis->TileCount[*tsumoTile] >= 4) yakuFlag = true;
				} else {
					if (analysis->TileCount[*tsumoTile] >= 2) yakuFlag = true;
				}
				return (chkHaishiki(analysis, "311111113")) && (!analysis->MachiInfo.FuritenFlag);
			}
		));
	/* �^���q */
	if (getRule(RULE_DAISHARIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��ԗ�", (getRule(RULE_DAISHARIN) == 2) ?
			yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen : yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��", "���Ύq", "���a", "�f���",
			[chkHaishiki](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkHaishiki(analysis, "022222220") && (analysis->TileCount[CircleFive] >= 2));
			}
		));
	/* ��|�� */
	if (getRule(RULE_DAICHIKURIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��|��", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��", "���Ύq", "���a", "�f���",
			[chkHaishiki](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkHaishiki(analysis, "022222220") && (analysis->TileCount[BambooFive] >= 2));
			}
		));
	/* �吔�� */
	if (getRule(RULE_DAISUURIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�吔��", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��", "���Ύq", "���a", "�f���",
			[chkHaishiki](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return (chkHaishiki(analysis, "022222220") && (analysis->TileCount[CharacterFive] >= 2));
			}
		));
	/* ���ԗ� */
	if (getRule(RULE_SHOSHARIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ԗ�", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��", "���Ύq", "���a",
			[chkHaishiki](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->TileCount[CircleFive] >= 2) &&
					(chkHaishiki(analysis, "222222200") || chkHaishiki(analysis, "002222222")));
			}
		));
	/* ���|�� */
	if (getRule(RULE_SHOCHIKURIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���|��", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��", "���Ύq", "���a",
			[chkHaishiki](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->TileCount[BambooFive] >= 2) &&
					(chkHaishiki(analysis, "222222200") || chkHaishiki(analysis, "002222222")));
			}
		));
	/* ������ */
	if (getRule(RULE_SHOSUURIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"������", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��", "���Ύq", "���a",
			[chkHaishiki](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->TileCount[CharacterFive] >= 2) &&
					(chkHaishiki(analysis, "222222200") || chkHaishiki(analysis, "002222222")));
			}
		));
	/* �~�����a (3.)14159265358979 */
	if (getRule(RULE_PI) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�~�����a", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F",
			[chkHaishiki](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "211131113");
			}
		));
	/* ��Q�a 1.4142135623730 */
	if (getRule(RULE_SQUARE_ROOT_TWO) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��Q�a", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F",
			[chkHaishiki](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "323211100");
			}
		));
	/* ���R�ΐ��̒� 2.7182818284590 */
	if (getRule(RULE_NATURAL_LOGARITHM) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���R�ΐ��̒�", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F",
			[chkHaishiki](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "230110141");
			}
		));
	/* �����a (0.)207879576350761 */
	if (getRule(RULE_IMAGINARY_NUMBER) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�����a", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F",
			[chkHaishiki](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "111022411");
			}
		));
	/* ��P�O�a 3.16227766016837 */
	if (getRule(RULE_SQUARE_ROOT_TEN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"��P�O�a", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F",
			[chkHaishiki](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return chkHaishiki(analysis, "222004310");
			}
		));
	/* ���C�� */
	if (getRule(RULE_QINGHAIHU) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���C��", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "���S�ћ��", "��u��", "���a",
			[chkHaishiki](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((chkHaishiki(analysis, "422000222") || chkHaishiki(analysis, "222000224")));
			}
		));
	/* ���ԗ� */
	auto chuusharin =
		[chkHaishiki](const MENTSU_ANALYSIS* const analysis, int suit) -> bool {
			return ((chkHaishiki(analysis, "202222220") || chkHaishiki(analysis, "222022220") ||
				chkHaishiki(analysis, "222202220") || chkHaishiki(analysis, "222222020") ||
				chkHaishiki(analysis, "022222202") || chkHaishiki(analysis, "022220222") ||
				chkHaishiki(analysis, "022202222") || chkHaishiki(analysis, "020222222")) &&
				((analysis->TileCount[suit + 4] >= 2) || (analysis->TileCount[suit + 5] >= 2))
				);
		};
	if (getRule(RULE_CHUUSHARIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ԗ�", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��",
			[chuusharin](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return chuusharin(analysis, TILE_SUIT_CIRCLES);
			}
		));
	/* ���|�� */
	if (getRule(RULE_CHUUCHIKURIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���|��", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��",
			[chuusharin](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return chuusharin(analysis, TILE_SUIT_BAMBOOS);
			}
		));
	/* ������ */
	if (getRule(RULE_CHUUSUURIN) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"������", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"����F", "��u��",
			[chuusharin](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return chuusharin(analysis, TILE_SUIT_CHARACTERS);
			}
		));
	/* TODO: ���̑��̃`���C�c�̏�ʖ� */
	/* �z���C�c */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"����F", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
		"��C�ʊёS�ћ��",
		[isshoku](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return isshoku(analysis, false);
		}
	));
}
