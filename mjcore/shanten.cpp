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
#include "tileutil.h"

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
	case ShantenType::regular:
		return calcShantenRegular(gameStat, playerID, tileCount);
	case ShantenType::pairs:
		return calcShantenChiitoi(gameStat, playerID, tileCount);
	case ShantenType::orphans:
		return calcShantenKokushi(gameStat, playerID, tileCount);
	case ShantenType::stellar:
		return calcShantenStellar(gameStat, playerID, tileCount, true);
	case ShantenType::civilWar:
		return calcShantenCivilWar(gameStat, playerID, tileCount);
	case ShantenType::tohokuGreen:
		return calcShantenTohokuGreen(gameStat, playerID, tileCount);
	case ShantenType::syzygy:
		return calcShantenSyzygy(gameStat, playerID, tileCount);
	case ShantenType::quanbukao:
		return calcShantenStellar(gameStat, playerID, tileCount, false);
	case ShantenType::sevenup:
		return calcShantenSevenup(gameStat, playerID, tileCount);
	case ShantenType::zuhelong:
		return calcShantenZuhelong(gameStat, playerID, tileCount);
	case ShantenType::ninnaji:
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
		if (tileCount[composeNumberTile(TileSuit::honors, i)] == 2) {tarzi++; atama = true;}
		if (tileCount[composeNumberTile(TileSuit::honors, i)] >= 3) {mianzi++;}
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
		case MeldStat::sequenceExposedLower: case MeldStat::sequenceExposedMiddle:
		case MeldStat::sequenceExposedUpper:
			// 順子の時
			tileCount[gameStat->Player[playerID].Meld[i].tile]++;
			tileCount[offsetTileNumber(gameStat->Player[playerID].Meld[i].tile, 1)]++;
			tileCount[offsetTileNumber(gameStat->Player[playerID].Meld[i].tile, 2)]++;
			break;
		case MeldStat::quadExposedLeft: case MeldStat::quadAddedLeft:
		case MeldStat::quadExposedCenter: case MeldStat::quadAddedCenter:
		case MeldStat::quadExposedRight: case MeldStat::quadAddedRight:
		case MeldStat::quadConcealed: // 暗槓も数えてあげましょう……
		case MeldStat::tripletExposedLeft: case MeldStat::tripletExposedCenter:
		case MeldStat::tripletExposedRight:
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
	for (auto i : AllTiles) {
		// 単純に対子の数を調べればよい
		// ただし、同じ牌４枚を対子２つとして使ってはならない
		if (tileCount[i] >= 2) shanten--;
#ifdef GUOBIAO
		if (tileCount[i] >= 4) shanten--; // 中国ルールでは4枚使いしてよい
#endif /* GUOBIAO */
	}
	// 暗刻がある場合に聴牌とみなさないようにする
	for (auto i : AllTiles)
		if ((tileCount[i] >= 3)&&(shanten < 1)) shanten++;
	// 鳴き面子や暗槓がある場合は七対子は不可能
	if (gameStat->Player[playerID].MeldPointer > 0) shanten = ShantenImpossible;

	return shanten;
}

