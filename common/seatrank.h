#pragma once

#include <cstdint>
#include <type_traits>
#include "gametbl.h"

namespace mihajong_structs {

typedef uint8_t PlayerRank;
template struct InfoByPlayer<PlayerRank>;
typedef InfoByPlayer<PlayerRank> PlayerRankList;
static_assert(std::is_trivially_copyable<PlayerRankList>::value, "PlayerRankList is not trivially copyable");
static_assert(std::is_standard_layout<PlayerRankList>::value, "PlayerRankList is not standard layout");

} /* namespace */
