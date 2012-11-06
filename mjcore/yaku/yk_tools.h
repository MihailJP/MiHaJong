#ifndef YK_TOOLS_H
#define YK_TOOLS_H

#include <type_traits>
#include "ykclass.h"
#include "yakurslt.h"

struct yaku::ParseMode {
	tileCode AtamaCode;
	ParseOrder Order;
};


class yaku::mentsuParser { // 面子パーサ
#ifdef MJCORE_EXPORTS
	static_assert(std::is_pod<yaku::ParseMode>::value, "ParseMode is not POD");
#endif
private:
	static bool makementsu_shuntsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
		int* const ProcessedMelds, tileCode tile); /* 順子の処理 */
	static void makementsu_koutsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
		int* const ProcessedMelds, tileCode tile); /* 刻子の処理 */
public:
	static void makementsu(const GameTable* const gameStat,
		PLAYER_ID targetPlayer, ParseMode AtamaCode,
		int* const ProcessedMelds, MELD_BUF MianziDat); /* 面子に分解する */
};

// ---------------------------------------------------------------------

class yaku::countingFacility {
private:
	static Int8ByTile countByMelds(
		const MELD_BUF MianziDat, uint8_t* const hits,
		std::function<bool (meldStat)> f);
	static inline int countSpecMentz(const MELD_BUF MianziDat, const tileCode* const targetKez, int numOfKez,
		const tileCode* const targetShunz, int numOfShunz, bool Mode, bool allowDup);
public:
	static int countPairs(
		const Int8ByTile tileCount, const tileCode* const targetTiles, int numOfTiles);
	static int countTileNumerals(const Int8ByTile tileCount);

	static Int8ByTile countKez(const MELD_BUF MianziDat, uint8_t* const Kezi);
	static Int8ByTile countAnKez(const MELD_BUF MianziDat, uint8_t* const Kezi);
	static Int8ByTile countDuiz(const MELD_BUF MianziDat);

	static Int8ByTile countShunz(const MELD_BUF MianziDat, uint8_t* const Shunzi);
	static Int8ByTile countAnShunz(const MELD_BUF MianziDat, uint8_t* const Shunzi);

	static Int8ByTile countKangz(const MELD_BUF MianziDat, uint8_t* const Kangzi);
	static Int8ByTile countAnKangz(const MELD_BUF MianziDat, uint8_t* const Kangzi);
	static Int8ByTile countKaKangz(const MELD_BUF MianziDat, uint8_t* const Kangzi);

	static int countSpecMentz(const MELD_BUF MianziDat, const tileCode* const targetKez, int numOfKez,
		const tileCode* const targetShunz, int numOfShunz, bool Mode);
	static int countSpecMentzWithDup(const MELD_BUF MianziDat, const tileCode* const targetKez, int numOfKez,
		const tileCode* const targetShunz, int numOfShunz, bool Mode);
	static int countMentzNumerals(const MELD_BUF MianziDat);
};

#endif
