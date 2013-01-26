#pragma once

#include "yaku.h"
#include <array>

class yaku::yakuCalculator::YakuCatalog::catalogInit {
private:
	static yaku::yakuCalculator::Yaku::HANFUNC get_yaku_han(const std::string& RuleTag);
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
public:
	static void init();
};
