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
#include <thread>
#include <array>

class yaku::yakuCalculator {
public:
	enum class HanUnit : uint8_t {han, semiMangan, yakuman};
	class Yaku;

private:
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

	enum class MachiType : uint8_t { // 街の種類
		invalid, // 無効
		ryanmen, // 両面
		kanchan, // 嵌張
		penchan, // 辺張
		shanpon, // 双ポン
		tanki    // 単騎
	};
	struct ShantenData : public std::array<Shanten, SHANTEN_PAGES> {
		constexpr Shanten operator[] (ShantenType s) const { return std::array<Shanten, SHANTEN_PAGES>::operator[](static_cast<std::size_t>(s)); }
		reference operator[] (ShantenType s) { return std::array<Shanten, SHANTEN_PAGES>::operator[](static_cast<std::size_t>(s)); }
		constexpr Shanten operator[] (std::size_t s) const { return std::array<Shanten, SHANTEN_PAGES>::operator[](s); }
		reference operator[] (std::size_t s) { return std::array<Shanten, SHANTEN_PAGES>::operator[](s); }
		ShantenData() = default;
		ShantenData(const ShantenData&) = default;
		ShantenData& operator = (const ShantenData&) = default;
	};
	struct MentsuAnalysis { // 面子解析結果
		PlayerID player;
		ShantenData shanten;
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
		const PlayerTable* PlayerStat() const { // プレイヤー情報へのポインタ
			return &(GameStat->Player[player]);
		}
		const Tile& TsumoHai() const { // 和了牌
			return PlayerStat()->Tsumohai();
		}
		bool MenzenFlag() const { // 門前かどうかのフラグ
			return PlayerStat()->MenzenFlag;
		}
		bool TsumoAgariFlag() const { // ツモアガリどうかのフラグ
			return GameStat->TsumoAgariFlag;
		}
	};
#ifdef MJCORE_EXPORTS
	static_assert(std::is_trivially_copyable<MentsuAnalysis>::value, "MentsuAnalysis is not trivially copyable");
	static_assert(std::is_standard_layout<MentsuAnalysis>::value, "MentsuAnalysis is not standard layout");
#endif
	typedef std::function<bool (const MentsuAnalysis* const)> YAKUFUNC;

	struct CalculatorParam {
		ParseMode pMode;
		MentsuAnalysis analysis;
		YAKUSTAT result;
		CalculatorParam() : pMode(), analysis(), result() {}
	};
#ifdef MJCORE_EXPORTS
	static_assert(std::is_trivially_copyable<CalculatorParam>::value, "CalculatorParam is not trivially copyable");
	static_assert(std::is_standard_layout<CalculatorParam>::value, "CalculatorParam is not standard layout");
#endif

public:
	class Yaku {
	public:
		class YAKU_HAN {
		public:
			class HAN { // 翻
			public:
				HAN();
				HAN(int8_t h);
				HAN(int8_t h, HanUnit u);
				int8_t getHan() const;
				HanUnit getUnit() const;
			private:
				int8_t han; // 数値
#ifndef GUOBIAO
				HanUnit unit; // 単位
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
			std::function<YAKU_HAN (const MentsuAnalysis* const)> hFunc;
		public:
			YAKU_HAN operator() (const MentsuAnalysis* const) const;
			HANFUNC ();
			HANFUNC (std::function<YAKU_HAN (const MentsuAnalysis* const)>);
		};
		class FixedHan : public HANFUNC {
		public:
			FixedHan () : HANFUNC () {}
			FixedHan (YAKU_HAN bHan);
			FixedHan (YAKU_HAN::HAN cHan, YAKU_HAN::HAN dHan);
		};
#ifndef GUOBIAO
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
#endif /* GUOBIAO */
	private:
		HANFUNC han;
		CodeConv::tstring yakuName; // 役の名前（文字列）
		YAKUFUNC yakuProc; // 役の判定方法
		std::set<CodeConv::tstring> suppressionList; // 下位役のリスト
		Yaku() {} // Default constructor
	public:
		bool checkYaku(const MentsuAnalysis* const mentsu);
		CodeConv::tstring getName(); // 役の名前を取得する
		HANFUNC getHan();
		YAKU_HAN getHan(const MentsuAnalysis* const mentsu);
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

private:
	class CalculatorThread {
	public:
		static void calculator(YAKUSTAT* result, const ParseMode* pMode, const GameTable* gameStat, MentsuAnalysis* analysis);
		CalculatorThread() = delete;
		CalculatorThread(const CalculatorThread&) = delete;
		CalculatorThread& operator= (const CalculatorThread&) = delete;
		~CalculatorThread() = delete;
	private:
		static void calcbasepoints(const GameTable* const gameStat, MentsuAnalysis* const analysis);
		static void countDora(const GameTable* const gameStat, MentsuAnalysis* const analysis, YAKUSTAT* const result);
		static void checkPostponedYaku(
			const GameTable* const gameStat, MentsuAnalysis* const analysis, YAKUSTAT* const result,
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
		const ShantenData shanten, YAKUSTAT* const yakuInfo);
	static void analysisLoop(const GameTable* const gameStat, PlayerID targetPlayer,
		const ShantenData shanten, YAKUSTAT* const yakuInfo);

