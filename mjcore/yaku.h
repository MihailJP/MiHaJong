#ifndef YAKU_H
#define YAKU_H

#include <cstring>
#include <type_traits>
#include <cstdio>
#include "largenum.h"
#include "gametbl.h"
#include "tileutil.h"

namespace yaku {
	EXPORT_STRUCT YAKUSTAT {
		int BasePoints; // 符
		int CoreHan; // 翻
		int BonusHan; // 翻（縛りを満たさない）
		int DoraQuantity; // ドラの数
		int UraDoraQuantity; // 裏ドラの数
		int AkaDoraQuantity; // 赤ドラの数
		int AoDoraQuantity; // 青ドラの数
		int AliceDora; // アリスドラの数
		int FlowerQuantity; // 花牌の数
		LargeNum AgariPoints; // 合計和了点
		char yakuNameList[1024];
		char yakuValList[1024];
		char yakumanNameList[1024];
		char yakumanValList[1024];
		// -----------------------------------------------------------------
		MELD_BUF MianziDat; // 面子に分解したデータ
		// -----------------------------------------------------------------
		static int getSize(); // 構造体のサイズを返す
		static void Init(YAKUSTAT* const myInstance); // インスタンスを初期化する
		static void exportYakuPoint(const YAKUSTAT* const myInstance, int* const exportArray);
		static void setYakuInfo(YAKUSTAT* const myInstance, int index, int value);
		static void setYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value);
		static void addYakuInfo(YAKUSTAT* const myInstance, int index, int value);
		static void addYakuInfo(YAKUSTAT* const myInstance, int index, const char* const value);
		static int getYakuInfo(const YAKUSTAT* const myInstance, int index);
		static void getYakuInfo(const YAKUSTAT* const myInstance, int index, char* const targetStr, int bufSize);
	};
#ifdef MJCORE_EXPORTS
	static_assert(std::is_pod<YAKUSTAT>::value, "YAKUSTAT is not POD");
#endif

	// ---------------------------------------------------------------------

	enum ParseOrder : uint8_t {
		Ke_Shun, Shun_Ke, Ke_Shun_Rev, Shun_Ke_Rev
	};
	struct ParseMode {
		tileCode AtamaCode;
		ParseOrder Order;
	};

#ifdef MJCORE_EXPORTS
	static_assert(std::is_pod<ParseMode>::value, "ParseMode is not POD");
#endif

	// ---------------------------------------------------------------------

	namespace mentsuParser { // 面子パーサ
		bool makementsu_shuntsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
			int* const ProcessedMelds, tileCode tile); /* 順子の処理 */
		void makementsu_koutsu(Int8ByTile& countForMentsu, MELD_BUF MianziDat,
			int* const ProcessedMelds, tileCode tile); /* 刻子の処理 */

		void makementsu(const GameTable* const gameStat,
			PLAYER_ID targetPlayer, ParseMode AtamaCode,
			int* const ProcessedMelds, MELD_BUF MianziDat); /* 面子に分解する */
		__declspec(dllexport) void makementsu(const GameTable* const gameStat,
			int targetPlayer, int AtamaCode, int* const ProcessedMelds, void* const MianziDat);
	}

	// ---------------------------------------------------------------------

	namespace countingFacility {
		/* 指定の対子があるか数える */
		int countPairs(
			const Int8ByTile tileCount, const tileCode* const targetTiles, int numOfTiles);
		__declspec(dllexport) int countPairs(
			const int* const tileCount, const int* const targetTiles, int numOfTiles);
		int countTileNumerals(const Int8ByTile tileCount);
		__declspec(dllexport) int countTileNumerals(const int* const tileCount);
	}
}

#endif
