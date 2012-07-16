#ifndef YAKU_H
#define YAKU_H

#include <cstring>
#include <type_traits>
#include <functional>
#include <list>
#include <Windows.h>
#include "largenum.h"
#include "gametbl.h"
#include "tileutil.h"
#include "ykclass.h"

namespace yaku {
	EXPORT_STRUCT YAKUSTAT {
		bool isValid; // 和了っているかどうか
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
	}

	// ---------------------------------------------------------------------

	namespace countingFacility {
		int countPairs(
			const Int8ByTile tileCount, const tileCode* const targetTiles, int numOfTiles);
		int countTileNumerals(const Int8ByTile tileCount);

		Int8ByTile countByMelds(
			const MELD_BUF MianziDat, int* const hits,
			std::function<bool (meldStat)> f);

		Int8ByTile countKez(const MELD_BUF MianziDat, int* const Kezi);
		Int8ByTile countAnKez(const MELD_BUF MianziDat, int* const Kezi);
		Int8ByTile countDuiz(const MELD_BUF MianziDat);

		Int8ByTile countShunz(const MELD_BUF MianziDat);
		Int8ByTile countAnShunz(const MELD_BUF MianziDat);

		Int8ByTile countKangz(const MELD_BUF MianziDat, int* const Kangzi);
		Int8ByTile countAnKangz(const MELD_BUF MianziDat, int* const Kangzi);
		Int8ByTile countKaKangz(const MELD_BUF MianziDat, int* const Kangzi);

		int countSpecMentz(const MELD_BUF MianziDat, const tileCode* const targetKez, int numOfKez,
			const tileCode* const targetShunz, int numOfShunz, bool Mode);
		int countMentzNumerals(const MELD_BUF MianziDat);
	}

	// ---------------------------------------------------------------------

	namespace yakuCalculator {

		class YakuCatalog { // 役の一覧 [singleton]
		private:
			YakuCatalog() {}
			YakuCatalog(const YakuCatalog&);
			YakuCatalog& operator= (const YakuCatalog&);
		public:
			static YakuCatalog* Instantiate(); // Singleton instance accessor
			std::list<Yaku> catalog;
		};

		__declspec(dllexport) void init();

		struct MENTSU_ANALYSIS { // 面子解析結果
			PLAYER_ID player;
			SHANTEN shanten[SHANTEN_PAGES];
			MELD_BUF MianziDat; // 面子パース結果
			Int8ByTile KeziCount; // 刻子・槓子の数
			Int8ByTile AnKeziCount; // 暗刻・暗槓の数
			Int8ByTile DuiziCount; // 対子・刻子・槓子の数
			Int8ByTile ShunziCount; // 順子の数
			Int8ByTile AnShunziCount; // 鳴いていない順子の数
			Int8ByTile KangziCount; // 槓子の数
			Int8ByTile AnKangziCount; // 暗槓の数
			Int8ByTile KaKangziCount; // 加槓の数
		};
#ifdef MJCORE_EXPORTS
		static_assert(std::is_pod<MENTSU_ANALYSIS>::value, "MENTSU_ANALYSIS is not POD");
#endif

		class CalculatorThread {
		public:
			static DWORD WINAPI calculator(LPVOID lpParam);
			static int numOfRunningThreads(); // 動いているスレッドの数
			static const int threadLimit = 4; // 同時に起動する最大のスレッド数
			CalculatorThread(); // デフォルトコンストラクタ
			~CalculatorThread(); // デフォルトデストラクタ
		private:
			static void incThreadCount();
			static void decThreadCount();
			static int runningThreads;
			static CRITICAL_SECTION cs;
			DWORD WINAPI calculate(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, const ParseMode* const pMode);
		};

		struct CalculatorParam {
			ParseMode pMode;
			CalculatorThread* instance;
			const GameTable* gameStat;
			MENTSU_ANALYSIS analysis;
			YAKUSTAT result;
		};
#ifdef MJCORE_EXPORTS
		static_assert(std::is_pod<CalculatorParam>::value, "CalculatorParam is not POD");
#endif

		YAKUSTAT countyaku(const GameTable* const gameStat, PLAYER_ID targetPlayer);
		__declspec(dllexport) void countyaku(const GameTable* const gameStat,
			YAKUSTAT* const yakuInfo, int targetPlayer);
	}
}

#endif
