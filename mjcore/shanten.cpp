#include "shanten.h"

using std::min;

/* �ʎq�f�[�^������ */

uint8_t* ShantenAnalyzer::mentsuAnalysisDat = nullptr;

__declspec(dllexport) void ShantenAnalyzer::initMentsuAnalysisDat() { // �ʎq�f�[�^������
	Compressed::file_mentz_dat* mnzdat = new Compressed::file_mentz_dat();
	mentsuAnalysisDat = new uint8_t[mnzdat->getDataSize()+4];
	memcpy_s(mentsuAnalysisDat, mnzdat->getDataSize()+4, mnzdat->getData(), mnzdat->getDataSize());
	delete mnzdat;
}

/* ���������v�Z���� */

MJCORE SHANTEN ShantenAnalyzer::calcShanten(const GameTable* const gameStat, PLAYER_ID playerID, shantenType mode)
{ // ���������v�Z����
	/* ���v���ꂼ��̖ʎq�̐��𐔂��� */
	Int8ByTile tileCount = countTilesInHand(gameStat, playerID);

	/* mode�ʕ��� */
	switch (mode) {
	case shantenRegular:
		return calcShantenRegular(gameStat, playerID, tileCount);
	case shantenPairs:
		return calcShantenChiitoi(gameStat, playerID, tileCount);
	case shantenOrphans:
		return calcShantenKokushi(gameStat, playerID, tileCount);
	case shantenStellar:
		return calcShantenStellar(gameStat, playerID, tileCount, true);
	case shantenCivilWar:
		return calcShantenCivilWar(gameStat, playerID, tileCount);
	case shantenSyzygy:
		return calcShantenSyzygy(gameStat, playerID, tileCount);
	case shantenQuanbukao:
		return calcShantenStellar(gameStat, playerID, tileCount, false);
	case shantenSevenup:
		return calcShantenSevenup(gameStat, playerID, tileCount);
	default:
		/* �S�����߂Ĉ�Ԙa���ɋ߂����Ԃ� */
		SHANTEN shanten, tmpShanten;
		shanten = calcShantenRegular(gameStat, playerID, tileCount);
		tmpShanten = calcShantenChiitoi(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenKokushi(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenStellar(gameStat, playerID, tileCount, true); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenCivilWar(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenSyzygy(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenStellar(gameStat, playerID, tileCount, false); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenSevenup(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		return shanten;
	}
}
__declspec(dllexport) int ShantenAnalyzer::calcShanten(const GameTable* const gameStat, int playerID, int mode)
{
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	return (int)calcShanten(gameStat, (PLAYER_ID)playerID, (shantenType)mode);
}

SHANTEN ShantenAnalyzer::calcShantenRegular(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount)
{ // �ʎq��̌����������߂�
	SHANTEN shanten = 8; // �S�������ĂȂ��Ă�Ńo���o����������ʎq��ɑ΂���8�����i���Ύq�ɑ΂��ĂȂ�6�����ɂȂ�j

	// ���v
	int mianzi = 0; int tarzi = 0; bool atama = false; // �ʎq���q�����̐�
	for (int suit = 0; suit < TILE_NUMERAL_COLORS; suit++) {
		unsigned int statcode = 0; unsigned int qDigit = 1;
		for (int i = 1; i <= 9; i++) {
			statcode += min(tileCount[suit * TILE_SUIT_STEP + i], 4) * qDigit;
			qDigit *= 5;
		}
		uint8_t tmpdat = mentsuAnalysisDat[statcode];
		mianzi += (tmpdat & 0x70) >> 4;
		tarzi += tmpdat & 0x0f;
		if (tmpdat & 0x80) atama = true;
	}

	// ���v
	for (int i = 1; i <= 7; i++) {
		if (tileCount[TILE_SUIT_HONORS + i] == 2) {tarzi++; atama = 1;}
		if (tileCount[TILE_SUIT_HONORS + i] >= 3) {mianzi++;}
	}

	// ���ʎq��ÞȂ�����ꍇ
	mianzi += gameStat->Player[playerID].MeldPointer;
	
	int mianziCount = 0;
	if (mianzi + tarzi > 4) {
		// �ʎq�����̂Ƃ�
		shanten = 8 - (mianzi * 2) - (4 - mianzi);
		// �ʎq�����ł��A�������鎞�͓���������
		if (atama) shanten -= 1;
	} else {
		// �����łȂ��Ƃ�
		shanten = 8 - (mianzi * 2) - tarzi;
	}

	return shanten;
}

SHANTEN ShantenAnalyzer::calcShantenChiitoi(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount)
{ // ���Ύq�ɑ΂�������������߂�B
	SHANTEN shanten = 6;
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++)
		// �P���ɑΎq�̐��𒲂ׂ�΂悢
		// �������A�����v�S����Ύq�Q�Ƃ��Ďg���Ă͂Ȃ�Ȃ�
		if (tileCount[i] >= 2) shanten--;
	// �Í�������ꍇ�ɒ��v�Ƃ݂Ȃ��Ȃ��悤�ɂ���
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++)
		if ((tileCount[i] >= 3)&&(shanten < 1)) shanten++;
	// ���ʎq��ÞȂ�����ꍇ�͎��Ύq�͕s�\
	if (gameStat->Player[playerID].MeldPointer > 0) shanten = SHANTEN_IMPOSSIBLE;

	return shanten;
}

SHANTEN ShantenAnalyzer::calcShantenKokushi(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount)
{ // ���m���o�ɑ΂�������������߂�B
	if (chkGameType(gameStat, SanmaS)) return SHANTEN_IMPOSSIBLE; // ���v�O���ł͕s�\

	tileCode YaojiuPai[13] = {
		CharacterOne, CharacterNine, CircleOne, CircleNine, BambooOne, BambooNine,
		EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
	};
	SHANTEN shanten = 13; bool atama = false;
	for (int i = 0; i < 13; i++) {
		// ���I��v�P��ނɂ��A�P���J�E���g����B
		if (tileCount[YaojiuPai[i]] >= 2) atama = true; // �A�^�}���
		if (tileCount[YaojiuPai[i]] >= 1) shanten--;
	}
	/* ����������ꍇ */
	if (atama) shanten--;
	// ���ʎq��ÞȂ�����ꍇ�͍��m���o���s�\
	if (gameStat->Player[playerID].MeldPointer > 0) shanten = SHANTEN_IMPOSSIBLE;

	return shanten;
}

SHANTEN ShantenAnalyzer::calcShantenStellar(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount, bool qixing)
{ // ����F�����s��/�S�s�ς̌����������߂�
	if ((!RuleData::chkRuleApplied("stellar_uushii"))&&(qixing)) return SHANTEN_IMPOSSIBLE;
	else if ((!RuleData::chkRuleApplied("quanbukao"))&&(!qixing)) return SHANTEN_IMPOSSIBLE;

	SHANTEN shanten = SHANTEN_IMPOSSIBLE;
	tileCode QixingZiPai[7] = {
		EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
	};
	int qixingZiPaiCount = 0;
	for (int i = 0; i < 7; i++)
		if (tileCount[QixingZiPai[i]] >= 1)
			qixingZiPaiCount++;
	for (int i = 0; i < 6; i++) {
		tileCode QixingPai[9];
		switch (i) {
		case 0: case 1:
			QixingPai[0] = CharacterOne; QixingPai[1] = CharacterFour;
			QixingPai[2] = CharacterSeven; break;
		case 2: case 3:
			QixingPai[0] = CharacterTwo; QixingPai[1] = CharacterFive;
			QixingPai[2] = CharacterEight; break;
		case 4: case 5:
			QixingPai[0] = CharacterThree; QixingPai[1] = CharacterSix;
			QixingPai[2] = CharacterNine; break;
		}
		switch (i) {
		case 2: case 4:
			QixingPai[3] = CircleOne; QixingPai[4] = CircleFour;
			QixingPai[5] = CircleSeven; break;
		case 0: case 5:
			QixingPai[3] = CircleTwo; QixingPai[4] = CircleFive;
			QixingPai[5] = CircleEight; break;
		case 1: case 3:
			QixingPai[3] = CircleThree; QixingPai[4] = CircleSix;
			QixingPai[5] = CircleNine; break;
		}
		switch (i) {
		case 3: case 5:
			QixingPai[6] = BambooOne; QixingPai[7] = BambooFour;
			QixingPai[8] = BambooSeven; break;
		case 1: case 4:
			QixingPai[6] = BambooTwo; QixingPai[7] = BambooFive;
			QixingPai[8] = BambooEight; break;
		case 0: case 2:
			QixingPai[6] = BambooThree; QixingPai[7] = BambooSix;
			QixingPai[8] = BambooNine; break;
		}
		int qixingShuPaiCount = 0;
		for (int i = 0; i < 9; i++)
			if ((tileCount[QixingPai[i]] >= 1) &&
				((qixingShuPaiCount < 7)||(!qixing))) qixingShuPaiCount++;
		SHANTEN tmpShanten = 13 - qixingZiPaiCount - qixingShuPaiCount;
		// ���ʎq��ÞȂ�����ꍇ�͕s�\
		if (gameStat->Player[playerID].MeldPointer > 0) shanten = SHANTEN_IMPOSSIBLE;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
}

SHANTEN ShantenAnalyzer::calcShantenCivilWar(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount)
{ // ����F��k�푈�̌����������߂�
	if (!RuleData::chkRuleApplied("civil_war")) return SHANTEN_IMPOSSIBLE;

	SHANTEN shanten = 13;
	// �ȉ��A�ꖇ�����ׂ�
	for (int i = 0; i < 6; i++) {
		Int8ByTile tileCountTmp;
		for (int j = 0; j < TILE_NONFLOWER_MAX; j++) tileCountTmp[j] = tileCount[j];
		tileCode CivilWarPai[NUM_OF_TILES_IN_HAND] = {
			NoTile, NoTile, NoTile, NoTile,
			NoTile, NoTile, NoTile, NoTile,
			SouthWind, SouthWind, SouthWind,
			NorthWind, NorthWind, NorthWind
		};
		switch (i) {
		case 0: case 1:
			CivilWarPai[0] = CharacterOne; CivilWarPai[1] = CivilWarPai[2] = CharacterEight;
			CivilWarPai[3] = CharacterOne; break;
		case 2: case 3:
			CivilWarPai[0] = CircleOne; CivilWarPai[1] = CivilWarPai[2] = CircleEight;
			CivilWarPai[3] = CircleOne; break;
		case 4: case 5:
			CivilWarPai[0] = BambooOne; CivilWarPai[1] = CivilWarPai[2] = BambooEight;
			CivilWarPai[3] = BambooOne; break;
		}
		switch (i) {
		case 2: case 4:
			CivilWarPai[4] = CharacterOne; CivilWarPai[5] = CivilWarPai[6] = CharacterEight;
			CivilWarPai[7] = CharacterFive; break;
		case 0: case 5:
			CivilWarPai[4] = CircleOne; CivilWarPai[5] = CivilWarPai[6] = CircleEight;
			CivilWarPai[7] = CircleFive; break;
		case 1: case 3:
			CivilWarPai[4] = BambooOne; CivilWarPai[5] = CivilWarPai[6] = BambooEight;
			CivilWarPai[7] = BambooFive; break;
		}
		int civilWarPaiCount = 0;
		for (int j = 0; j < NUM_OF_TILES_IN_HAND; j++) {
			if (tileCountTmp[CivilWarPai[j]] >= 1) {
				civilWarPaiCount++;
				tileCountTmp[CivilWarPai[j]]--;
			}
		}
		SHANTEN tmpShanten = 13 - civilWarPaiCount;
		// ���ʎq��ÞȂ�����ꍇ�͍l���Ȃ�
		if (gameStat->Player[playerID].MeldPointer > 0) shanten = SHANTEN_IMPOSSIBLE;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
}

SHANTEN ShantenAnalyzer::calcShantenSyzygy(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount)
{ // ����F�f������̌����������߂�
	if (!RuleData::chkRuleApplied("syzygy")) return SHANTEN_IMPOSSIBLE;

	// �ȉ��A�ꖇ�����ׂ�
	Int8ByTile tileCountTmp;
	for (int i = 0; i < TILE_NONFLOWER_MAX; i++) tileCountTmp[i] = tileCount[i];
	tileCode syzygyPai[NUM_OF_TILES_IN_HAND] = {
		CircleOne, CircleOne, CircleTwo,
		CircleThree, CircleThree, CircleFour,
		CircleFive, CircleFive, CircleSix,
		CircleSeven, CircleSeven, CircleEight,
		CircleNine, CircleNine
	};
	int syzygyPaiCount = 0;
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (tileCountTmp[syzygyPai[i]] >= 1) {
			syzygyPaiCount++;
			tileCountTmp[syzygyPai[i]]--;
		}
	}
	// ���ʎq��ÞȂ�����ꍇ�͍l���Ȃ�

	return (gameStat->Player[playerID].MeldPointer > 0) ? SHANTEN_IMPOSSIBLE : (13 - syzygyPaiCount);
}

SHANTEN ShantenAnalyzer::calcShantenSevenup(const GameTable* const gameStat, PLAYER_ID playerID, Int8ByTile& tileCount)
{ // ����F�Z�u���A�b�v�̌����������߂�
	if (!RuleData::chkRuleApplied("sevenup")) return SHANTEN_IMPOSSIBLE;

	SHANTEN shanten = 13;
	// �ȉ��A�ꖇ�����ׂ�
	for (int i = 0; i < 3; i++) {
		Int8ByTile tileCountTmp;
		for (int j = 0; j < TILE_NONFLOWER_MAX; j++) tileCountTmp[j] = tileCount[j];
		tileCode tileArrange[NUM_OF_TILES_IN_HAND] = {
			NoTile, NoTile, NoTile, NoTile,
			NoTile, NoTile, NoTile,
			EastWind, SouthWind, WestWind, NorthWind,
			WhiteDragon, GreenDragon, RedDragon,
		};
		for (int j = 0; j < 7; j++)
			tileArrange[j] = (tileCode)(i * TILE_SUIT_STEP + j + 1);

		int yakuTileCount = 0;
		for (int j = 0; j < NUM_OF_TILES_IN_HAND; j++) {
			if (tileCountTmp[tileArrange[j]] >= 1) {
				yakuTileCount++;
				tileCountTmp[tileArrange[j]]--;
			}
		}
		SHANTEN tmpShanten = 13 - yakuTileCount;
		// ���ʎq��ÞȂ�����ꍇ�͍l���Ȃ�
		if (gameStat->Player[playerID].MeldPointer > 0) shanten = SHANTEN_IMPOSSIBLE;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
}
