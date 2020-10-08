#pragma once

#include <cstdint>
#include "../mjexport.h"
#include "../../common/yakurslt.h"

class yaku {
public:
	typedef mihajong_structs::YakuResult YAKUSTAT;
	class yakuCalculator;

private:
	enum class ParseOrder : uint8_t {
		Ke_Shun, Shun_Ke, Ke_Shun_Rev, Shun_Ke_Rev
	};
	struct ParseMode;
	class mentsuParser;
	class countingFacility;
public: /* Monostate class: cannot be instantiated */
	yaku() = delete;
	yaku(const yaku&) = delete;
	yaku& operator= (const yaku&) = delete;
	~yaku() = delete;
};
