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
isRichiReqSatisfied_type isRichiReqSatisfied = nullptr;
calcShanten_type calcShanten = nullptr;
chkdaopaiability_type chkdaopaiability = nullptr;
chkShisanBuDa_type chkShisanBuDa = nullptr;
chkShisiBuDa_type chkShisiBuDa = nullptr;

EXPORT void setfunc(
	playerwind_type ptr_playerwind,
	playerRelative_type ptr_playerRelative,
	RelativePositionOf_type ptr_RelativePositionOf,
	tilesLeft_type ptr_tilesLeft,
	calcRank_type ptr_calcRank,
	chkFuriten_type ptr_chkFuriten,
	isTenpai_type ptr_isTenpai,
	isRichiReqSatisfied_type ptr_isRichiReqSatisfied,
	calcShanten_type ptr_calcShanten,
	chkdaopaiability_type ptr_chkdaopaiability,
	chkShisanBuDa_type ptr_chkShisanBuDa,
	chkShisiBuDa_type ptr_chkShisiBuDa)
{
	playerwind = ptr_playerwind;
	playerRelative = ptr_playerRelative;
	RelativePositionOf = ptr_RelativePositionOf;
	tilesLeft = ptr_tilesLeft;
	calcRank = ptr_calcRank;
	chkFuriten = ptr_chkFuriten;
	isTenpai = ptr_isTenpai;
	isRichiReqSatisfied = ptr_isRichiReqSatisfied;
	calcShanten = ptr_calcShanten;
	chkdaopaiability = ptr_chkdaopaiability;
	chkShisanBuDa = ptr_chkShisanBuDa;
	chkShisiBuDa = ptr_chkShisiBuDa;
}

}
}
