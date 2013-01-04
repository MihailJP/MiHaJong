#include "utils.h"

namespace mihajong_graphic {
namespace utils {

playerwind_type playerwind = nullptr;
playerRelative_type playerRelative = nullptr;
RelativePositionOf_type RelativePositionOf = nullptr;
tilesLeft_type tilesLeft = nullptr;
calcRank_type calcRank = nullptr;

EXPORT void setfunc(
	playerwind_type ptr_playerwind,
	playerRelative_type ptr_playerRelative,
	RelativePositionOf_type ptr_RelativePositionOf,
	tilesLeft_type ptr_tilesLeft,
	calcRank_type ptr_calcRank)
{
	playerwind = ptr_playerwind;
	playerRelative = ptr_playerRelative;
	RelativePositionOf = ptr_RelativePositionOf;
	tilesLeft = ptr_tilesLeft;
	calcRank = ptr_calcRank;
}

}
}
