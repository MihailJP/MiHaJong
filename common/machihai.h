#pragma once

#include <cstdint>
#include <type_traits>
#include "tilecode.h"

namespace mihajong_structs {

struct MachihaiTileInfo {
	bool MachihaiFlag; // 待ち牌になっているかのフラグ
	int8_t MachihaiCount; // 待ち牌の残り枚数
};
static_assert(std::is_pod<MachihaiTileInfo>::value, "MachihaiTileInfo is not POD");

template struct InfoByTile<MachihaiTileInfo>;
struct MachihaiInfo { // 待ち牌とかの情報を格納(chkFuriten関数用)
	bool FuritenFlag; // フリテンかどうかのフラグ
	InfoByTile<MachihaiTileInfo> Machihai; // 待ち牌情報
	int8_t MachihaiTotal; // 待ち牌の合計枚数
	int8_t MachiMen; // 何面待ち？
};
static_assert(std::is_pod<MachihaiInfo>::value, "MachihaiInfo is not POD");

struct TileStatus { // gettilestatus用
	bool isExistent, canFormQuad,
		seqMidWait, seqDoubleSideWait, seqSingleSideWait,
		formsPair, formsSequence, formsTriplet;
};
static_assert(std::is_pod<TileStatus>::value, "TileStatus is not POD");

} /* namespace */
