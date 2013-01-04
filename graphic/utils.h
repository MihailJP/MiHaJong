#pragma once

#include "exports.h"
#include "../mjcore/func.h"

namespace mihajong_graphic {
namespace utils {

typedef seatAbsolute (*playerwind_type)(const GameTable*, PLAYER_ID, int);
typedef seatRelative (*playerRelative_type)(PLAYER_ID, PLAYER_ID);
typedef PLAYER_ID (*RelativePositionOf_type)(PLAYER_ID, seatRelative);
typedef int (*tilesLeft_type)(const GameTable*);
typedef PlayerRankList (*calcRank_type)(const GameTable*);

#ifdef GRAPHIC_EXPORTS
extern playerwind_type playerwind;
extern playerRelative_type playerRelative;
extern RelativePositionOf_type RelativePositionOf;
extern tilesLeft_type tilesLeft;
extern calcRank_type calcRank;
#endif

EXPORT void setfunc(
	playerwind_type ptr_playerwind,
	playerRelative_type ptr_playerRelative,
	RelativePositionOf_type ptr_RelativePositionOf,
	tilesLeft_type ptr_tilesLeft,
	calcRank_type ptr_calcRank);

}
}
