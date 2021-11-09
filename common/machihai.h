#pragma once

#include <cstdint>
#include <type_traits>
#include "tilecode.h"

namespace mihajong_structs {

struct MachihaiTileInfo {
	bool MachihaiFlag; // 待ち牌になっているかのフラグ
	int8_t MachihaiCount; // 待ち牌の残り枚数
};
static_assert(std::is_trivially_copyable<MachihaiTileInfo>::value, "MachihaiTileInfo is not trivially copyable");
static_assert(std::is_standard_layout<MachihaiTileInfo>::value, "MachihaiTileInfo is not standard layout");

template class InfoByTile<MachihaiTileInfo>;
struct MachihaiInfo { // 待ち牌とかの情報を格納(chkFuriten関数用)
	bool FuritenFlag; // フリテンかどうかのフラグ
	InfoByTile<MachihaiTileInfo> Machihai; // 待ち牌情報
	int8_t MachihaiTotal; // 待ち牌の合計枚数
	int8_t MachiMen; // 何面待ち？
	MachihaiInfo() : FuritenFlag(), Machihai(), MachihaiTotal(), MachiMen() {}
};
static_assert(std::is_trivially_copyable<MachihaiInfo>::value, "MachihaiInfo is not trivially copyable");
static_assert(std::is_standard_layout<MachihaiInfo>::value, "MachihaiInfo is not standard layout");

struct TileStatus { // gettilestatus用
	bool isExistent, canFormQuad,
		seqMidWait, seqDoubleSideWait, seqSingleSideWait,
		formsPair, formsSequence, formsTriplet;
	TileStatus() : isExistent(), canFormQuad(),
		seqMidWait(), seqDoubleSideWait(), seqSingleSideWait(),
		formsPair(), formsSequence(), formsTriplet()
	{}
};
static_assert(std::is_trivially_copyable<TileStatus>::value, "TileStatus is not trivially copyable");
static_assert(std::is_standard_layout<TileStatus>::value, "TileStatus is not standard layout");

} /* namespace */
