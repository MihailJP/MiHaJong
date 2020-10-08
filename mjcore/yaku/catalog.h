#pragma once

#include "yaku.h"
#include <array>
#include "../ruletbl.h"

class yaku::yakuCalculator::YakuCatalog::catalogInit {
private:
#ifndef GUOBIAO
	static yaku::yakuCalculator::Yaku::HANFUNC get_yaku_han(const std::string& RuleTag);
#endif /* GUOBIAO */
	static bool isshoku (const MENTSU_ANALYSIS* const analysis, bool chin_itsu);
	static const std::array<char[8], 3> parsedat_monochrome5;
	static const std::array<char[8], 3> parsedat_monochrome4;
	static const std::array<char[4], 3> parsedat_monochrome3;
	static const std::array<char[8], 90> parsedat_bichrome5;
	static const std::array<char[8], 42> parsedat_bichrome4;
	static const std::array<char[4], 18> parsedat_bichrome3;
	static const std::array<char[8], 150> parsedat_trichrome5;
	static const std::array<char[8], 36> parsedat_trichrome4;
	static const std::array<char[4], 6> parsedat_trichrome3;
	static void yakulst_contextual();
	static void yakulst_pinhu();
	static void yakulst_irregular();
	static void yakulst_suit();
	static void yakulst_yaojiu();
	static void yakulst_triplet_1();
	static void yakulst_triplet_2();
	static void yakulst_triplet_3();
	static void yakulst_quad();
	static void yakulst_sequence();
	static void yakulst_misc();
	static void yakulst_dora();
	template <std::size_t kSize, std::size_t sSize> static bool chktiles(const MENTSU_ANALYSIS* const analysis,
		const std::array<TileCode, kSize> targetKez,
		const std::array<TileCode, sSize> targetShunz, bool noDui); // 面子手・七対子兼用の判定関数
	template <std::size_t kSize, std::size_t sSize> static bool chktiles_nodup(const MENTSU_ANALYSIS* const analysis,
		const std::array<TileCode, kSize> targetKez,
		const std::array<TileCode, sSize> targetShunz, bool noDui); // 面子手・七対子兼用の判定関数
	static bool isQiDui(const MENTSU_ANALYSIS* const analysis);
#ifndef GUOBIAO
	template <std::size_t dSize> static bool chktiles(const MENTSU_ANALYSIS* const analysis, const std::array<TileCode, dSize>& targetDuiz); // 判定関数
#endif /* GUOBIAO */
public:
	static void init();
public: /* Monostate class: cannot be instantiated */
	catalogInit() = delete;
	catalogInit(const catalogInit&) = delete;
	catalogInit& operator= (const catalogInit&) = delete;
	~catalogInit() = delete;
};