Shanten ShantenAnalyzer::calcShantenKokushi(const GameTable* const gameStat, PlayerID playerID, const Int8ByTile& tileCount)
{ // 国士無双に対する向聴数を求める。
	if (gameStat->chkGameType(GameTypeID::sanmaS)) return ShantenImpossible; // 数牌三麻では不可能

	TileCode YaojiuPai[13] = {
		TileCode::characterOne, TileCode::characterNine, TileCode::circleOne, TileCode::circleNine, TileCode::bambooOne, TileCode::bambooNine,
		TileCode::eastWind, TileCode::southWind, TileCode::westWind, TileCode::northWind, TileCode::whiteDragon, TileCode::greenDragon, TileCode::redDragon
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
		QixingPai[0] = TileCode::characterOne; QixingPai[1] = TileCode::characterFour;
		QixingPai[2] = TileCode::characterSeven; break;
	case 2: case 3:
		QixingPai[0] = TileCode::characterTwo; QixingPai[1] = TileCode::characterFive;
		QixingPai[2] = TileCode::characterEight; break;
	case 4: case 5:
		QixingPai[0] = TileCode::characterThree; QixingPai[1] = TileCode::characterSix;
		QixingPai[2] = TileCode::characterNine; break;
	}
	switch (pattern) {
	case 2: case 4:
		QixingPai[3] = TileCode::circleOne; QixingPai[4] = TileCode::circleFour;
		QixingPai[5] = TileCode::circleSeven; break;
	case 0: case 5:
		QixingPai[3] = TileCode::circleTwo; QixingPai[4] = TileCode::circleFive;
		QixingPai[5] = TileCode::circleEight; break;
	case 1: case 3:
		QixingPai[3] = TileCode::circleThree; QixingPai[4] = TileCode::circleSix;
		QixingPai[5] = TileCode::circleNine; break;
	}
	switch (pattern) {
	case 3: case 5:
		QixingPai[6] = TileCode::bambooOne; QixingPai[7] = TileCode::bambooFour;
		QixingPai[8] = TileCode::bambooSeven; break;
	case 1: case 4:
		QixingPai[6] = TileCode::bambooTwo; QixingPai[7] = TileCode::bambooFive;
		QixingPai[8] = TileCode::bambooEight; break;
	case 0: case 2:
		QixingPai[6] = TileCode::bambooThree; QixingPai[7] = TileCode::bambooSix;
		QixingPai[8] = TileCode::bambooNine; break;
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
		TileCode::eastWind, TileCode::southWind, TileCode::westWind, TileCode::northWind, TileCode::whiteDragon, TileCode::greenDragon, TileCode::redDragon
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
			TileCode::noTile, TileCode::noTile, TileCode::noTile, TileCode::noTile,
			TileCode::noTile, TileCode::noTile, TileCode::noTile, TileCode::noTile,
			TileCode::southWind, TileCode::southWind, TileCode::southWind,
			TileCode::northWind, TileCode::northWind, TileCode::northWind
		};
		switch (i) {
		case 0: case 1:
			CivilWarPai[0] = TileCode::characterOne; CivilWarPai[1] = CivilWarPai[2] = TileCode::characterEight;
			CivilWarPai[3] = TileCode::characterOne; break;
		case 2: case 3:
			CivilWarPai[0] = TileCode::circleOne; CivilWarPai[1] = CivilWarPai[2] = TileCode::circleEight;
			CivilWarPai[3] = TileCode::circleOne; break;
		case 4: case 5:
			CivilWarPai[0] = TileCode::bambooOne; CivilWarPai[1] = CivilWarPai[2] = TileCode::bambooEight;
			CivilWarPai[3] = TileCode::bambooOne; break;
		}
		switch (i) {
		case 2: case 4:
			CivilWarPai[4] = TileCode::characterOne; CivilWarPai[5] = CivilWarPai[6] = TileCode::characterEight;
			CivilWarPai[7] = TileCode::characterFive; break;
		case 0: case 5:
			CivilWarPai[4] = TileCode::circleOne; CivilWarPai[5] = CivilWarPai[6] = TileCode::circleEight;
			CivilWarPai[7] = TileCode::circleFive; break;
		case 1: case 3:
			CivilWarPai[4] = TileCode::bambooOne; CivilWarPai[5] = CivilWarPai[6] = TileCode::bambooEight;
			CivilWarPai[7] = TileCode::bambooFive; break;
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
			TileCode::characterOne,   TileCode::characterTwo,   TileCode::characterThree,
			TileCode::characterFour,  TileCode::characterFive,  TileCode::characterSix,
			TileCode::characterSeven, TileCode::characterEight, TileCode::characterNine,
			TileCode::eastWind,       TileCode::eastWind,       TileCode::noTile,
			TileCode::northWind,      TileCode::northWind
		};
		switch (i) {
		case 0: case 1:
			TohokuGreenPai[1] = TileCode::bambooTwo; break;
		case 2: case 3:
			TohokuGreenPai[2] = TileCode::bambooThree; break;
		case 4: case 5:
			TohokuGreenPai[3] = TileCode::bambooFour; break;
		case 6: case 7:
			TohokuGreenPai[5] = TileCode::bambooSix; break;
		case 8: case 9:
			TohokuGreenPai[7] = TileCode::bambooEight; break;
		}
		TohokuGreenPai[11] = (i % 2 == 0) ? TileCode::eastWind : TileCode::northWind;
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
		TileCode::circleOne, TileCode::circleOne, TileCode::circleTwo,
		TileCode::circleThree, TileCode::circleThree, TileCode::circleFour,
		TileCode::circleFive, TileCode::circleFive, TileCode::circleSix,
		TileCode::circleSeven, TileCode::circleSeven, TileCode::circleEight,
		TileCode::circleNine, TileCode::circleNine
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
			TileCode::noTile, TileCode::noTile, TileCode::noTile, TileCode::noTile,
			TileCode::noTile, TileCode::noTile, TileCode::noTile,
			TileCode::eastWind, TileCode::southWind, TileCode::westWind, TileCode::northWind,
			TileCode::whiteDragon, TileCode::greenDragon, TileCode::redDragon,
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
			TileCode::characterTwo, TileCode::southWind, TileCode::characterFour,
			TileCode::circleTwo,    TileCode::southWind, TileCode::circleFour,
			TileCode::bambooTwo,    TileCode::southWind, TileCode::bambooFour,
			TileCode::eastWind, TileCode::southWind, TileCode::westWind, TileCode::northWind,
			i == 0 ? TileCode::circleEight : TileCode::whiteDragon,
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
