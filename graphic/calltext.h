#pragma once

#include "exports.h"
#ifdef GRAPHIC_EXPORTS
#include "gametbl.h"
#endif

namespace mihajong_graphic {
namespace calltext {

enum CallType {
	None = -1,
	Chii,
	Pon,
	Kan,
	Riichi,
	Ron,
	RonQualified,
	Tsumo,
	Flower,
	North,
	Tenpai,
	Noten,
	Kyuushu,
	Tahai,
	Shouhai,
	Kuikae,
	AgariHouki,
	Chonbo,
	NagashiMangan,
	Renchan,
	Oyanagare,
};

EXPORT CallType getCall(PlayerID playerID);
EXPORT void setCall(PlayerID playerID, CallType callType);

}
namespace callvalue {

struct CallValue {
	signed   short Mantissa;
	unsigned short Exponent;
};

EXPORT CallValue getVal(PlayerID playerID);
EXPORT void setVal(PlayerID playerID, signed short mantissa, unsigned short exponent);

}
}
