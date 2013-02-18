#pragma once

#include "exports.h"
#include "../mjcore/func.h"
#include "../mjcore/tileutil.h"
#include "../common/shanten.h"

namespace mihajong_graphic {
namespace utils {

using namespace mihajong_structs;

typedef seatAbsolute (*playerwind_type)(const GameTable*, PlayerID, int);
typedef seatRelative (*playerRelative_type)(PlayerID, PlayerID);
typedef PlayerID (*RelativePositionOf_type)(PlayerID, seatRelative);
typedef int (*tilesLeft_type)(const GameTable*);
typedef PlayerRankList (*calcRank_type)(const GameTable*);
typedef MachihaiInfo (*chkFuriten_type)(const GameTable*, PlayerID);
typedef bool (*isTenpai_type)(const GameTable*, PlayerID);
typedef bool (*isRichiReqSatisfied_type)(const GameTable*, PlayerID);
typedef Shanten (*calcShanten_type)(const GameTable*, PlayerID, ShantenType);
typedef bool (*chkdaopaiability_type)(const GameTable*, PlayerID);
typedef bool (*chkShisanBuDa_type)(const GameTable*, PlayerID);
typedef bool (*chkShisiBuDa_type)(const GameTable*, PlayerID);
typedef Int8ByTile (*countTilesInHand_type)(const GameTable*, PlayerID);
typedef bool (*chkAnkanAbility_type)(const GameTable*, PlayerID);

#ifdef GRAPHIC_EXPORTS
extern playerwind_type playerwind;
extern playerRelative_type playerRelative;
extern RelativePositionOf_type RelativePositionOf;
extern tilesLeft_type tilesLeft;
extern calcRank_type calcRank;
extern chkFuriten_type chkFuriten;
extern isTenpai_type isTenpai;
extern isRichiReqSatisfied_type isRichiReqSatisfied;
extern calcShanten_type calcShanten;
extern chkdaopaiability_type chkdaopaiability;
extern chkShisanBuDa_type chkShisanBuDa;
extern chkShisiBuDa_type chkShisiBuDa;
extern countTilesInHand_type countTilesInHand;
extern chkAnkanAbility_type chkAnkanAbility;
#endif

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
	chkShisiBuDa_type ptr_chkShisiBuDa,
	countTilesInHand_type ptr_countTilesInHand,
	chkAnkanAbility_type ptr_chkAnkanAbility);

}
}
