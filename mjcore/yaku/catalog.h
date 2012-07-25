#ifndef CATALOG_H
#define CATALOG_H

#include "yaku.h"

class yaku::yakuCalculator::YakuCatalog::catalogInit {
	friend yaku::yakuCalculator;
	friend yaku::yakuCalculator::YakuCatalog;
private:
	static void yakulst_contextual();
	static void yakulst_pinhu();
	static void yakulst_yaojiu();
};

#endif
