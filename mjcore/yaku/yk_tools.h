#pragma once

#include <type_traits>
#include <array>
#include <vector>
#include "ykclass.h"
#include "../../common/tilecode.h"

using namespace mihajong_structs;

struct yaku::ParseMode {
	TileCode AtamaCode;
	ParseOrder Order;
};


class yaku::mentsuParser { // 面子パーサ
#ifdef MJCORE_EXPORTS
	static_assert(std::is_trivially_copyable<yaku::ParseMode>::value, "ParseMode is not trivially copyable");
	static_assert(std::is_standard_layout<yaku::ParseMode>::value, "ParseMode is not standard layout");
#endif
private:
	static bool makementsu_shuntsu(Int8ByTile& countForMentsu, MeldBuf MianziDat,
		int* const ProcessedMelds, TileCode tile); /* 順子の処理 */
	static void makementsu_koutsu(Int8ByTile& countForMentsu, MeldBuf MianziDat,
		int* const ProcessedMelds, TileCode tile); /* 刻子の処理 */
public:
	static void makementsu(const GameTable* const gameStat,
		PlayerID targetPlayer, ParseMode AtamaCode,
		int* const ProcessedMelds, MeldBuf MianziDat); /* 面子に分解する */
public: /* Monostate class: cannot be instantiated */
	mentsuParser() = delete;
	mentsuParser(const mentsuParser&) = delete;
	mentsuParser& operator= (const mentsuParser&) = delete;
	~mentsuParser() = delete;
};

// ---------------------------------------------------------------------

class yaku::countingFacility {
private:
	static Int8ByTile countByMelds(
		const MeldBuf MianziDat, uint8_t* const hits,
		std::function<bool (MeldStat)> f);
	template<std::size_t kSize, std::size_t sSize> static int countSpecMentz(const MeldBuf MianziDat,
		const std::array<TileCode, kSize>& targetKez, const std::array<TileCode, sSize>& targetShunz, bool Mode, bool allowDup);
public:
	template<std::size_t dSize> static int countPairs(const Int8ByTile tileCount, const std::array<TileCode, dSize>& targetTiles);
	static int countTileNumerals(const Int8ByTile tileCount);

	static Int8ByTile countKez(const MeldBuf MianziDat, uint8_t* const Kezi);
	static Int8ByTile countAnKez(const MeldBuf MianziDat, uint8_t* const Kezi);
	static Int8ByTile countDuiz(const MeldBuf MianziDat);

	static Int8ByTile countShunz(const MeldBuf MianziDat, uint8_t* const Shunzi);
	static Int8ByTile countAnShunz(const MeldBuf MianziDat, uint8_t* const Shunzi);

	static Int8ByTile countKangz(const MeldBuf MianziDat, uint8_t* const Kangzi);
	static Int8ByTile countAnKangz(const MeldBuf MianziDat, uint8_t* const Kangzi);
	static Int8ByTile countKaKangz(const MeldBuf MianziDat, uint8_t* const Kangzi);

	template<std::size_t kSize, std::size_t sSize> static int countSpecMentz(const MeldBuf MianziDat,
		const std::array<TileCode, kSize>& targetKez,
		const std::array<TileCode, sSize>& targetShunz, bool Mode)
	{
		return countSpecMentz(MianziDat, targetKez, targetShunz, Mode, false);
	}
	template<std::size_t kSize, std::size_t sSize> static int countSpecMentzWithDup(const MeldBuf MianziDat,
		const std::array<TileCode, kSize>& targetKez,
		const std::array<TileCode, sSize>& targetShunz, bool Mode)
	{
		return countSpecMentz(MianziDat, targetKez, targetShunz, Mode, true);
	}

	static int countMentzNumerals(const MeldBuf MianziDat);
public: /* Monostate class: cannot be instantiated */
	countingFacility() = delete;
	countingFacility(const countingFacility&) = delete;
	countingFacility& operator= (const countingFacility&) = delete;
	~countingFacility() = delete;
};
