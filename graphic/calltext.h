#pragma once

#include "exports.h"
#ifdef GRAPHIC_EXPORTS
#include "gametbl.h"
#endif

namespace mihajong_graphic {
namespace calltext {

enum class CallType {
	none = -1,
	chii,
	pon,
	kan,
	riichi,
	ron,
	ronQualified,
	tsumo,
	flower,
	north,
	tenpai,
	noten,
	kyuushu,
	tahai,
	shouhai,
	kuikae,
	agariHouki,
	chonbo,
	nagashiMangan,
	renchan,
	oyanagare,
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