	static void countDora(const GameTable* const gameStat, MentsuAnalysis* const analysis,
		YAKUSTAT* const result, PlayerID targetPlayer);

public:
	static void init();

	static YAKUSTAT countyaku(const GameTable* const gameStat, PlayerID targetPlayer);
	static bool chkShisanBuDa(const GameTable* const gameStat, PlayerID targetPlayer);
	static bool chkShisiBuDa(const GameTable* const gameStat, PlayerID targetPlayer);
	static bool checkShibari(const GameTable* const gameStat, const YAKUSTAT* const yakuStat);
};

#ifdef GUOBIAO
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN operator"" _fen(unsigned long long fen);
yaku::yakuCalculator::Yaku::FixedHan operator"" _fenF(unsigned long long fen);
#else /* GUOBIAO */
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN operator"" _han(unsigned long long han);
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN operator"" _mangan(unsigned long long han);
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN operator"" _mangan(long double han);
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN operator"" _yakuman(unsigned long long han);
yaku::yakuCalculator::Yaku::YAKU_HAN::HAN operator"" _yakuman(long double han);
yaku::yakuCalculator::Yaku::FixedHan operator"" _hanF(unsigned long long han);
yaku::yakuCalculator::Yaku::KuisagariHan operator"" _hanK(unsigned long long han);
yaku::yakuCalculator::Yaku::MenzenHan operator"" _hanM(unsigned long long han);
yaku::yakuCalculator::Yaku::FixedHan operator"" _hanD(unsigned long long han);
yaku::yakuCalculator::Yaku::MenzenHan operator"" _hanMD(unsigned long long han);
yaku::yakuCalculator::Yaku::FixedHan operator"" _manganF(unsigned long long han);
yaku::yakuCalculator::Yaku::FixedHan operator"" _manganF(long double han);
yaku::yakuCalculator::Yaku::MenzenHan operator"" _manganM(unsigned long long han);
yaku::yakuCalculator::Yaku::MenzenHan operator"" _manganM(long double han);
yaku::yakuCalculator::Yaku::FixedHan operator"" _yakumanF(unsigned long long han);
yaku::yakuCalculator::Yaku::FixedHan operator"" _yakumanF(long double han);
yaku::yakuCalculator::Yaku::MenzenHan operator"" _yakumanM(unsigned long long han);
yaku::yakuCalculator::Yaku::MenzenHan operator"" _yakumanM(long double han);
yaku::yakuCalculator::Yaku::FixedHan operator"" _yakumanD(unsigned long long han);
yaku::yakuCalculator::Yaku::FixedHan operator"" _yakumanD(long double han);
yaku::yakuCalculator::Yaku::MenzenHan operator"" _yakumanMD(unsigned long long han);
yaku::yakuCalculator::Yaku::MenzenHan operator"" _yakumanMD(long double han);
#endif /* GUOBIAO */
