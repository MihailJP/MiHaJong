#ifndef CATALOG_H
#define CATALOG_H

#include "yaku.h"

class yaku::yakuCalculator::YakuCatalog::catalogInit {
	friend yaku::yakuCalculator;
	friend yaku::yakuCalculator::YakuCatalog;
private:
	static yaku::yakuCalculator::Yaku::HANFUNC get_yaku_han(const std::string& RuleTag);
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
};

#endif
