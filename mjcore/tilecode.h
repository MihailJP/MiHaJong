#pragma once

#include <cstdint>
#ifdef MJCORE_EXPORTS
#include <type_traits>
#include "logging.h"
#endif
#include "mjexport.h"
#include "except.h"

#include "../common/tilecode.h"

// ”v‚ÌŽí—Þ
#define TILE_CODE_MAXIMUM      mihajong_structs::TileCodeMaximum
#define TILE_SUIT_STEP         mihajong_structs::TileSuitStep
#define TILE_NUMERAL_COLORS    mihajong_structs::TileNumeralColors

#define TILE_SUIT_CHARACTERS   mihajong_structs::TileSuitCharacters
#define TILE_SUIT_CIRCLES      mihajong_structs::TileSuitCircles
#define TILE_SUIT_BAMBOOS      mihajong_structs::TileSuitBamboos
#define TILE_SUIT_HONORS       mihajong_structs::TileSuitHonors
#define TILE_NONFLOWER_MAX     mihajong_structs::TileNonflowerMax
#define TILE_SUIT_FLOWERS      mihajong_structs::TileSuitFlowers
#define TILE_FLOWER_MAX        mihajong_structs::TileFlowerMax

using mihajong_structs::TileCode;
using mihajong_structs::FlagByTile;
using mihajong_structs::Int8ByTile;
using mihajong_structs::UInt8ByTile;
