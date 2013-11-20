#pragma once

#include <cstdint>
#include <type_traits>
#include <functional>
#include <list>
#include <set>
#include <map>
#include <vector>
#include "ykclass.h"
#include "yk_tools.h"
#include "../gametbl.h"
#include "../tileutil.h"
#include "../shanten.h"
#include "../../common/thread.h"

class yaku::yakuCalculator {
private:
	enum hanUnit : uint8_t {Han, SemiMangan, Yakuman};
	class Yaku;

	class YakuCatalog { // 役の一覧 [singleton]
		friend yaku::yakuCalculator;
	private:
		YakuCatalog() {}
		YakuCatalog(const YakuCatalog&) = delete; // Delete unexpected copy constructor
		YakuCatalog& operator= (const YakuCatalog&) = delete; // Delete unexpected copy constructor
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
		PlayerID player;
		Shanten shanten[SHANTEN_PAGES];
		MeldBuf MianziDat; // 面子パース結果
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
		const Tile* TsumoHai; // 和了牌へのポインタ
		const bool* MenzenFlag; // 門前かどうかのフラグ
		const bool* TsumoAgariFlag; // ツモアガリどうかのフラグ
	};
#ifdef MJCORE_EXPORTS
	static_assert(std::is_pod<MENTSU_ANALYSIS>::value, "MENTSU_ANALYSIS is not POD");
#endif
	typedef std::function<bool (const MENTSU_ANALYSIS* const)> YAKUFUNC;

	struct CalculatorParam {
		ParseMode pMode;
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
#ifdef GUOBIAO
				static const HAN
					yv_1, yv_2, yv_3, yv_4, yv_6, yv_8, yv_12, yv_16, yv_24, yv_32, yv_48, yv_64, yv_88;
#else /* GUOBIAO */
				static const HAN
					yv_null, yv_1han, yv_2han, yv_3han, yv_4han, yv_5han, yv_6han, yv_7han, yv_8han, 
					yv_mangan, yv_haneman, yv_baiman, yv_3baiman, yv_yakuman, yv_double_yakuman,
					yv_triple_yakuman, yv_quad_yakuman;
#endif /* GUOBIAO */
			private:
				int8_t han; // 数値
#ifndef GUOBIAO
				hanUnit unit; // 単位
#endif /* GUOBIAO */
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
			YAKU_HAN operator() (const MENTSU_ANALYSIS* const) const;
			HANFUNC ();
			HANFUNC (std::function<YAKU_HAN (const MENTSU_ANALYSIS* const)>);
		};
		class FixedHan : public HANFUNC {
		public:
			FixedHan () : HANFUNC () {}
			FixedHan (YAKU_HAN bHan);
			FixedHan (YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan);
		};
#ifdef GUOBIAO
		static const FixedHan
			yval_1, yval_2, yval_3, yval_4, yval_6, yval_8, yval_12, yval_16,
			yval_24, yval_32, yval_48, yval_64, yval_88;
#else /* GUOBIAO */
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
#endif /* GUOBIAO */
	private:
		HANFUNC han;
		CodeConv::tstring yakuName; // 役の名前（文字列）
		YAKUFUNC yakuProc; // 役の判定方法
		std::set<CodeConv::tstring> suppressionList; // 下位役のリスト
		Yaku() {} // Default constructor
	public:
		bool checkYaku(const MENTSU_ANALYSIS* const mentsu);
		CodeConv::tstring getName(); // 役の名前を取得する
		HANFUNC getHan();
		YAKU_HAN getHan(const MENTSU_ANALYSIS* const mentsu);
		std::set<CodeConv::tstring> getSuppression();
		// Constructor
		Yaku(CodeConv::tstring name, HANFUNC hanVal, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
			CodeConv::tstring yk6, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
			CodeConv::tstring yk6, CodeConv::tstring yk7, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
			CodeConv::tstring yk6, CodeConv::tstring yk7, CodeConv::tstring yk8, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
			CodeConv::tstring yk6, CodeConv::tstring yk7, CodeConv::tstring yk8, CodeConv::tstring yk9, YAKUFUNC f);
		Yaku(CodeConv::tstring name, HANFUNC hanVal,
			CodeConv::tstring yk1, CodeConv::tstring yk2, CodeConv::tstring yk3, CodeConv::tstring yk4, CodeConv::tstring yk5,
			CodeConv::tstring yk6, CodeConv::tstring yk7, CodeConv::tstring yk8, CodeConv::tstring yk9, CodeConv::tstring yk10,
			YAKUFUNC f);
	};

	class CalculatorThread {
	public:
		static void calculator(YAKUSTAT* result, const ParseMode* pMode, const GameTable* gameStat, MENTSU_ANALYSIS* analysis);
		CalculatorThread() = delete;
		CalculatorThread(const CalculatorThread&) = delete;
		CalculatorThread& operator= (const CalculatorThread&) = delete;
		~CalculatorThread() = delete;
	private:
		static void calcbasepoints(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis);
		static void countDora(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, YAKUSTAT* const result);
		static void checkPostponedYaku(
			const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis, YAKUSTAT* const result,
			std::map<CodeConv::tstring, Yaku::YAKU_HAN> &yakuHan, std::set<CodeConv::tstring> &suppression,
			std::vector<CodeConv::tstring> &yakuOrd);
		static void hanSummation(
			int& totalHan, int& totalSemiMangan, int& totalBonusHan, int& totalBonusSemiMangan,
			std::map<CodeConv::tstring, Yaku::YAKU_HAN> &yakuHan, std::vector<CodeConv::tstring> &yakuOrd, YAKUSTAT* const result);
	};

#ifndef GUOBIAO
	static void doubling(yaku::YAKUSTAT* const yStat);
#endif /* GUOBIAO */
	static void calculateScore(yaku::YAKUSTAT* const yStat);

	static void analysisNonLoop(const GameTable* const gameStat, PlayerID targetPlayer,
		Shanten* const shanten, YAKUSTAT* const yakuInfo);
	static void analysisLoop(const GameTable* const gameStat, PlayerID targetPlayer,
		Shanten* const shanten, YAKUSTAT* const yakuInfo);

	static void countDora(const GameTable* const gameStat, MENTSU_ANALYSIS* const analysis,
		YAKUSTAT* const result, PlayerID targetPlayer);

public:
	static void init();

	static YAKUSTAT countyaku(const GameTable* const gameStat, PlayerID targetPlayer);
	static bool chkShisanBuDa(const GameTable* const gameStat, PlayerID targetPlayer);
	static bool chkShisiBuDa(const GameTable* const gameStat, PlayerID targetPlayer);
	static bool checkShibari(const GameTable* const gameStat, const YAKUSTAT* const yakuStat);
};
