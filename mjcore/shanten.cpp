#include "shanten.h"

#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <algorithm>
#include <cassert>
#include "except.h"
#include "TileCode.h"
#include "func.h"
#include "logging.h"
#include "reader/readrsrc.h"
#include "../common/strcode.h"
#include "decomp.h"
#include "ruletbl.h"

using std::min;

/* �ʎq�f�[�^������ */

uint8_t* ShantenAnalyzer::mentsuAnalysisDat = nullptr;

__declspec(dllexport) void ShantenAnalyzer::initMentsuAnalysisDat() { // �ʎq�f�[�^������
	Compressed::file_mentz_dat* mnzdat = new Compressed::file_mentz_dat();
	mentsuAnalysisDat = new uint8_t[mnzdat->getDataSize()+4];
#ifdef _MSC_VER
	memcpy_s(mentsuAnalysisDat, mnzdat->getDataSize()+4, mnzdat->getData(), mnzdat->getDataSize());
#else
	memcpy(mentsuAnalysisDat, mnzdat->getData(), mnzdat->getDataSize());
#endif
	delete mnzdat;
}

/* ���������v�Z���� */

MJCORE Shanten ShantenAnalyzer::calcShanten(const GameTable* const gameStat, PlayerID playerID, ShantenType mode)
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
	case shantenZuhelong:
		return calcShantenZuhelong(gameStat, playerID, tileCount);
	default:
		/* �S�����߂Ĉ�Ԙa���ɋ߂����Ԃ� */
		Shanten shanten, tmpShanten;
		shanten = calcShantenRegular(gameStat, playerID, tileCount);
		tmpShanten = calcShantenChiitoi(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenKokushi(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenStellar(gameStat, playerID, tileCount, true); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenCivilWar(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenSyzygy(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenStellar(gameStat, playerID, tileCount, false); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenSevenup(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenZuhelong(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		return shanten;
	}
}
__declspec(dllexport) int ShantenAnalyzer::calcShanten(const GameTable* const gameStat, int playerID, int mode)
{
	assert((gameStat == &GameStat)||(gameStat == &StatSandBox));
	return (int)calcShanten(gameStat, (PlayerID)playerID, (ShantenType)mode);
}

unsigned int ShantenAnalyzer::chkMianzi(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount, unsigned limit) {
	// �ʎq��2�A�Ύq�E���q��1�Ƃ������l
	unsigned int ans = 0;
	// ���v
	int mianzi = 0; int tarzi = 0; bool atama = false; // �ʎq���q�����̐�
	for (int suit = 0; suit < TILE_NUMERAL_COLORS; suit++) {
		unsigned int statcode = 0; unsigned int qDigit = 1;
		for (int i = 1; i <= 9; i++) {
			statcode += std::min((int)tileCount[suit * TileSuitStep + i], 4) * qDigit;
			qDigit *= 5;
		}
		uint8_t tmpdat = mentsuAnalysisDat[statcode];
		mianzi += (tmpdat & 0x70) >> 4;
		tarzi += tmpdat & 0x0f;
		if (tmpdat & 0x80) atama = true;
	}

	// ���v
	for (int i = 1; i <= 7; i++) {
		if (tileCount[TileSuitHonors + i] == 2) {tarzi++; atama = true;}
		if (tileCount[TileSuitHonors + i] >= 3) {mianzi++;}
	}

	// ���ʎq��ÞȂ�����ꍇ
	mianzi += gameStat->Player[playerID].MeldPointer;
	
	int mianziCount = 0;
	if (mianzi + tarzi > limit) {
		// �ʎq�����̂Ƃ�
		ans = (mianzi * 2) + (limit - mianzi);
		// �ʎq�����ł��A�������鎞�͓���������
		if (atama) ans += 1;
	} else {
		// �����łȂ��Ƃ�
		ans = (mianzi * 2) + tarzi;
	}

	return ans;
}

Shanten ShantenAnalyzer::calcShantenRegular(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount)
{ // �ʎq��̌����������߂�
	return 8 - // �S�������ĂȂ��Ă�Ńo���o����������ʎq��ɑ΂���8�����i���Ύq�ɑ΂��ĂȂ�6�����ɂȂ�j
		chkMianzi(gameStat, playerID, tileCount, 4);
}

Shanten ShantenAnalyzer::calcShantenChiitoi(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount)
{ // ���Ύq�ɑ΂�������������߂�B
	Shanten shanten = 6;
	for (int i = 0; i < TileNonflowerMax; i++)
		// �P���ɑΎq�̐��𒲂ׂ�΂悢
		// �������A�����v�S����Ύq�Q�Ƃ��Ďg���Ă͂Ȃ�Ȃ�
		if (tileCount[i] >= 2) shanten--;
	// �Í�������ꍇ�ɒ��v�Ƃ݂Ȃ��Ȃ��悤�ɂ���
	for (int i = 0; i < TileNonflowerMax; i++)
		if ((tileCount[i] >= 3)&&(shanten < 1)) shanten++;
	// ���ʎq��ÞȂ�����ꍇ�͎��Ύq�͕s�\
	if (gameStat->Player[playerID].MeldPointer > 0) shanten = ShantenImpossible;

	return shanten;
}

Shanten ShantenAnalyzer::calcShantenKokushi(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount)
{ // ���m���o�ɑ΂�������������߂�B
	if (chkGameType(gameStat, SanmaS)) return ShantenImpossible; // ���v�O���ł͕s�\

	TileCode YaojiuPai[13] = {
		CharacterOne, CharacterNine, CircleOne, CircleNine, BambooOne, BambooNine,
		EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
	};
	Shanten shanten = 13; bool atama = false;
	for (int i = 0; i < 13; i++) {
		// ���I��v�P��ނɂ��A�P���J�E���g����B
		if (tileCount[YaojiuPai[i]] >= 2) atama = true; // �A�^�}���
		if (tileCount[YaojiuPai[i]] >= 1) shanten--;
	}
	/* ����������ꍇ */
	if (atama) shanten--;
	// ���ʎq��ÞȂ�����ꍇ�͍��m���o���s�\
	if (gameStat->Player[playerID].MeldPointer > 0) shanten = ShantenImpossible;

	return shanten;
}

void ShantenAnalyzer::setQixingTilePattern(TileCode* const QixingPai, unsigned int pattern) {
	switch (pattern) {
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
	switch (pattern) {
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
	switch (pattern) {
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
}

Shanten ShantenAnalyzer::calcShantenStellar(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount, bool qixing)
{ // ����F�����s��/�S�s�ς̌����������߂�
	if ((!RuleData::chkRuleApplied("stellar_uushii"))&&(qixing)) return ShantenImpossible;
	else if ((!RuleData::chkRuleApplied("quanbukao"))&&(!qixing)) return ShantenImpossible;

	Shanten shanten = ShantenImpossible;
	TileCode QixingZiPai[7] = {
		EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
	};
	int qixingZiPaiCount = 0;
	for (int i = 0; i < 7; i++)
		if (tileCount[QixingZiPai[i]] >= 1)
			qixingZiPaiCount++;
	for (int i = 0; i < 6; i++) {
		TileCode QixingPai[9];
		setQixingTilePattern(QixingPai, i);
		int qixingShuPaiCount = 0;
		for (int i = 0; i < 9; i++)
			if ((tileCount[QixingPai[i]] >= 1) &&
				((qixingShuPaiCount < 7)||(!qixing))) qixingShuPaiCount++;
		Shanten tmpShanten = 13 - qixingZiPaiCount - qixingShuPaiCount;
		// ���ʎq��ÞȂ�����ꍇ�͕s�\
		if (gameStat->Player[playerID].MeldPointer > 0) shanten = ShantenImpossible;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
}

Shanten ShantenAnalyzer::calcShantenCivilWar(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount)
{ // ����F��k�푈�̌����������߂�
	if (!RuleData::chkRuleApplied("civil_war")) return ShantenImpossible;

	Shanten shanten = 13;
	// �ȉ��A�ꖇ�����ׂ�
	for (int i = 0; i < 6; i++) {
		Int8ByTile tileCountTmp;
		for (int j = 0; j < TileNonflowerMax; j++) tileCountTmp[j] = tileCount[j];
		TileCode CivilWarPai[NumOfTilesInHand] = {
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
		for (int j = 0; j < NumOfTilesInHand; j++) {
			if (tileCountTmp[CivilWarPai[j]] >= 1) {
				civilWarPaiCount++;
				tileCountTmp[CivilWarPai[j]]--;
			}
		}
		Shanten tmpShanten = 13 - civilWarPaiCount;
		// ���ʎq��ÞȂ�����ꍇ�͍l���Ȃ�
		if (gameStat->Player[playerID].MeldPointer > 0) shanten = ShantenImpossible;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
}

Shanten ShantenAnalyzer::calcShantenSyzygy(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount)
{ // ����F�f������̌����������߂�
	if (!RuleData::chkRuleApplied("syzygy")) return ShantenImpossible;

	// �ȉ��A�ꖇ�����ׂ�
	Int8ByTile tileCountTmp;
	for (int i = 0; i < TileNonflowerMax; i++) tileCountTmp[i] = tileCount[i];
	TileCode syzygyPai[NumOfTilesInHand] = {
		CircleOne, CircleOne, CircleTwo,
		CircleThree, CircleThree, CircleFour,
		CircleFive, CircleFive, CircleSix,
		CircleSeven, CircleSeven, CircleEight,
		CircleNine, CircleNine
	};
	int syzygyPaiCount = 0;
	for (int i = 0; i < NumOfTilesInHand; i++) {
		if (tileCountTmp[syzygyPai[i]] >= 1) {
			syzygyPaiCount++;
			tileCountTmp[syzygyPai[i]]--;
		}
	}
	// ���ʎq��ÞȂ�����ꍇ�͍l���Ȃ�

	return (gameStat->Player[playerID].MeldPointer > 0) ? ShantenImpossible : (13 - syzygyPaiCount);
}

Shanten ShantenAnalyzer::calcShantenSevenup(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount)
{ // ����F�Z�u���A�b�v�̌����������߂�
	if (!RuleData::chkRuleApplied("sevenup")) return ShantenImpossible;

	Shanten shanten = 13;
	// �ȉ��A�ꖇ�����ׂ�
	for (int i = 0; i < 3; i++) {
		Int8ByTile tileCountTmp;
		for (int j = 0; j < TileNonflowerMax; j++) tileCountTmp[j] = tileCount[j];
		TileCode tileArrange[NumOfTilesInHand] = {
			NoTile, NoTile, NoTile, NoTile,
			NoTile, NoTile, NoTile,
			EastWind, SouthWind, WestWind, NorthWind,
			WhiteDragon, GreenDragon, RedDragon,
		};
		for (int j = 0; j < 7; j++)
			tileArrange[j] = (TileCode)(i * TileSuitStep + j + 1);

		int yakuTileCount = 0;
		for (int j = 0; j < NumOfTilesInHand; j++) {
			if (tileCountTmp[tileArrange[j]] >= 1) {
				yakuTileCount++;
				tileCountTmp[tileArrange[j]]--;
			}
		}
		Shanten tmpShanten = 13 - yakuTileCount;
		// ���ʎq��ÞȂ�����ꍇ�͍l���Ȃ�
		if (gameStat->Player[playerID].MeldPointer > 0) shanten = ShantenImpossible;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
}

Shanten ShantenAnalyzer::calcShantenZuhelong(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount)
{ // ����F�g�����̌����������߂�
	if (!RuleData::chkRuleApplied("zuhelong")) return ShantenImpossible;

	Shanten shanten = ShantenImpossible;
	int qixingZiPaiCount = 0;
	for (int i = 0; i < 6; i++) {
		TileCode QixingPai[9];
		setQixingTilePattern(QixingPai, i);
		int qTileCount = 0;
		Int8ByTile tmpTileCount; memcpy(tmpTileCount.val, tileCount.val, sizeof(tmpTileCount.val));
		for (int i = 0; i < 9; i++) {
			if (tmpTileCount[QixingPai[i]] >= 1) {
				++qTileCount; --tmpTileCount[QixingPai[i]];
			}
		}
		Shanten tmpShanten = 11 - qTileCount - chkMianzi(gameStat, playerID, tmpTileCount, 1);
		// ���ʎq��ÞȂ�2�ȏ゠��ꍇ�͕s�\
		if (gameStat->Player[playerID].MeldPointer > 1) shanten = ShantenImpossible;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
}
