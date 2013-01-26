#include "utils.h"

namespace mihajong_graphic {
namespace utils {

playerwind_type playerwind = nullptr;
playerRelative_type playerRelative = nullptr;
RelativePositionOf_type RelativePositionOf = nullptr;
tilesLeft_type tilesLeft = nullptr;
calcRank_type calcRank = nullptr;
chkFuriten_type chkFuriten = nullptr;
isTenpai_type isTenpai = nullptr;

EXPORT void setfunc(
	playerwind_type ptr_playerwind,
	playerRelative_type ptr_playerRelative,
	RelativePositionOf_type ptr_RelativePositionOf,
	tilesLeft_type ptr_tilesLeft,
	calcRank_type ptr_calcRank,
	chkFuriten_type ptr_chkFuriten,
	isTenpai_type ptr_isTenpai)
{
	playerwind = ptr_playerwind;
	playerRelative = ptr_playerRelative;
	RelativePositionOf = ptr_RelativePositionOf;
	tilesLeft = ptr_tilesLeft;
	calcRank = ptr_calcRank;
	chkFuriten = ptr_chkFuriten;
	isTenpai = ptr_isTenpai;
}

}
}
