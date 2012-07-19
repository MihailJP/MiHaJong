#ifndef YAKU_H
#define YAKU_H

#include <cstring>
#include <type_traits>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <Windows.h>
#include "largenum.h"
#include "gametbl.h"
#include "tileutil.h"
#include "except.h"

class yaku {
public:
	EXPORT_STRUCT YAKUSTAT {
		static const int SemiMangan = 12500; // 半満貫
		static const int LimitMinus = 100; // マイナス翻の下限
		bool isValid; // 和了っているかどうか
		int BasePoints; // 符
		int CoreHan, CoreSemiMangan; // 翻
		int BonusHan, BonusSemiMangan; // 翻（縛りを満たさない）
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

private:
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

	class mentsuParser { // 面子パーサ
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

	class countingFacility {
	private:
		static Int8ByTile countByMelds(
			const MELD_BUF MianziDat, int* const hits,
			std::function<bool (meldStat)> f);
	public:
		static int countPairs(
			const Int8ByTile tileCount, const tileCode* const targetTiles, int numOfTiles);
		static int countTileNumerals(const Int8ByTile tileCount);

		static Int8ByTile countKez(const MELD_BUF MianziDat, int* const Kezi);
		static Int8ByTile countAnKez(const MELD_BUF MianziDat, int* const Kezi);
		static Int8ByTile countDuiz(const MELD_BUF MianziDat);

		static Int8ByTile countShunz(const MELD_BUF MianziDat);
		static Int8ByTile countAnShunz(const MELD_BUF MianziDat);

		static Int8ByTile countKangz(const MELD_BUF MianziDat, int* const Kangzi);
		static Int8ByTile countAnKangz(const MELD_BUF MianziDat, int* const Kangzi);
		static Int8ByTile countKaKangz(const MELD_BUF MianziDat, int* const Kangzi);

		static int countSpecMentz(const MELD_BUF MianziDat, const tileCode* const targetKez, int numOfKez,
			const tileCode* const targetShunz, int numOfShunz, bool Mode);
		static int countMentzNumerals(const MELD_BUF MianziDat);
	};

	// ---------------------------------------------------------------------

public:
	class yakuCalculator {
	private:
		enum hanUnit : uint8_t {Han, Mangan, Yakuman};
		class Yaku;

		class YakuCatalog { // 役の一覧 [singleton]
		private:
			YakuCatalog() {}
			YakuCatalog(const YakuCatalog&);
			YakuCatalog& operator= (const YakuCatalog&);
		public:
			static YakuCatalog* Instantiate(); // Singleton instance accessor
			std::list<Yaku> catalog;
		};

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
		typedef std::function<bool
			(const GameTable* const, const MENTSU_ANALYSIS* const)> YAKUFUNC;

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

		class Yaku {
		public:
			class YAKU_HAN {
			public:
				class HAN { // 翻
				public:
					HAN();
					HAN(int8_t h);
					HAN(int8_t h, hanUnit u);
					int8_t getHan();
					hanUnit getUnit();
				private:
					int8_t han; // 数値
					hanUnit unit; // 単位
				};
				HAN coreHan; // 縛りを満たす翻
				HAN bonusHan; // 縛りを満たさない翻
				YAKU_HAN();
				YAKU_HAN(HAN han);
				YAKU_HAN(HAN han, HAN bonus);
			};
		private:
			YAKU_HAN han;
			std::string yakuName; // 役の名前（文字列）
			YAKUFUNC yakuProc; // 役の判定方法
			std::set<std::string> suppressionList; // 下位役のリスト
			Yaku() {} // Default constructor
		public:
			bool checkYaku(const GameTable* const gameStat, const MENTSU_ANALYSIS* const mentsu);
			std::string getName(); // 役の名前を取得する
			YAKU_HAN getHan();
			std::set<std::string> getSuppression();
			// Constructor
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan, YAKUFUNC f);
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, YAKUFUNC f);
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, YAKUFUNC f);
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, YAKUFUNC f);
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, std::string yk4, YAKUFUNC f);
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5, YAKUFUNC f);
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
				std::string yk6, YAKUFUNC f);
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
				std::string yk6, std::string yk7, YAKUFUNC f);
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
				std::string yk6, std::string yk7, std::string yk8, YAKUFUNC f);
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
				std::string yk6, std::string yk7, std::string yk8, std::string yk9, YAKUFUNC f);
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
				std::string yk6, std::string yk7, std::string yk8, std::string yk9, std::string yk10,
				YAKUFUNC f);
		};

		static void analysisNonLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
			SHANTEN* const shanten, YAKUSTAT* const yakuInfo);
		static void analysisLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
			SHANTEN* const shanten, YAKUSTAT* const yakuInfo);

	public:
		static __declspec(dllexport) void init();

		static YAKUSTAT countyaku(const GameTable* const gameStat, PLAYER_ID targetPlayer);
		static __declspec(dllexport) void countyaku(const GameTable* const gameStat,
			YAKUSTAT* const yakuInfo, int targetPlayer);
	};
};
#endif
