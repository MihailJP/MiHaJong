#pragma once

#include <cstdint>
#ifdef MJCORE_EXPORTS
#include <type_traits>
#include "logging.h"
#endif
#include "mjexport.h"
#include "except.h"

#include "../common/tilecode.h"

// 牌の種類
#define TileCodeMaximum      mihajong_structs::TileCodeMaximum
#define TileSuitStep         mihajong_structs::TileSuitStep
#define TILE_NUMERAL_COLORS    mihajong_structs::TileNumeralColors

#define TileSuitCharacters   mihajong_structs::TileSuitCharacters
#define TileSuitCircles      mihajong_structs::TileSuitCircles
#define TileSuitBamboos      mihajong_structs::TileSuitBamboos
#define TileSuitHonors       mihajong_structs::TileSuitHonors
#define TileNonflowerMax     mihajong_structs::TileNonflowerMax
#define TileSuitFlowers      mihajong_structs::TileSuitFlowers
#define TILE_FLOWER_MAX        mihajong_structs::TileFlowerMax

using mihajong_structs::TileCode;
using mihajong_structs::FlagByTile;
using mihajong_structs::Int8ByTile;
using mihajong_structs::UInt8ByTile;
