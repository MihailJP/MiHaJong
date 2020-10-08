#include "yaku.h"
#include "yk_tools.h"

#define COUNT_PAIRS(dSize) template int yaku::countingFacility::countPairs<dSize>(const Int8ByTile, const std::array<TileCode, dSize>&)
#define COUNT_SPEC_MENTZ(kSize, sSize) template int yaku::countingFacility::countSpecMentz<kSize, sSize>(const MeldBuf, const std::array<TileCode, kSize>&, const std::array<TileCode, sSize>&, bool, bool)

/* 順子の処理 */
bool yaku::mentsuParser::makementsu_shuntsu(Int8ByTile& countForMentsu, MeldBuf MianziDat,
	int* const ProcessedMelds, TileCode tile)
{ /* 順子の処理 */
	if ((countForMentsu[tile] >= 1)&&
		(countForMentsu[offsetTileNumber(tile, 1)] >= 1)&&
		(countForMentsu[offsetTileNumber(tile, 2)] >= 1)) {
			MianziDat[*ProcessedMelds].mstat = MeldStat::sequenceConcealed;
			MianziDat[(*ProcessedMelds)++].tile = tile;
			--countForMentsu[tile]; --countForMentsu[offsetTileNumber(tile, 1)];
			--countForMentsu[offsetTileNumber(tile, 2)];
			return true;
	}
	return false;
}

/* 刻子の処理 */
void yaku::mentsuParser::makementsu_koutsu(Int8ByTile& countForMentsu, MeldBuf MianziDat,
	int* const ProcessedMelds, TileCode tile)
{ /* 刻子の処理 */
	if (countForMentsu[tile] >= 3) {
		MianziDat[*ProcessedMelds].mstat = MeldStat::tripletConcealed;
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
	if (AtamaCode.Order == ParseOrder::Shun_Ke)
		for (auto i = NumberTiles.begin(); i != NumberTiles.end(); i++)
			if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, *i))
				--i;
	// 順子(順子優先逆順モードの時)
	if (AtamaCode.Order == ParseOrder::Shun_Ke_Rev)
		for (auto i = NumberTiles.rbegin(); i != NumberTiles.rend(); i++)
			if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, *i))
				--i;

	// 暗刻(正順モードの時)
	if ((AtamaCode.Order == ParseOrder::Ke_Shun)||(AtamaCode.Order == ParseOrder::Shun_Ke))
		for (auto i = AllTiles.begin(); i != AllTiles.end(); i++)
			makementsu_koutsu(countForMentsu, MianziDat, ProcessedMelds, *i);
	// 暗刻(逆順モードの時)
	if ((AtamaCode.Order == ParseOrder::Ke_Shun_Rev)||(AtamaCode.Order == ParseOrder::Shun_Ke_Rev))
		for (auto i = AllTiles.rbegin(); i != AllTiles.rend(); i++)
			makementsu_koutsu(countForMentsu, MianziDat, ProcessedMelds, *i);

	// 順子(暗刻優先正順モードの時)
	if (AtamaCode.Order == ParseOrder::Ke_Shun)
		for (auto i = NumberTiles.begin(); i != NumberTiles.end(); i++)
			if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, *i))
				--i;
	// 順子(暗刻優先逆順モードの時)
	if (AtamaCode.Order == ParseOrder::Ke_Shun_Rev)
		for (auto i = NumberTiles.rbegin(); i != NumberTiles.rend(); i++)
			if (makementsu_shuntsu(countForMentsu, MianziDat, ProcessedMelds, *i))
				--i;

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
template<std::size_t dSize> int yaku::countingFacility::countPairs(
	const Int8ByTile tileCount, const std::array<TileCode, dSize>& targetTiles)
{
	// 指定した種類の対子を数える
	trace(_T("対子の種類を調べます。"));
	int yakuflagcount = 0;
	for (auto i : targetTiles)
		if (tileCount[i] >= 2) yakuflagcount++;
	return yakuflagcount;
}

COUNT_PAIRS( 2);
COUNT_PAIRS( 3);
COUNT_PAIRS( 5);
COUNT_PAIRS( 6);
COUNT_PAIRS( 7);
COUNT_PAIRS( 8);
COUNT_PAIRS( 9);
COUNT_PAIRS(10);
COUNT_PAIRS(11);
COUNT_PAIRS(12);
COUNT_PAIRS(13);
COUNT_PAIRS(14);
COUNT_PAIRS(15);
COUNT_PAIRS(20);

