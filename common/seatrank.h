#pragma once

#include <cstdint>
#include <type_traits>
#include "gametbl.h"

namespace mihajong_structs {

enum seatAbsolute : uint8_t { sEast, sSouth, sWest, sNorth };
enum seatRelative : uint8_t { sSelf, sRight, sOpposite, sLeft };

typedef uint8_t PlayerRank;
template struct InfoByPlayer<PlayerRank>;
typedef InfoByPlayer<PlayerRank> PlayerRankList;
static_assert(std::is_pod<PlayerRankList>::value, "PlayerRankList is not POD");

} /* namespace */
