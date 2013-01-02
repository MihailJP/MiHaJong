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
};

EXPORT CallType getCall(PLAYER_ID playerID);
EXPORT void setCall(PLAYER_ID playerID, CallType callType);

}
}
