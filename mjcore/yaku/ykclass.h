#pragma once

#include <cstdint>
#include "../mjexport.h"

class yaku {
public:
	EXPORT_STRUCT YAKUSTAT;
	class yakuCalculator;

private:
	enum ParseOrder : uint8_t {
		Ke_Shun, Shun_Ke, Ke_Shun_Rev, Shun_Ke_Rev
	};
	struct ParseMode;
	class mentsuParser;
	class countingFacility;
};
