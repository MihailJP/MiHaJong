#include "yaku.h"


/* 順子の処理 */
bool yaku::mentsuParser::makementsu_shuntsu(Int8ByTile& countForMentsu, MeldBuf MianziDat,
	int* const ProcessedMelds, TileCode tile)
{ /* 順子の処理 */
	if ((countForMentsu[tile] >= 1)&&
		(countForMentsu[tile+1] >= 1)&&
		(countForMentsu[tile+2] >= 1)) {
			MianziDat[*ProcessedMelds].mstat = meldSequenceConcealed;
			MianziDat[(*ProcessedMelds)++].tile = tile;
			--countForMentsu[tile]; --countForMentsu[tile+1];
			--countForMentsu[tile+2];
			return true;
	}
	return false;
}

/* 刻子の処理 */
void yaku::mentsuParser::makementsu_koutsu(Int8ByTile& countForMentsu, MeldBuf MianziDat,
	int* const ProcessedMelds, TileCode tile)
{ /* 刻子の処理 */
	if (countForMentsu[tile] >= 3) {
		MianziDat[*ProcessedMelds].mstat = meldTripletConcealed;
		MianziDat[(*ProcessedMelds)++].tile = tile;
		countForMentsu[tile] -= 3;
	}
	return;
}

/* 面子に分解する */
void yaku::mentsuParser::makementsu(const GameTable* const gameStat,
	PlayerID targetPlayer, ParseMode AtamaCode, int* const ProcessedMelds_, MeldBuf MianziDat)
{ /* 面子に分解する */
	memset(MianziDat, 0, sizeof(MeldBuf)); // 初期化
	auto countForMentsu = countTilesInHand(gameStat, targetPlayer);
	int processedMelds;
	int* const ProcessedMelds = ProcessedMelds_ ? ProcessedMelds_ : &processedMelds;
	*ProcessedMelds = 0;

	// 雀頭となり得ない牌なら戻る
	if (countForMentsu[AtamaCode.AtamaCode] < 2) return;
	MianziDat[0].tile = AtamaCode.AtamaCode; (*ProcessedMelds)++;
	countForMentsu[AtamaCode.AtamaCode] -= 2;

	// 順子(順子優先正順モードの時)
	if (AtamaCode.Order == Shun_Ke)
		for (int i = 1; i < TileSuitHonors; i++)
			if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, (TileCode)i))
				--i;
	// 順子(順子優先逆順モードの時)
	if (AtamaCode.Order == Shun_Ke_Rev)
		for (int i = TileSuitHonors - 1; i > 0; i--)
			if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, (TileCode)i))
				++i;

	// 暗刻(正順モードの時)
	if ((AtamaCode.Order == Ke_Shun)||(AtamaCode.Order == Shun_Ke))
		for (int i = 1; i < TileNonflowerMax; i++)
			makementsu_koutsu(countForMentsu, MianziDat, ProcessedMelds, (TileCode)i);
	// 暗刻(逆順モードの時)
	if ((AtamaCode.Order == Ke_Shun_Rev)||(AtamaCode.Order == Shun_Ke_Rev))
		for (int i = TileNonflowerMax - 1; i > 0; i--)
			makementsu_koutsu(countForMentsu, MianziDat, ProcessedMelds, (TileCode)i);

	// 順子(暗刻優先正順モードの時)
	if (AtamaCode.Order == Ke_Shun)
		for (int i = 1; i < TileSuitHonors; i++)
			if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, (TileCode)i))
				--i;
	// 順子(暗刻優先逆順モードの時)
	if (AtamaCode.Order == Ke_Shun_Rev)
		for (int i = TileSuitHonors - 1; i > 0; i--)
			if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, (TileCode)i))
				++i;

	// 鳴いた面子、暗槓
	for (int i = 1; i <= gameStat->Player[targetPlayer].MeldPointer; i++) {
		MianziDat[*ProcessedMelds].mstat = gameStat->Player[targetPlayer].Meld[i].mstat;
		MianziDat[*ProcessedMelds].tile = gameStat->Player[targetPlayer].Meld[i].tile;
		/*for (int j = 0; j < 4; j++)
			MianziDat[*ProcessedMelds].red[j] = gameStat->Player[targetPlayer].Meld[i].red[j];*/
		++(*ProcessedMelds);
	}
}

