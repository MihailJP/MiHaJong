#ifndef YAKU_H
#define YAKU_H

#include <cstdint>
#include <type_traits>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <Windows.h>
#include "ykclass.h"
#include "yk_tools.h"
#include "../largenum.h"
#include "../gametbl.h"
#include "../tileutil.h"
#include "../except.h"

class yaku::yakuCalculator {
private:
	enum hanUnit : uint8_t {Han, SemiMangan, Yakuman};
	class Yaku;

	class YakuCatalog { // 役の一覧 [singleton]
		friend yaku::yakuCalculator;
	private:
		YakuCatalog() {}
		YakuCatalog(const YakuCatalog&);
		YakuCatalog& operator= (const YakuCatalog&);
		class catalogInit;
	public:
		static YakuCatalog* Instantiate(); // Singleton instance accessor
		std::list<Yaku> catalog;
	};

	enum MachiType : uint8_t { // 街の種類
		machiInvalid, // 無効
		machiRyanmen, // 両面
		machiKanchan, // 嵌張
		machiPenchan, // 辺張
		machiShanpon, // 双ポン
		machiTanki    // 単騎
	};
	struct MENTSU_ANALYSIS { // 面子解析結果
		PLAYER_ID player;
		SHANTEN shanten[SHANTEN_PAGES];
		MELD_BUF MianziDat; // 面子パース結果
		uint8_t BasePoint; // 符
		MachiType Machi; // 待ちの種類
		bool isPinfu; // 平和になってるかどうか
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
		int numOfRunningThreads(); // 動いているスレッドの数
		static const int threadLimit = 4; // 同時に起動する最大のスレッド数
		CalculatorThread(); // デフォルトコンストラクタ
		~CalculatorThread(); // デフォルトデストラクタ
	private:
		void incThreadCount();
		void decThreadCount();
		int runningThreads;
		CRITICAL_SECTION cs;
		static void calcbasepoints(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis);
		DWORD WINAPI calculate(
			const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis,
			const ParseMode* const pMode, YAKUSTAT* const result);
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
				static const HAN
					yv_null, yv_1han, yv_2han, yv_3han, yv_4han, yv_5han, yv_6han, yv_7han, yv_8han, 
					yv_mangan, yv_haneman, yv_baiman, yv_3baiman, yv_yakuman, yv_double_yakuman;
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
	public:
		/*typedef std::function<YAKU_HAN
			(const GameTable* const, const MENTSU_ANALYSIS* const)> HANFUNC;*/
		class HANFUNC {
		protected:
			YAKU_HAN base_Han;
		public:
			virtual YAKU_HAN operator() (const GameTable* const, const MENTSU_ANALYSIS* const);
			HANFUNC ();
			HANFUNC (YAKU_HAN bHan);
			HANFUNC (YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan);
		};
		class FixedHan : public HANFUNC {
		public:
			YAKU_HAN operator() (const GameTable* const, const MENTSU_ANALYSIS* const);
			FixedHan () : HANFUNC () {}
			FixedHan (YAKU_HAN bHan) : HANFUNC (bHan) {}
			FixedHan (YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan) : HANFUNC (cHan, dHan) {}
		};
		class MenzenHan : public HANFUNC {
		public:
			YAKU_HAN operator() (const GameTable* const, const MENTSU_ANALYSIS* const);
			MenzenHan () : HANFUNC () {}
			MenzenHan (YAKU_HAN bHan) : HANFUNC (bHan) {}
			MenzenHan (YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan) : HANFUNC (cHan, dHan) {}
		};
		class KuisagariHan : public HANFUNC {
		public:
			YAKU_HAN operator() (const GameTable* const, const MENTSU_ANALYSIS* const);
			KuisagariHan () : HANFUNC () {}
			KuisagariHan (YAKU_HAN bHan) : HANFUNC (bHan) {}
			KuisagariHan (YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan) : HANFUNC (cHan, dHan) {}
		};
		static const FixedHan yval_none, yval_1han, yval_2han, yval_3han, yval_4han, yval_5han, yval_6han,
			yval_mangan, yval_baiman, yval_yakuman, yval_double_yakuman;
		static const MenzenHan yval_1han_menzen, yval_2han_menzen, yval_3han_menzen,
			yval_4han_menzen, yval_5han_menzen, yval_6han_menzen;
		static const KuisagariHan yval_1han_kuisagari, yval_2han_kuisagari, yval_3han_kuisagari,
			yval_4han_kuisagari, yval_5han_kuisagari, yval_6han_kuisagari;
	private:
		HANFUNC han;
		std::string yakuName; // 役の名前（文字列）
		YAKUFUNC yakuProc; // 役の判定方法
		std::set<std::string> suppressionList; // 下位役のリスト
		Yaku() {} // Default constructor
	public:
		bool checkYaku(const GameTable* const gameStat, const MENTSU_ANALYSIS* const mentsu);
		std::string getName(); // 役の名前を取得する
		HANFUNC getHan();
		YAKU_HAN getHan(const GameTable* const gameStat, const MENTSU_ANALYSIS* const mentsu);
		std::set<std::string> getSuppression();
		// Constructor
		Yaku(std::string name, HANFUNC hanVal, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
			std::string yk6, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
			std::string yk6, std::string yk7, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
			std::string yk6, std::string yk7, std::string yk8, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
			std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
			std::string yk6, std::string yk7, std::string yk8, std::string yk9, YAKUFUNC f);
		Yaku(std::string name, HANFUNC hanVal,
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

#endif
