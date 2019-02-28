#pragma once

#include "exports.h"
#include "../common/seatrank.h"
#include "../common/gametbl.h"
#include "../common/machihai.h"
#include "../common/shanten.h"

namespace mihajong_graphic {
namespace utils {

using namespace mihajong_structs;

typedef seatRelative (*playerRelative_type)(PlayerID, PlayerID);
typedef PlayerID (*RelativePositionOf_type)(PlayerID, seatRelative);
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
typedef bool (*isPao_type)(const GameTable*, PlayerID, PlayerID);
typedef bool (*isPaoAgari_type)(const GameTable*, PlayerID);
typedef void (*bgmplay_type)(unsigned);
typedef LPCTSTR (*getName_type)(PlayerID);
typedef bool (*isWatchMode_type)();
typedef bool (*isAboveBase_type)(const GameTable*, PlayerID);
typedef bool (*isStandAlone_type)();

#ifdef GRAPHIC_EXPORTS
extern playerRelative_type playerRelative;
extern RelativePositionOf_type RelativePositionOf;
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
extern isPao_type isPao;
extern isPaoAgari_type isPaoAgari;
extern bgmplay_type bgmplay;
extern getName_type getName;
extern isWatchMode_type isWatchMode;
extern isAboveBase_type isAboveBase;
extern isStandAlone_type isStandAlone;
#endif

EXPORT void setfunc(
	playerRelative_type ptr_playerRelative,
	RelativePositionOf_type ptr_RelativePositionOf,
	calcRank_type ptr_calcRank,
	chkFuriten_type ptr_chkFuriten,
	isTenpai_type ptr_isTenpai,
	isRichiReqSatisfied_type ptr_isRichiReqSatisfied,
	calcShanten_type ptr_calcShanten,
	chkdaopaiability_type ptr_chkdaopaiability,
	chkShisanBuDa_type ptr_chkShisanBuDa,
	chkShisiBuDa_type ptr_chkShisiBuDa,
	countTilesInHand_type ptr_countTilesInHand,
	chkAnkanAbility_type ptr_chkAnkanAbility,
	isPao_type ptr_isPao,
	isPaoAgari_type ptr_isPaoAgari,
	bgmplay_type ptr_bgmplay,
	getName_type ptr_getName,
	isWatchMode_type ptr_isWatchMode,
	isAboveBase_type ptr_isAboveBase,
	isStandAlone_type ptr_isStandAlone);

}
}

#ifdef GRAPHIC_EXPORTS
namespace sound {
namespace util {
	using mihajong_graphic::utils::bgmplay;
}
}
#endif
