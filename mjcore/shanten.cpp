#define NOMINMAX
#include "shanten.h"

#ifdef _WIN32
#include <windows.h>
#endif /*_WIN32*/
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <string>
#include <algorithm>
#include <cassert>
#include "except.h"
#include "func.h"
#include "logging.h"
#include "reader/readrsrc.h"
#include "../common/strcode.h"
#include "decomp.h"
#include "ruletbl.h"

using std::min;

/* 面子データ初期化 */

uint8_t* ShantenAnalyzer::mentsuAnalysisDat = nullptr;

void ShantenAnalyzer::initMentsuAnalysisDat() { // 面子データ初期化
	Compressed::file_mentz_dat* mnzdat = new Compressed::file_mentz_dat();
	mentsuAnalysisDat = new uint8_t[mnzdat->getDataSize()+4];
#ifdef _MSC_VER
	memcpy_s(mentsuAnalysisDat, mnzdat->getDataSize()+4, mnzdat->getData(), mnzdat->getDataSize());
#else
	memcpy(mentsuAnalysisDat, mnzdat->getData(), mnzdat->getDataSize());
#endif
	delete mnzdat;
}

/* 向聴数を計算する */

MJCORE Shanten ShantenAnalyzer::calcShanten(const GameTable* const gameStat, PlayerID playerID, ShantenType mode)
{ // 向聴数を計算する
	/* 数牌それぞれの面子の数を数える */
	Int8ByTile tileCount = countTilesInHand(gameStat, playerID);

	/* mode別分岐 */
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
	case shantenTohokuGreen:
		return calcShantenTohokuGreen(gameStat, playerID, tileCount);
	case shantenSyzygy:
		return calcShantenSyzygy(gameStat, playerID, tileCount);
	case shantenQuanbukao:
		return calcShantenStellar(gameStat, playerID, tileCount, false);
	case shantenSevenup:
		return calcShantenSevenup(gameStat, playerID, tileCount);
	case shantenZuhelong:
		return calcShantenZuhelong(gameStat, playerID, tileCount);
	case shantenNinnaji:
		return calcShantenNinnaji(gameStat, playerID, tileCount);
	default:
		/* 全部求めて一番和了に近いやつを返す */
		Shanten shanten, tmpShanten;
		shanten = calcShantenRegular(gameStat, playerID, tileCount);
		tmpShanten = calcShantenChiitoi(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenKokushi(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenStellar(gameStat, playerID, tileCount, true); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenCivilWar(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenTohokuGreen(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenSyzygy(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenStellar(gameStat, playerID, tileCount, false); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenSevenup(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenZuhelong(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		tmpShanten = calcShantenNinnaji(gameStat, playerID, tileCount); if (tmpShanten < shanten) shanten = tmpShanten;
		return shanten;
	}
}

unsigned int ShantenAnalyzer::chkMianzi(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount, unsigned limit, unsigned mode) {
	// 面子を2、対子・塔子を1とした数値
	unsigned int ans = 0;
	// 数牌
	int mianzi = 0; int tarzi = 0; bool atama = false; // 面子塔子雀頭の数
	for (int suit = 0; suit < TileNumeralColors; suit++) {
		unsigned int statcode = 0; unsigned int qDigit = 1;
		for (int i = 1; i <= 9; i++) {
			statcode += std::min(static_cast<int>(tileCount[suit * TileSuitStep + i]), 4) * qDigit;
			qDigit *= 5;
		}
		uint8_t tmpdat = mentsuAnalysisDat[statcode * 4 + ((mode & (3 << (2 * suit))) >> (2 * suit))];
		mianzi += (tmpdat & 0x70) >> 4;
		tarzi += tmpdat & 0x0f;
		if (tmpdat & 0x80) atama = true;
	}

	// 字牌
	for (int i = 1; i <= 7; i++) {
		if (tileCount[TileSuitHonors + i] == 2) {tarzi++; atama = true;}
		if (tileCount[TileSuitHonors + i] >= 3) {mianzi++;}
	}

	// 鳴き面子や暗槓がある場合
	mianzi += gameStat->Player[playerID].MeldPointer;
	
	int mianziCount = 0;
	if (mianzi + tarzi > static_cast<int>(limit)) {
		// 面子多多のとき
		ans = (mianzi * 2) + (limit - mianzi);
		// 面子多多でも、頭がある時は頭も数える
		if (atama) ans += 1;
	} else {
		// そうでないとき
		ans = (mianzi * 2) + tarzi;
	}

	return ans;
}
unsigned int ShantenAnalyzer::chkMianzi(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount, unsigned limit) {
	unsigned int melds = 0u;
	for (unsigned i = 0; i < 64; ++i)
		melds = std::max(melds, chkMianzi(gameStat, playerID, tileCount, limit, i));
	return melds;
}

void ShantenAnalyzer::addExposedMeld(const GameTable* const gameStat, PlayerID playerID, Int8ByTile& tileCount) {
	// 鳴き面子を数える
	for (int i = 1; i < gameStat->Player[playerID].MeldPointer; i++) {
		switch (gameStat->Player[playerID].Meld[i].mstat) {
		case meldSequenceExposedLower: case meldSequenceExposedMiddle:
		case meldSequenceExposedUpper:
			// 順子の時
			tileCount[gameStat->Player[playerID].Meld[i].tile]++;
			tileCount[gameStat->Player[playerID].Meld[i].tile + 1]++;
			tileCount[gameStat->Player[playerID].Meld[i].tile + 2]++;
			break;
		case meldQuadExposedLeft: case meldQuadAddedLeft:
		case meldQuadExposedCenter: case meldQuadAddedCenter:
		case meldQuadExposedRight: case meldQuadAddedRight:
		case meldQuadConcealed: // 暗槓も数えてあげましょう……
		case meldTripletExposedLeft: case meldTripletExposedCenter:
		case meldTripletExposedRight:
			// 刻子の時(槓子も含む)
			tileCount[gameStat->Player[playerID].Meld[i].tile] += 3;
			break;
		default:
			// 異常データ
			RaiseTolerant(EXCEPTION_MJCORE_INVALID_DATA, _T("副露データに暗順子、暗刻子、または不明な種類の面子が検出されました"));
		}
	}
}

Shanten ShantenAnalyzer::calcShantenRegular(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount)
{ // 面子手の向聴数を求める
	return 8 - // 全く揃ってないてんでバラバラだったら面子手に対して8向聴（七対子に対してなら6向聴になる）
		chkMianzi(gameStat, playerID, tileCount, 4);
}

Shanten ShantenAnalyzer::calcShantenChiitoi(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount)
{ // 七対子に対する向聴数を求める。
	Shanten shanten = 6;
	for (int i = 0; i < TileNonflowerMax; i++) {
		// 単純に対子の数を調べればよい
		// ただし、同じ牌４枚を対子２つとして使ってはならない
		if (tileCount[i] >= 2) shanten--;
#ifdef GUOBIAO
		if (tileCount[i] >= 4) shanten--; // 中国ルールでは4枚使いしてよい
#endif /* GUOBIAO */
	}
	// 暗刻がある場合に聴牌とみなさないようにする
	for (int i = 0; i < TileNonflowerMax; i++)
		if ((tileCount[i] >= 3)&&(shanten < 1)) shanten++;
	// 鳴き面子や暗槓がある場合は七対子は不可能
	if (gameStat->Player[playerID].MeldPointer > 0) shanten = ShantenImpossible;

	return shanten;
}

Shanten ShantenAnalyzer::calcShantenKokushi(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount)
{ // 国士無双に対する向聴数を求める。
	if (gameStat->chkGameType(SanmaS)) return ShantenImpossible; // 数牌三麻では不可能

	TileCode YaojiuPai[13] = {
		CharacterOne, CharacterNine, CircleOne, CircleNine, BambooOne, BambooNine,
		EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
	};
	Shanten shanten = 13; bool atama = false;
	for (int i = 0; i < 13; i++) {
		// ヤオ九牌１種類につき、１をカウントする。
		if (tileCount[YaojiuPai[i]] >= 2) atama = true; // アタマ候補
		if (tileCount[YaojiuPai[i]] >= 1) shanten--;
	}
	/* 雀頭がある場合 */
	if (atama) shanten--;
	// 鳴き面子や暗槓がある場合は国士無双も不可能
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

Shanten ShantenAnalyzer::calcShantenStellar(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount, bool qixing)
{ // 特殊：七星不靠/全不靠の向聴数を求める
#ifndef GUOBIAO
	if ((!RuleData::chkRuleApplied("stellar_uushii"))&&(qixing)) return ShantenImpossible;
	else if ((!RuleData::chkRuleApplied("quanbukao"))&&(!qixing)) return ShantenImpossible;
#endif /* GUOBIAO */

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
		// 鳴き面子や暗槓がある場合は不可能
		if (gameStat->Player[playerID].MeldPointer > 0) shanten = ShantenImpossible;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
}

Shanten ShantenAnalyzer::calcShantenCivilWar(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount)
{ // 特殊：南北戦争の向聴数を求める
#ifdef GUOBIAO
	return ShantenImpossible;
#else /* GUOBIAO */
	if (!RuleData::chkRuleApplied("civil_war")) return ShantenImpossible;

	Shanten shanten = 13;
	// 以下、一枚ずつ調べる
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
		// 鳴き面子や暗槓がある場合は考えない
		if (gameStat->Player[playerID].MeldPointer > 0) shanten = ShantenImpossible;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
#endif /* GUOBIAO */
}

Shanten ShantenAnalyzer::calcShantenTohokuGreen(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount)
{ // 特殊：東北新幹線グリーン車の向聴数を求める
#ifdef GUOBIAO
	return ShantenImpossible;
#else /* GUOBIAO */
	if (!RuleData::chkRuleApplied("tohoku_shinkansen_green")) return ShantenImpossible;

	Shanten shanten = 13;
	// 以下、一枚ずつ調べる
	for (int i = 0; i < 10; i++) {
		Int8ByTile tileCountTmp;
		for (int j = 0; j < TileNonflowerMax; j++) tileCountTmp[j] = tileCount[j];
		addExposedMeld(gameStat, playerID, tileCountTmp); // 鳴き有効
		TileCode TohokuGreenPai[NumOfTilesInHand] = {
			CharacterOne,   CharacterTwo,   CharacterThree,
			CharacterFour,  CharacterFive,  CharacterSix,
			CharacterSeven, CharacterEight, CharacterNine,
			EastWind,       EastWind,       NoTile,
			NorthWind,      NorthWind
		};
		switch (i) {
		case 0: case 1:
			TohokuGreenPai[1] = BambooTwo; break;
		case 2: case 3:
			TohokuGreenPai[2] = BambooThree; break;
		case 4: case 5:
			TohokuGreenPai[3] = BambooFour; break;
		case 6: case 7:
			TohokuGreenPai[5] = BambooSix; break;
		case 8: case 9:
			TohokuGreenPai[7] = BambooEight; break;
		}
		TohokuGreenPai[11] = (i % 2 == 0) ? EastWind : NorthWind;
		int tohokuGreenPaiCount = 0;
		for (int j = 0; j < NumOfTilesInHand; j++) {
			if (tileCountTmp[TohokuGreenPai[j]] >= 1) {
				tohokuGreenPaiCount++;
				tileCountTmp[TohokuGreenPai[j]]--;
			}
		}
		Shanten tmpShanten = 13 - tohokuGreenPaiCount;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
#endif /* GUOBIAO */
}

Shanten ShantenAnalyzer::calcShantenSyzygy(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount)
{ // 特殊：惑星直列の向聴数を求める
#ifdef GUOBIAO
	return ShantenImpossible;
#else /* GUOBIAO */
	if (!RuleData::chkRuleApplied("syzygy")) return ShantenImpossible;

	// 以下、一枚ずつ調べる
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
	// 鳴き面子や暗槓がある場合は考えない

	return (gameStat->Player[playerID].MeldPointer > 0) ? ShantenImpossible : (13 - syzygyPaiCount);
#endif /* GUOBIAO */
}

Shanten ShantenAnalyzer::calcShantenSevenup(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount)
{ // 特殊：セブンアップの向聴数を求める
#ifdef GUOBIAO
	return ShantenImpossible;
#else /* GUOBIAO */
	if (!RuleData::chkRuleApplied("sevenup")) return ShantenImpossible;

	Shanten shanten = 13;
	// 以下、一枚ずつ調べる
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
			tileArrange[j] = static_cast<TileCode>(i * TileSuitStep + j + 1);

		int yakuTileCount = 0;
		for (int j = 0; j < NumOfTilesInHand; j++) {
			if (tileCountTmp[tileArrange[j]] >= 1) {
				yakuTileCount++;
				tileCountTmp[tileArrange[j]]--;
			}
		}
		Shanten tmpShanten = 13 - yakuTileCount;
		// 鳴き面子や暗槓がある場合は考えない
		if (gameStat->Player[playerID].MeldPointer > 0) shanten = ShantenImpossible;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
#endif /* GUOBIAO */
}

Shanten ShantenAnalyzer::calcShantenZuhelong(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount)
{ // 特殊：組合龍の向聴数を求める
#ifndef GUOBIAO
	if (!RuleData::chkRuleApplied("zuhelong")) return ShantenImpossible;
#endif /* GUOBIAO */

	Shanten shanten = ShantenImpossible;
	int qixingZiPaiCount = 0;
	for (int i = 0; i < 6; i++) {
		TileCode QixingPai[9];
		setQixingTilePattern(QixingPai, i);
		int qTileCount = 0;
		Int8ByTile tmpTileCount;
		for (int i = 0; i < 9; i++) {
			if (tmpTileCount[QixingPai[i]] >= 1) {
				++qTileCount; --tmpTileCount[QixingPai[i]];
			}
		}
		Shanten tmpShanten = 11 - qTileCount - chkMianzi(gameStat, playerID, tmpTileCount, 1);
		// 鳴き面子や暗槓が2つ以上ある場合は不可能
		if (gameStat->Player[playerID].MeldPointer > 1) shanten = ShantenImpossible;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
}

Shanten ShantenAnalyzer::calcShantenNinnaji(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount)
{ // 特殊：仁和寺の向聴数を求める
#ifdef GUOBIAO
	return ShantenImpossible;
#else /* GUOBIAO */
	if (!RuleData::chkRuleApplied("ninnaji")) return ShantenImpossible;

	Shanten shanten = 13;
	// 以下、一枚ずつ調べる
	for (int i = 0; i < 2; i++) {
		Int8ByTile tileCountTmp;
		for (int j = 0; j < TileNonflowerMax; j++) tileCountTmp[j] = tileCount[j];
		TileCode tileArrange[NumOfTilesInHand] = {
			CharacterTwo, SouthWind, CharacterFour,
			CircleTwo,    SouthWind, CircleFour,
			BambooTwo,    SouthWind, BambooFour,
			EastWind, SouthWind, WestWind, NorthWind,
			i == 0 ? CircleEight : WhiteDragon,
		};

		int yakuTileCount = 0;
		for (int j = 0; j < NumOfTilesInHand; j++) {
			if (tileCountTmp[tileArrange[j]] >= 1) {
				yakuTileCount++;
				tileCountTmp[tileArrange[j]]--;
			}
		}
		Shanten tmpShanten = 13 - yakuTileCount;
		// 鳴き面子や暗槓がある場合は考えない
		if (gameStat->Player[playerID].MeldPointer > 0) shanten = ShantenImpossible;
		if (tmpShanten < shanten) shanten = tmpShanten;
	}

	return shanten;
#endif /* GUOBIAO */
}
