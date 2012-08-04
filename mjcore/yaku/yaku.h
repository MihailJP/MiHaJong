#ifndef YAKU_H
#define YAKU_H

#include <cstdint>
#include <string>
#include <type_traits>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <array>
#include <vector>
#include <Windows.h>
#include "ykclass.h"
#include "yk_tools.h"
#include "../largenum.h"
#include "../gametbl.h"
#include "../tileutil.h"
#include "../except.h"
#include "../logging.h"

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
		MachihaiInfo MachiInfo; // 待ち牌の数とか
		bool isPinfu; // 平和になってるかどうか
		Int8ByTile KeziCount; // 刻子・槓子の数
		Int8ByTile AnKeziCount; // 暗刻・暗槓の数
		Int8ByTile DuiziCount; // 対子・刻子・槓子の数
		Int8ByTile ShunziCount; // 順子の数
		Int8ByTile AnShunziCount; // 鳴いていない順子の数
		Int8ByTile KangziCount; // 槓子の数
		Int8ByTile AnKangziCount; // 暗槓の数
		Int8ByTile KaKangziCount; // 加槓の数
		Int8ByTile SeenTiles; // 見えている牌
		Int8ByTile TileCount; // 牌の数
		uint8_t TotalKezi; // 刻子槓子合計
		uint8_t TotalAnKezi; // 暗刻暗槓合計
		uint8_t TotalShunzi; // 順子合計
		uint8_t TotalAnShunzi; // 暗順子合計
		uint8_t TotalKangzi; // 槓子合計
		uint8_t TotalAnKangzi; // 暗槓合計
		uint8_t TotalKaKangzi; // 加槓合計
		const GameTable* GameStat; // 卓情報へのポインタ
		const PlayerTable* PlayerStat; // プレイヤー情報へのポインタ
		const TILE* TsumoHai; // 和了牌へのポインタ
		const bool* MenzenFlag; // 門前かどうかのフラグ
		const bool* TsumoAgariFlag; // ツモアガリどうかのフラグ
	};
#ifdef MJCORE_EXPORTS
	static_assert(std::is_pod<MENTSU_ANALYSIS>::value, "MENTSU_ANALYSIS is not POD");
#endif
	typedef std::function<bool (const MENTSU_ANALYSIS* const)> YAKUFUNC;

	class CalculatorThread {
	public:
		static DWORD WINAPI calculator(LPVOID lpParam);
		int numOfRunningThreads(); // 動いているスレッドの数
		static const int threadLimit = 4; // 同時に起動する最大のスレッド数
		int numOfStartedThreads(); // 開始したスレッドの数
		void sync(int threads); // スレッドを同期する
		CalculatorThread(); // デフォルトコンストラクタ
		~CalculatorThread(); // デフォルトデストラクタ
	private:
		void incThreadCount();
		void decThreadCount();
		int runningThreads;
		CRITICAL_SECTION cs;
		int startedThreads;
		static void calcbasepoints(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis);
		DWORD WINAPI calculate(
			const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis,
			const ParseMode* const pMode, YAKUSTAT* const result);
		static void countDora(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, YAKUSTAT* const result);
		static void doraText(YAKUSTAT* const result, const char* const label, int quantity);
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
				int8_t getHan() const;
				hanUnit getUnit() const;
				static const HAN
					yv_null, yv_1han, yv_2han, yv_3han, yv_4han, yv_5han, yv_6han, yv_7han, yv_8han, 
					yv_mangan, yv_haneman, yv_baiman, yv_3baiman, yv_yakuman, yv_double_yakuman,
					yv_triple_yakuman, yv_quad_yakuman;
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
		class HANFUNC {
		protected:
			std::function<YAKU_HAN (const MENTSU_ANALYSIS* const)> hFunc;
		public:
			YAKU_HAN operator() (const MENTSU_ANALYSIS* const);
			HANFUNC ();
			HANFUNC (std::function<YAKU_HAN (const MENTSU_ANALYSIS* const)>);
		};
		class FixedHan : public HANFUNC {
		public:
			FixedHan () : HANFUNC () {}
			FixedHan (YAKU_HAN bHan);
			FixedHan (YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan);
		};
		class MenzenHan : public HANFUNC {
		public:
			MenzenHan () : HANFUNC () {}
			MenzenHan (YAKU_HAN bHan);
			MenzenHan (YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan);
		};
		class KuisagariHan : public HANFUNC {
		public:
			KuisagariHan () : HANFUNC () {}
			KuisagariHan (YAKU_HAN bHan);
			KuisagariHan (YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan);
		};
		static const FixedHan yval_none, yval_1han, yval_2han, yval_3han, yval_4han, yval_5han, yval_6han,
			yval_mangan, yval_baiman, yval_yakuman, yval_double_yakuman,
			yval_triple_yakuman, yval_quad_yakuman,
			yval_1han_dependent, yval_2han_dependent, yval_4han_dependent, yval_yakuman_dependent;
		static const MenzenHan yval_1han_menzen, yval_2han_menzen, yval_3han_menzen,
			yval_4han_menzen, yval_5han_menzen, yval_6han_menzen,
			yval_mangan_menzen, yval_baiman_menzen, yval_yakuman_menzen, yval_double_yakuman_menzen,
			yval_1han_menzen_dependent, yval_2han_menzen_dependent, yval_yakuman_menzen_dependent;
		static const KuisagariHan yval_1han_kuisagari, yval_2han_kuisagari, yval_3han_kuisagari,
			yval_4han_kuisagari, yval_5han_kuisagari, yval_6han_kuisagari;
	private:
		HANFUNC han;
		std::string yakuName; // 役の名前（文字列）
		YAKUFUNC yakuProc; // 役の判定方法
		std::set<std::string> suppressionList; // 下位役のリスト
		Yaku() {} // Default constructor
	public:
		bool checkYaku(const MENTSU_ANALYSIS* const mentsu);
		std::string getName(); // 役の名前を取得する
		HANFUNC getHan();
		YAKU_HAN getHan(const MENTSU_ANALYSIS* const mentsu);
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

	static void doubling(yaku::YAKUSTAT* const yStat);
	static void calculateScore(yaku::YAKUSTAT* const yStat);

	static void analysisNonLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
		SHANTEN* const shanten, YAKUSTAT* const yakuInfo);
	static void analysisLoop(const GameTable* const gameStat, PLAYER_ID targetPlayer,
		SHANTEN* const shanten, YAKUSTAT* const yakuInfo);

public:
	static __declspec(dllexport) void init();

	static YAKUSTAT countyaku(const GameTable* const gameStat, PLAYER_ID targetPlayer);
	static __declspec(dllexport) void countyaku(const GameTable* const gameStat,
		YAKUSTAT* const yakuInfo, int targetPlayer);
	static bool chkShisanBuDa(const GameTable* const gameStat, PLAYER_ID targetPlayer);
	static __declspec(dllexport) int chkShisanBuDa(const GameTable* const gameStat, int targetPlayer);
	static bool chkShisiBuDa(const GameTable* const gameStat, PLAYER_ID targetPlayer);
	static __declspec(dllexport) int chkShisiBuDa(const GameTable* const gameStat, int targetPlayer);
};

#endif