// -------------------------------------------------------------------------

/* 指定の対子があるか数える */
int yaku::countingFacility::countPairs(
	const Int8ByTile tileCount, const TileCode* const targetTiles, int numOfTiles)
{
	// 指定した種類の対子を数える
	trace(_T("対子の種類を調べます。"));
	int yakuflagcount = 0;
	for (int i = 0; i < numOfTiles; i++)
		if (tileCount[targetTiles[i]] >= 2) yakuflagcount++;
	return yakuflagcount;
}

/* 数字の合計を数える(七対子版) */
int yaku::countingFacility::countTileNumerals(const Int8ByTile tileCount) {
	/* 数字の合計を数える */
	int Cifr = 0;
	for (int i = 1; i < (TileSuitHonors - 1); i++)
		Cifr += tileCount[i] * (i % TileSuitStep);
	return Cifr;
}
	
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* 計数用関数 */
Int8ByTile yaku::countingFacility::countByMelds(
	const MeldBuf MianziDat, uint8_t* const hits,
	std::function<bool (MeldStat)> f)
{ // 計数ルーチン
	Int8ByTile hitCount; memset(&hitCount, 0, sizeof(hitCount));
	if (hits != nullptr) *hits = 0;
	for (int i = 1; i < SizeOfMeldBuffer; i++) {
		if (f(MianziDat[i].mstat)&&(MianziDat[i].tile != NoTile)) {
			++(hitCount[MianziDat[i].tile]);
			if (hits != nullptr) ++(*hits);
		}
	}
	return hitCount;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* 刻子の数を数える */
Int8ByTile yaku::countingFacility::countKez(const MeldBuf MianziDat, uint8_t* const Kezi) { /* 刻子の数を数える */
	trace(_T("刻子・槓子の種類を調べます。"));
	return countByMelds(MianziDat, Kezi, [](MeldStat x){return x >= meldTripletConcealed;});
}

/* 暗刻子の数を数える */
Int8ByTile yaku::countingFacility::countAnKez(const MeldBuf MianziDat, uint8_t* const Kezi) { /* 暗刻子の数を数える */
	trace(_T("暗刻子・暗槓子の種類を調べます。"));
	return countByMelds(MianziDat, Kezi, [](MeldStat x){return ((x == meldTripletConcealed)||(x == meldQuadConcealed));});
}

/* 対子・刻子・槓子の数を数える */
Int8ByTile yaku::countingFacility::countDuiz(const MeldBuf MianziDat) { /* 対子・刻子・槓子の数を数える */
	trace(_T("対子・刻子・槓子の種類を調べます。"));
	auto DuiCount = countKez(MianziDat, nullptr);
	++(DuiCount[MianziDat[0].tile]);
	return DuiCount;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* 順子の数を数える */
Int8ByTile yaku::countingFacility::countShunz(const MeldBuf MianziDat, uint8_t* const Shunzi) { /* 順子の数を数える */
	trace(_T("順子の種類を調べます。"));
	return countByMelds(MianziDat, Shunzi, [](MeldStat x){return x < meldTripletConcealed;});
}

/* 暗順子の数を数える */
Int8ByTile yaku::countingFacility::countAnShunz(const MeldBuf MianziDat, uint8_t* const Shunzi) { /* 暗順子の数を数える */
	trace(_T("暗順子の種類を調べます。"));
	return countByMelds(MianziDat, Shunzi, [](MeldStat x){return x == meldSequenceConcealed;});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* 槓子の数を数える */
Int8ByTile yaku::countingFacility::countKangz(const MeldBuf MianziDat, uint8_t* const Kangzi) { /* 槓子の数を数える */
	trace(_T("槓子の種類を調べます。"));
	return countByMelds(MianziDat, Kangzi, [](MeldStat x){return x >= meldQuadConcealed;});
}

/* 暗槓子の数を数える */
Int8ByTile yaku::countingFacility::countAnKangz(const MeldBuf MianziDat, uint8_t* const Kangzi) { /* 暗槓子の数を数える */
	trace(_T("暗槓子の種類を調べます。"));
	return countByMelds(MianziDat, Kangzi, [](MeldStat x){return x == meldQuadConcealed;});
}

/* 加槓の数を数える */
Int8ByTile yaku::countingFacility::countKaKangz(const MeldBuf MianziDat, uint8_t* const Kangzi) { /* 加槓の数を数える */
	trace(_T("加槓の種類を調べます。"));
	return countByMelds(MianziDat, Kangzi, [](MeldStat x){return x >= meldQuadAddedLeft;});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* 指定の刻子・順子があるか数える */
inline int yaku::countingFacility::countSpecMentz(const MeldBuf MianziDat, const TileCode* const targetKez, int numOfKez,
	const TileCode* const targetShunz, int numOfShunz, bool Mode, bool allowDup)
{ // 指定した種類の面子を数える
	int kz = 0; int sz = 0;
	auto DuiCount = Mode ? countKez(MianziDat, nullptr) : countDuiz(MianziDat);
	auto ShunzCount = countShunz(MianziDat, nullptr);
	int yakuflagcount = 0;
	for (int i = 0; i < numOfKez; i++) {
		if (targetKez[i] == NoTile) continue;
		if (DuiCount[targetKez[i]] >= 1) ++yakuflagcount;
	}
	for (int i = 0; i < numOfShunz; i++) {
		if (targetShunz[i] == NoTile) continue;
		if (ShunzCount[targetShunz[i]] >= 1)
			yakuflagcount += allowDup ? ShunzCount[targetShunz[i]] : 1;
	}
	return yakuflagcount;
}
int yaku::countingFacility::countSpecMentz(const MeldBuf MianziDat, const TileCode* const targetKez, int numOfKez,
	const TileCode* const targetShunz, int numOfShunz, bool Mode)
{
	return countSpecMentz(MianziDat, targetKez, numOfKez, targetShunz, numOfShunz, Mode, false);
}
int yaku::countingFacility::countSpecMentzWithDup(const MeldBuf MianziDat, const TileCode* const targetKez, int numOfKez,
	const TileCode* const targetShunz, int numOfShunz, bool Mode)
{
	return countSpecMentz(MianziDat, targetKez, numOfKez, targetShunz, numOfShunz, Mode, true);
}

/* 数字の合計を数える */
int yaku::countingFacility::countMentzNumerals(const MeldBuf MianziDat) { /* 数字の合計を数える */
	int Cifr = 0;
	for (int i = 0; i < SizeOfMeldBuffer; i++) {
		if (MianziDat[i].tile % TileCodeMaximum < TileSuitHonors) {
			if (i == 0) { // 雀頭
				Cifr += (MianziDat[0].tile % TileSuitStep) * 2;
			} else { // 面子
				switch (MianziDat[i].mstat) {
				case meldSequenceConcealed: case meldSequenceExposedLower:
				case meldSequenceExposedMiddle: case meldSequenceExposedUpper:
					Cifr += (((MianziDat[i].tile % TileSuitStep)+1) * 3); // 順子
					break;
				case meldTripletConcealed: case meldTripletExposedLeft:
				case meldTripletExposedCenter: case meldTripletExposedRight:
					Cifr += ((MianziDat[i].tile % TileSuitStep) * 3); // 刻子
					break;
				default:
					Cifr += ((MianziDat[i].tile % TileSuitStep) * 4); // 槓子
					break;
				}
			}
		}
	}
	return Cifr;
}