/* 数字の合計を数える(七対子版) */
int yaku::countingFacility::countTileNumerals(const Int8ByTile tileCount) {
	/* 数字の合計を数える */
	int Cifr = 0;
	for (auto i = NumberTiles.begin(); i != NumberTiles.end(); i++)
		Cifr += tileCount[*i] * getTileNumber(*i);
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
		if (f(MianziDat[i].mstat)&&(MianziDat[i].tile != TileCode::noTile)) {
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
	return countByMelds(MianziDat, Kezi, [](MeldStat x){return x >= MeldStat::tripletConcealed;});
}

/* 暗刻子の数を数える */
Int8ByTile yaku::countingFacility::countAnKez(const MeldBuf MianziDat, uint8_t* const Kezi) { /* 暗刻子の数を数える */
	trace(_T("暗刻子・暗槓子の種類を調べます。"));
	return countByMelds(MianziDat, Kezi, [](MeldStat x){return ((x == MeldStat::tripletConcealed)||(x == MeldStat::quadConcealed));});
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
	return countByMelds(MianziDat, Shunzi, [](MeldStat x){return x < MeldStat::tripletConcealed;});
}

/* 暗順子の数を数える */
Int8ByTile yaku::countingFacility::countAnShunz(const MeldBuf MianziDat, uint8_t* const Shunzi) { /* 暗順子の数を数える */
	trace(_T("暗順子の種類を調べます。"));
	return countByMelds(MianziDat, Shunzi, [](MeldStat x){return x == MeldStat::sequenceConcealed;});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* 槓子の数を数える */
Int8ByTile yaku::countingFacility::countKangz(const MeldBuf MianziDat, uint8_t* const Kangzi) { /* 槓子の数を数える */
	trace(_T("槓子の種類を調べます。"));
	return countByMelds(MianziDat, Kangzi, [](MeldStat x){return x >= MeldStat::quadConcealed;});
}

/* 暗槓子の数を数える */
Int8ByTile yaku::countingFacility::countAnKangz(const MeldBuf MianziDat, uint8_t* const Kangzi) { /* 暗槓子の数を数える */
	trace(_T("暗槓子の種類を調べます。"));
	return countByMelds(MianziDat, Kangzi, [](MeldStat x){return x == MeldStat::quadConcealed;});
}

/* 加槓の数を数える */
Int8ByTile yaku::countingFacility::countKaKangz(const MeldBuf MianziDat, uint8_t* const Kangzi) { /* 加槓の数を数える */
	trace(_T("加槓の種類を調べます。"));
	return countByMelds(MianziDat, Kangzi, [](MeldStat x){return x >= MeldStat::quadAddedLeft;});
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/* 指定の刻子・順子があるか数える */
template<std::size_t kSize, std::size_t sSize> int yaku::countingFacility::countSpecMentz(const MeldBuf MianziDat,
	const std::array<TileCode, kSize>& targetKez, const std::array<TileCode, sSize>& targetShunz, bool Mode, bool allowDup)
{ // 指定した種類の面子を数える
	int kz = 0; int sz = 0;
	auto DuiCount = Mode ? countKez(MianziDat, nullptr) : countDuiz(MianziDat);
	auto ShunzCount = countShunz(MianziDat, nullptr);
	int yakuflagcount = 0;
	for (auto i : targetKez) {
		if (i == TileCode::noTile) continue;
		if (DuiCount[i] >= 1) ++yakuflagcount;
	}
	for (auto i : targetShunz) {
		if (i == TileCode::noTile) continue;
		if (ShunzCount[i] >= 1)
			yakuflagcount += allowDup ? ShunzCount[i] : 1;
	}
	return yakuflagcount;
}

COUNT_SPEC_MENTZ( 1, 0);
COUNT_SPEC_MENTZ( 4, 0);
COUNT_SPEC_MENTZ( 5, 0);
COUNT_SPEC_MENTZ( 6, 0);
COUNT_SPEC_MENTZ( 7, 0);
COUNT_SPEC_MENTZ( 8, 0);
COUNT_SPEC_MENTZ(12, 0);
COUNT_SPEC_MENTZ(13, 0);

COUNT_SPEC_MENTZ( 3, 1);
COUNT_SPEC_MENTZ( 5, 1);
COUNT_SPEC_MENTZ( 6, 1);
COUNT_SPEC_MENTZ( 7, 1);
COUNT_SPEC_MENTZ( 8, 1);
COUNT_SPEC_MENTZ(10, 1);
COUNT_SPEC_MENTZ(11, 1);
COUNT_SPEC_MENTZ(14, 1);

COUNT_SPEC_MENTZ( 2, 2);

COUNT_SPEC_MENTZ( 3, 3);
COUNT_SPEC_MENTZ( 9, 3);
COUNT_SPEC_MENTZ(10, 3);

COUNT_SPEC_MENTZ( 7, 4);
COUNT_SPEC_MENTZ( 8, 4);
COUNT_SPEC_MENTZ(14, 4);

COUNT_SPEC_MENTZ( 6, 6);
COUNT_SPEC_MENTZ(12, 6);
COUNT_SPEC_MENTZ(13, 6);

COUNT_SPEC_MENTZ(10, 7);
COUNT_SPEC_MENTZ(11, 7);

COUNT_SPEC_MENTZ( 3, 8);
COUNT_SPEC_MENTZ(20, 8);

COUNT_SPEC_MENTZ( 3, 9);
COUNT_SPEC_MENTZ(15, 9);

/* 数字の合計を数える */
int yaku::countingFacility::countMentzNumerals(const MeldBuf MianziDat) { /* 数字の合計を数える */
	int Cifr = 0;
	for (int i = 0; i < SizeOfMeldBuffer; i++) {
		if (Tile(MianziDat[i].tile).isNumber()) {
			if (i == 0) { // 雀頭
				Cifr += getTileNumber(MianziDat[0].tile) * 2;
			} else { // 面子
				switch (MianziDat[i].mstat) {
				case MeldStat::sequenceConcealed: case MeldStat::sequenceExposedLower:
				case MeldStat::sequenceExposedMiddle: case MeldStat::sequenceExposedUpper:
					Cifr += (getTileNumber(MianziDat[i].tile) + 1) * 3; // 順子
					break;
				case MeldStat::tripletConcealed: case MeldStat::tripletExposedLeft:
				case MeldStat::tripletExposedCenter: case MeldStat::tripletExposedRight:
					Cifr += getTileNumber(MianziDat[i].tile) * 3; // 刻子
					break;
				default:
					Cifr += getTileNumber(MianziDat[i].tile) * 4; // 槓子
					break;
				}
			}
		}
	}
	return Cifr;
}
