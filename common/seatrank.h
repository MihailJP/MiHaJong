#pragma once

#include <cstdint>
#include <type_traits>
#include "gametbl.h"

namespace mihajong_structs {

typedef uint8_t PlayerRank;
template struct InfoByPlayer<PlayerRank>;
typedef InfoByPlayer<PlayerRank> PlayerRankList;
static_assert(std::is_pod<PlayerRankList>::value, "PlayerRankList is not POD");

} /* namespace */
